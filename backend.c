#include "common.h"
#include "hash_table.h"
#include "linked_list.h"
#include "utils.h"

/**
 * @file backend.c
 * @author Maja Nyberg & (Fadi Zemzemi)
 * @date 6 november 2023
 * @brief A backend implementation of a store.
 */

static bool equal_int(elem_t element_1, elem_t element_2)
{
    // Function is used when keys are integers
    return element_1.i == element_2.i;
}

static int hash_function_int(elem_t key)
{
    // Takes the integer value of a key and returns a bucket for that integer
    int int_key = key.i;
    int bucket = int_key % No_Buckets;
    return bucket;
}

static int hash_function_char(elem_t key)
{
    // Hash function for when key is char*
    char *char_key = key.p;
    int i = 0;
    int hash = 0;

    while (char_key[i] != '\0')
    {
        hash = (hash * 31) + char_key[i]; // 31 is a good prime number to use, Wikipedia
        i++;
    }

    int bucket = hash % No_Buckets;
    bucket = abs(bucket);
    return bucket;
}
static bool element_equal_char(elem_t a, elem_t b)
{
    // Function is used when wanting to compare two char *
    char *letter_a = a.p;
    char *letter_b = b.p;

    for (int i = 0; *letter_a != '\0'; i++)
    {
        if (*letter_a != *letter_b)
        {
            return false;
        }
        letter_a++;
        letter_b++;
    }

    return true;
}

static merch_t *make_merch(elem_t n, char *d, int p, ioopm_list_t *l)
{
    merch_t *m = calloc(1, sizeof(merch_t));

    m->name = n;
    m->desc = d;
    m->price = p;
    m->location = l;

    return m;
}

void ioopm_add_merch_to_db(ioopm_hash_table_t *db, elem_t merch_n, char *desc, int price)
{
    ioopm_list_t *location = ioopm_linked_list_create(element_equal_char);

    merch_t *m_ptr = make_merch(merch_n, desc, price, location);

    ioopm_hash_table_insert(db, m_ptr->name, ptr_elem(m_ptr));
}

void ioopm_list_merch_db(ioopm_hash_table_t *db)
{
    /*
    ioopm_list_t *merch_list = ioopm_hash_table_keys(db);
    linked_list_sort(merch_list);
    int counter = 0;
    link_t *current_link = merch_list->head;

    while (current_link != NULL)
    {
        char *print_link = (char *)current_link->value.p;
        printf("%d; %s\n", counter + 1, print_link);
        counter++;
        current_link = current_link->next;

        if (counter % 20 == 0)
        {
            char *question = ask_question_string("Vill du fortsätta [L]ista varor?\n");
            while (*question != 'L')
            {
                if (*question == 'N')
                free(question);
                break;
            }
            free(question);
        }
    }
    */
}

static void remove_merch_storage(ioopm_hash_table_t *storage, elem_t merch_n)
{
    ioopm_list_t *keys_to_remove = ioopm_hash_table_keys(storage);
    elem_t result;

    for (size_t i = 0; i < ioopm_linked_list_size(keys_to_remove); ++i)
    {
        elem_t current_key = ioopm_linked_list_get(keys_to_remove, i);
        ioopm_hash_table_lookup(storage, current_key, &result);
        char *current_merch = (char *)result.p;

        if (element_equal_char(ptr_elem(current_merch), merch_n))
        {
            ioopm_hash_table_remove(storage, current_key);
        }
    }
    ioopm_linked_list_destroy(keys_to_remove);
}

static void remove_merch_carts(ioopm_hash_table_t *carts, elem_t merch_n)
{
    elem_t result;
    for (int j = 0; j < No_Buckets; j++) // ASSUMPTION: There i is No_Buckets in common
    {
        if (ioopm_hash_table_lookup(carts, int_elem(j), &result))
        {
            ioopm_hash_table_t *single_cart = (ioopm_hash_table_t *)result.p;
            if (ioopm_hash_table_has_key(single_cart, merch_n)) // ASSUMPTION: Bara en specifik merch i varje cart
            {
                ioopm_hash_table_remove(single_cart, merch_n);
            }
        }
    }
}

void ioopm_remove_merch_db(ioopm_hash_table_t *db, ioopm_hash_table_t *storage, ioopm_hash_table_t *carts, elem_t merch_n)
{
    remove_merch_storage(storage, merch_n);

    remove_merch_carts(carts, merch_n);

    elem_t result;
    ioopm_hash_table_lookup(db, merch_n, &result);

    merch_t *m_ptr = (merch_t *)result.p;
    merch_t m = *m_ptr;

    ioopm_list_t *loc = m.location;

    ioopm_linked_list_destroy(loc);
    free(m_ptr->desc);
    ioopm_hash_table_remove(db, merch_n);
    free(merch_n.p);
    free(m_ptr);
}

void ioopm_edit_merch_db(ioopm_hash_table_t *db, ioopm_hash_table_t *storage, ioopm_hash_table_t *carts, elem_t old_key, elem_t new_key, char *new_desc, int new_price)
{
    /*
    merch_t *merch_result;
    elem_t result;
    ioopm_hash_table_lookup(db, old_key, &result);
    merch_result = (merch_t *)result.p;
    
    //Kolla om det är samma namn först
    if (new_key.p == old_key.p)
    {
        merch_result->desc = new_desc;
        merch_result->price = new_price;
    }

    ioopm_list_t *locations = merch_result->location;
    merch_t *new_merch;
    new_merch->desc = new_desc;
    new_merch->name = new_key;
    new_merch->price = new_price;
    new_merch->location = locations;
    elem_t merch_value = ptr_elem(new_merch);

    // Gå igenom db och spara location pointer och ändra
    //MÅSTE KOLLA HEAD->VALUE.P
    remove_merch_db(db, storage, carts, merch_result->name);
    ioopm_hash_table_insert(db, new_key, merch_value);
    // Gå igenom storage db och ändra alla merch types baserat på deras location

    ioopm_list_t *all_locations = ioopm_hash_table_keys(storage);
    ioopm_list_t *all_merchandise = ioopm_hash_table_values(storage);
    ioopm_list_iterator_t *loc_iterator = ioopm_list_iterator(all_locations);
    ioopm_list_iterator_t *merch_iterator = ioopm_list_iterator(all_merchandise);
    int list_size = ioopm_linked_list_size(all_locations);
    // Planen med denna loop är att den ska skapa två linked list med alla storage platser samt alla keys aka merch namn. Den ska sedan jämf-
    // öra merch namnen med det gamla merch namnet och om de är samma så tar vi bort hela entryn och insertar på nytt fast med det nya namnet
    // som value.
    for (int i = 0; i < list_size; i++)
    {
        elem_t current_loc_elem = ioopm_iterator_current(loc_iterator);
        char *current_merch_name = ioopm_iterator_current(merch_iterator).p;
        // Kolla om location->value(namn på merch) är samma
        if (strcmp(current_merch_name, (char *)merch_result->name.p))
        {
            ioopm_hash_table_remove(storage, ioopm_iterator_current(loc_iterator));
            ioopm_hash_table_insert(storage, current_loc_elem, new_key);
        }
        ioopm_iterator_next(loc_iterator);
        ioopm_iterator_next(merch_iterator);
    }
    // Gå igenom carts och ändra
    ioopm_list_iterator_t *cart_iterator = ioopm_list_iterator(carts);
    size_t carts_in_sys = ioopm_linked_list_size(carts);
    elem_t merch_quantity;
    ioopm_list_t *cart_merch;

    for (int i = 0; i < carts_in_sys; i++)
    {
        ioopm_hash_table_t *current_cart = ioopm_iterator_current(cart_iterator).p;
        cart_merch = ioopm_hash_table_keys(current_cart);

        // om den nuvarande carten har det gamla merch namnet i sig så måste vi ta bort varan och lägga till igen med rätt namn
        if (ioopm_linked_list_contains(cart_merch, old_key))
        {
            elem_t *result_tmp;
            ioopm_hash_table_lookup(current_cart, old_key, result_tmp);
            merch_quantity.i = result_tmp->i;
            ioopm_hash_table_remove(current_cart, old_key);
            ioopm_hash_table_insert(current_cart, old_key, merch_quantity);
            free(current_cart);
        }
        ioopm_iterator_next(cart_iterator);
    }

    // spara info, ta bort från ht och lägg in igen
    */
}

void ioopm_show_stock_db(ioopm_hash_table_t *db, elem_t merch_n)
{
    elem_t result;
    if (ioopm_hash_table_lookup(db, merch_n, &result))
    {
        merch_t *m = (merch_t *)result.p;
        ioopm_list_t *loc = m->location;
        int length = ioopm_linked_list_size(loc);

        if (length == 0)
        {
            printf("%s is not yet placed in storage.\n", (char*)merch_n.p);
        }
        else
        {
            for (int i = 0; i < length; i++)
            {
                elem_t current = ioopm_linked_list_get(loc, i);
                shelf_t *shelf_quant = (shelf_t *)current.p;
                printf("\nShelf: %s  Quantity: %d\n", shelf_quant->shelf, shelf_quant->quantity);
            }
        }
    }
}

void ioopm_replenish_db(ioopm_hash_table_t *db, ioopm_hash_table_t *storage, elem_t merch_n, shelf_t *shelf, int stock_increase)
{
    elem_t result_d;
    ioopm_hash_table_lookup(db, merch_n, &result_d);
    merch_t *result_db = (merch_t *)result_d.p;
    link_t *current_link = result_db->location->head;

    if (ioopm_linked_list_contains(result_db->location, ptr_elem(shelf->shelf)))
    {
        shelf_t *current_shelf;

        while (current_link)
        {
            current_shelf = (shelf_t *)current_link->value.p; 
            char *current_shelf_place = current_shelf->shelf;
            if (strcmp(current_shelf_place, shelf->shelf))
            {
                current_shelf->quantity = current_shelf->quantity + stock_increase;
                break;
            }
            current_link = current_link->next;
        } 
    }
    else 
    {
        elem_t result;
        shelf->quantity = stock_increase;

        ioopm_linked_list_append(result_db->location, ptr_elem(shelf));
        if (!ioopm_hash_table_lookup(storage, ptr_elem(shelf->shelf), &result))
        {
            ioopm_hash_table_insert(storage, ptr_elem(shelf->shelf), result_db->name);
        }
    }
}

static void reduce_stock(ioopm_hash_table_t *db, ioopm_hash_table_t *storage, elem_t merch_n, int quantity)
{
    elem_t result;
    if (ioopm_hash_table_lookup(db, merch_n, &result))
    {
        merch_t *merch_ptr = (merch_t *)result.p; // get merch that should be reduced
        ioopm_list_t *loc = merch_ptr->location;  // get it's location list
        int quantity_counter = 0;
        while (quantity_counter != quantity)
        {
            shelf_t *shelf = (shelf_t *)ioopm_linked_list_get(loc, 0).p; // get the shelf_t (shelf name and quantity of merch)
            int current_quant = shelf->quantity;
            for (int j = 0; j <= quantity && current_quant>0 && shelf->quantity >= 0 && quantity_counter != quantity; j++) // reduce the stock of the merch on teh current shelf until wanted quantity is fulfilled
            {
                current_quant = current_quant - 1;
                shelf->quantity = current_quant; // reducing
                quantity_counter = quantity_counter + 1;

                if (shelf->quantity == 0) // if the shelf is empty remove it from storage and loc list
                {
                    ioopm_hash_table_remove(storage, ptr_elem(shelf->shelf));
                    ioopm_linked_list_remove(loc, 0);
                }
            }
        }
    }
}

static void increase_stock(ioopm_hash_table_t *db, ioopm_hash_table_t *storage, elem_t merch_n, int quantity)
{
    elem_t result;
    if (ioopm_hash_table_lookup(db, merch_n, &result))
    {
        merch_t *merch_ptr = (merch_t *)result.p; // get merch that should be increased
        ioopm_list_t *loc = merch_ptr->location;  // get it's location list
        int quantity_counter = 0;
        for (int i = 0; i < ioopm_linked_list_size(loc); i++) // go through each location
        {
            shelf_t *shelf = (shelf_t *)ioopm_linked_list_get(loc, i).p; // get the shelf_t (shelf name and quantity of merch)
            int current_quant = shelf->quantity;
            for (int j = 0; j <= quantity && quantity_counter != quantity; j++) // reduce the stock of the merch on teh current shelf until wanted quantity is fulfilled
            {
                current_quant = current_quant + 1;
                shelf->quantity = current_quant; // reducing
                quantity_counter = quantity_counter + 1;
            }
        }
    }
}

static bool in_stock(ioopm_hash_table_t *db, ioopm_hash_table_t *storage, elem_t merch_n, int quantity)
{
    elem_t result;
    if (ioopm_hash_table_lookup(db, merch_n, &result))
    {
        merch_t *merch_ptr = (merch_t *)result.p;
        ioopm_list_t *loc = merch_ptr->location;
        int sum = 0;
        for (int i = 0; i< ioopm_linked_list_size(loc); i++)
        {
            shelf_t *shelf = (shelf_t *)ioopm_linked_list_get(loc, i).p;
            int current_quant = shelf->quantity;
            sum = sum + current_quant;
            if (quantity <= sum)
            {
                return true;
            }
        }
    }
    return false;
}

void ioopm_print_carts_db(ioopm_hash_table_t *carts)
{
    elem_t result;

    ioopm_list_t *carts_nrs = ioopm_hash_table_keys(carts); // get all cart numbers

    for (int i = 0; i < ioopm_linked_list_size(carts_nrs); i++)
    {
        int cart_nr = ioopm_linked_list_get(carts_nrs, i).i; // get a single cart number

        ioopm_hash_table_lookup(carts, int_elem(cart_nr), &result);
        ioopm_hash_table_t *single_cart = (ioopm_hash_table_t *)result.p; // the cart represented by cart_nr

        ioopm_list_t *merch_list = ioopm_hash_table_keys(single_cart);
        ioopm_list_t *quantity_list = ioopm_hash_table_values(single_cart);

        printf("\nCart %d\n", cart_nr);
        for (int j = 0; j < ioopm_hash_table_size(single_cart); j++)
        {
            char *merch = (char *)ioopm_linked_list_get(merch_list, j).p;
            int quantity = ioopm_linked_list_get(quantity_list, j).i;
            printf("Merch: %s  Quantity: %d\n", merch, quantity);
        }
        ioopm_linked_list_destroy(merch_list);
        ioopm_linked_list_destroy(quantity_list);
    }
    ioopm_linked_list_destroy(carts_nrs);
    
}


void ioopm_create_cart_db(ioopm_hash_table_t *carts, int *total_carts)
{
    ioopm_hash_table_t *new_cart = ioopm_hash_table_create(hash_function_char, element_equal_char, equal_int);
    *total_carts = *total_carts + 1;
    int cart_nr = *total_carts;
    ioopm_hash_table_insert(carts, int_elem(cart_nr), ptr_elem(new_cart));
    printf("\nYour cart number: %d\nEmpty cart\n", cart_nr);
}

void ioopm_remove_cart_db(ioopm_hash_table_t *db, ioopm_hash_table_t *storage, ioopm_hash_table_t *carts, int cart_nr)
{
    elem_t result;
    ioopm_hash_table_lookup(carts, int_elem(cart_nr), &result);

    ioopm_hash_table_t *single_cart = (ioopm_hash_table_t *)result.p;
    ioopm_list_t *keys = ioopm_hash_table_keys(single_cart);
    ioopm_list_t *values = ioopm_hash_table_values(single_cart);

    for (int i = 0; i<ioopm_linked_list_size(keys); i++)
    {
        elem_t m_name = ioopm_linked_list_get(keys, i);
        int quantity = ioopm_linked_list_get(values, i).i;
        increase_stock(db, storage, m_name, quantity);
    }
    ioopm_hash_table_clear(single_cart);
    ioopm_hash_table_remove(carts, int_elem(cart_nr));
    free(single_cart);
    ioopm_linked_list_destroy(keys);
    ioopm_linked_list_destroy(values);
}

void ioopm_add_to_cart_db(ioopm_hash_table_t *db, ioopm_hash_table_t *storage, ioopm_hash_table_t *carts, int cart_nr, elem_t merch_n, int quantity)
{
    // checka finns det i hash table, redan gjort??
    elem_t result;
    // checka finns det i storage

    while (!in_stock(db, storage, merch_n, quantity))
    {
        quantity = ask_question_int("\nThe requested quantity is not in stock, please request another quantity:\n");
    }
    // insert i rätt cart
    ioopm_hash_table_lookup(carts, int_elem(cart_nr), &result);
    ioopm_hash_table_t *single_cart = (ioopm_hash_table_t *)result.p;
    if (ioopm_hash_table_has_key(single_cart, merch_n))
    {
        ioopm_hash_table_lookup(single_cart, merch_n, &result);
        int new_quantity = result.i;
        new_quantity = new_quantity+quantity;

        ioopm_hash_table_remove(single_cart, merch_n);
        ioopm_hash_table_insert(single_cart, merch_n, int_elem(new_quantity));
        reduce_stock(db, storage, merch_n, quantity);
    }
    else
    {
        ioopm_hash_table_insert(single_cart, merch_n, int_elem(quantity));
        reduce_stock(db, storage, merch_n, quantity);
    }
}

void ioopm_remove_from_cart_db(ioopm_hash_table_t *db, ioopm_hash_table_t *storage, ioopm_hash_table_t *carts, int cart_nr, elem_t merch_n, int quantity)
{
    elem_t result;
    elem_t result2;
    ioopm_hash_table_lookup(carts, int_elem(cart_nr), &result);
    ioopm_hash_table_t *single_cart = (ioopm_hash_table_t *)result.p;
    if (ioopm_hash_table_has_key(single_cart, merch_n))
    {
        ioopm_hash_table_lookup(single_cart, merch_n, &result2);
        int available_quantity = (int) result2.i;
        int check = available_quantity-quantity;
        if (check == 0)
        {
            ioopm_hash_table_remove(single_cart, merch_n);
            increase_stock(db, storage, merch_n, quantity);
        }
        else
        {
            ioopm_hash_table_remove(single_cart, merch_n);
            ioopm_hash_table_insert(single_cart, merch_n, int_elem(check));
            increase_stock(db, storage, merch_n, quantity);
        }
    }
}

int ioopm_calculate_cost_db(ioopm_hash_table_t *db, ioopm_hash_table_t *carts, int cart_nr)
{
    elem_t result;
    elem_t result_price;
    int sum = 0;

    ioopm_hash_table_lookup(carts, int_elem(cart_nr), &result);
    ioopm_hash_table_t *single_cart = (ioopm_hash_table_t *)result.p;

    ioopm_list_t *merch_list = ioopm_hash_table_keys(single_cart);
    ioopm_list_t *quantity_list = ioopm_hash_table_values(single_cart);

    printf("Cart %d\n", cart_nr);
    for (int j = 0; j < ioopm_hash_table_size(single_cart); j++)
    {
        char *merch_name = (char *)ioopm_linked_list_get(merch_list, j).p;
        int quantity = ioopm_linked_list_get(quantity_list, j).i;

        ioopm_hash_table_lookup(db, ptr_elem(merch_name), &result_price);
        merch_t *merch = (merch_t *)result_price.p;
        int price = merch->price;
        sum = sum + (price * quantity);
    }
    ioopm_linked_list_destroy(merch_list);
    ioopm_linked_list_destroy(quantity_list);

    return sum;
}

void ioopm_checkout_db(ioopm_hash_table_t *db, ioopm_hash_table_t *storage, ioopm_hash_table_t *carts, int cart_nr)
{
    elem_t result;

    ioopm_hash_table_lookup(carts, int_elem(cart_nr), &result);
    ioopm_hash_table_t *single_cart = (ioopm_hash_table_t *)result.p;

    int cost = ioopm_calculate_cost_db(db, carts, cart_nr);

    printf("Total cost: %d\n", cost);
    printf("Thank you for shopping with us!\n");

    ioopm_hash_table_destroy(single_cart);
    ioopm_hash_table_remove(carts, int_elem(cart_nr));
}

int ioopm_quit_db(ioopm_hash_table_t *db, ioopm_hash_table_t *storage, ioopm_hash_table_t *carts, int *total_carts, ioopm_list_t *shelves)
{   
    // carts
    ioopm_list_t *keys_carts = ioopm_hash_table_keys(carts);

    for (int i = 0; i < ioopm_linked_list_size(keys_carts); i++)
    {
        int cart_nr = ioopm_linked_list_get(keys_carts, i).i;
        ioopm_remove_cart_db(db, storage, carts, cart_nr);
    }

    // db
    ioopm_list_t *keys_db = ioopm_hash_table_keys(db);

    for (int i = 0; i < ioopm_linked_list_size(keys_db); i++)
    {
        elem_t name = ioopm_linked_list_get(keys_db, i);
        ioopm_remove_merch_db(db, storage, carts, name);
    }
    ioopm_linked_list_destroy(keys_db);
    ioopm_hash_table_destroy(db);

    for (int j = 0; ioopm_linked_list_size(shelves)!=0; j++)
    {
        shelf_t *shelf = ioopm_linked_list_get(shelves, 0).p;
        free(shelf->shelf);
        ioopm_linked_list_remove(shelves, 0);
        free(shelf);
    }
    ioopm_linked_list_destroy(shelves);
    // storage
    ioopm_hash_table_destroy(storage);
    ioopm_linked_list_destroy(keys_carts);
    ioopm_hash_table_destroy(carts);
    free(total_carts);

    return 0;
}

ioopm_hash_table_t *ioopm_create_carts()
{
    ioopm_hash_table_t *carts = ioopm_hash_table_create(hash_function_int, equal_int, equal_int);
    return carts;
}

ioopm_hash_table_t *ioopm_create_database_part()
{
    ioopm_hash_table_t *db = ioopm_hash_table_create(hash_function_char, element_equal_char, element_equal_char);
    return db;
}

ioopm_list_t *ioopm_create_shelves()
{
    ioopm_list_t *shelves = ioopm_linked_list_create(element_equal_char);
    return shelves;
}
