#include "frontend.h"
#include "utils.h"
#include "hash_table.h"
#include "backend.h"
#include "common.h"
#include "linked_list.h"

/**
 * @file frontend.c
 * @author Maja Nyberg & (Fadi Zemzemi)
 * @date 6 november 2023
 * @brief A simple user interface for a store.
 */
void add_merch(ioopm_hash_table_t *db)
{
    elem_t name = ptr_elem(NULL);
    do
    {
        char *input_str = ask_question_string("\nName:\n");
        name.p = strdup(input_str);

        free(input_str);

    } while (ioopm_hash_table_has_key(db, name));

    char *desc = ask_question_string("Description:\n");
    int price;
    do
    {
        price = ask_question_int("Price:\n");

    } while (price<0);

    ioopm_add_merch_to_db(db, name, desc, price);
}

void list_merch(ioopm_hash_table_t *db)
{
    // NOT IMPLEMENTED, ACCEPTED BY ELIAS
}

void remove_merch(ioopm_hash_table_t *db, ioopm_hash_table_t *storage, ioopm_hash_table_t *carts)
{
    list_merch(db);
    elem_t key = ptr_elem(NULL);
    do
    {
        if (ioopm_hash_table_size(db) == 0)
        {
            printf("\nThere are no merch to remove!\n");
            return;
        }
        char *input_str = ask_question_string("\nWhich merchandise do you want to remove?\n");
        key.p = strdup(input_str);

        free(input_str);

        if (!ioopm_hash_table_has_key(db, key))
        {
            printf("Merchandise not found. Please try again.\n");
        }
    } while (!ioopm_hash_table_has_key(db, key));

    ioopm_remove_merch_db(db, storage, carts, key);

    list_merch(db);
}

void edit_merch(ioopm_hash_table_t *db, ioopm_hash_table_t *storage, ioopm_hash_table_t *carts)
{
    // NOT IMPLEMENTED, ACCEPTED BY ELIAS
}

void show(ioopm_hash_table_t *db)
{
    list_merch(db);
    elem_t key = ptr_elem(NULL);
    do
    {
        if (ioopm_hash_table_size(db) == 0)
        {
            printf("\nThere are no items to show!\n");
            return;
        }
        char *input_str = ask_question_string("Which merchandise's stock do you want to see?\n");
        key.p = strdup(input_str);

        free(input_str);

        if (!ioopm_hash_table_has_key(db, key))
        {
            printf("Merchandise not found. Please try again.\n");
        }
    } while (!ioopm_hash_table_has_key(db, key));

    ioopm_show_stock_db(db, key);

    free(key.p);
}

void replenish_stock(ioopm_hash_table_t *db, ioopm_hash_table_t *storage, ioopm_hash_table_t *carts, ioopm_list_t *shelves)
{
    list_merch(db);
    elem_t key;
    elem_t shelf_func;
    elem_t result_lookup;
    char *name_cmp;
    shelf_t *shelf = calloc(1, sizeof(shelf_t));
    int stock_increase;
    do
    {
        if (ioopm_hash_table_size(db) == 0)
        {
            printf("\nThere are no items to replenish!\n");
            return;
        }
        char *input_str = ask_question_string("\nWhich merchandise do you want to replenish?\n");
        key.p = strdup(input_str);

        free(input_str);
    } while (!ioopm_hash_table_has_key(db, key));

    if (ioopm_hash_table_has_key(db, key))
    {
        printf("This merchandise are available on these shelves:\n");
        ioopm_show_stock_db(db, key);
        stock_increase = ask_question_int("How much would you like to increase the stock with?\n");
        while (stock_increase < 1)
        {
            printf("The increase needs to be equal to or higher than 1!\n");
            stock_increase = ask_question_int("How much would you like to increase the stock with?;\n");
        }
        char *shelf_input = ask_question_shelf("Where would you like to put the newly replenished stock?\n");
        shelf->shelf = strdup(shelf_input);
        shelf_func = ptr_elem(shelf->shelf);

        ioopm_linked_list_append(shelves, ptr_elem(shelf));

        free(shelf_input);
        ioopm_hash_table_lookup(storage, shelf_func, &result_lookup);
        name_cmp = (char *)result_lookup.p;
        

        while (ioopm_hash_table_has_key(storage, shelf_func) && strcmp(name_cmp, (char *)key.p) !=0) /*Kolla så att det inte är samma value som vår merch*/
        {
            printf("This shelf is already in use by another merchandise, please enter a new one;\n");
            char *shelf_input = ask_question_shelf("Where would you like to put the newly replenished stock?\n");
            shelf->shelf = strdup(shelf_input);
            shelf_func = ptr_elem(shelf->shelf);
            ioopm_linked_list_append(shelves, ptr_elem(shelf));


            free(shelf_input);
        }
        ioopm_replenish_db(db, storage, key, shelf, stock_increase);
    }
    free(key.p);
}

void create_cart(ioopm_hash_table_t *carts, int *total_carts)
{
    ioopm_create_cart_db(carts, total_carts);
}

void remove_cart(ioopm_hash_table_t *db, ioopm_hash_table_t *storage, ioopm_hash_table_t *carts)
{
    ioopm_print_carts_db(carts);
    int cart_nr = 0;
    do
    {
        if (ioopm_hash_table_size(carts) == 0)
        {
            printf("\nThere are no carts to remove!\n");
            return;
        }
        cart_nr = ask_question_int("\nWhat cart would you like to remove?\n");

    } while (!ioopm_hash_table_has_key(carts, int_elem(cart_nr)));

    ioopm_remove_cart_db(db, storage, carts, cart_nr);
}

void add_to_cart(ioopm_hash_table_t *db, ioopm_hash_table_t *storage, ioopm_hash_table_t *carts)
{
    ioopm_print_carts_db(carts);
    elem_t cart_nr = int_elem(0);
    elem_t merch = ptr_elem(NULL);
    do
    {
        if (ioopm_hash_table_size(carts) == 0)
        {
            printf("\nThere are no carts to add to!\n");
            return;
        }
        cart_nr.i = ask_question_int("\nWhat cart would you like to add to?\n");

    } while (!ioopm_hash_table_has_key(carts, cart_nr));

    list_merch(db);
    do
    {
        if (ioopm_hash_table_size(db) == 0)
        {
            printf("\nThere are no items in the database!\n");
            return;
        }
        char *input_str2 = ask_question_string("\nWhat item would you like to add to your cart?\n");
        merch.p = strdup(input_str2);
        free(input_str2);

    } while (!ioopm_hash_table_has_key(db, merch));

    int quantity;
    do
    {
        quantity = ask_question_int("Quantity: \n");

    } while (quantity<0);

    ioopm_add_to_cart_db(db, storage, carts, cart_nr.i, merch, quantity);
}


void remove_from_cart(ioopm_hash_table_t *db, ioopm_hash_table_t *storage, ioopm_hash_table_t *carts)
{
    ioopm_print_carts_db(carts);
    elem_t cart_nr = int_elem(0);
    elem_t merch = ptr_elem(NULL);
    do
    {
        if (ioopm_hash_table_size(carts) == 0)
        {
            printf("\nThere are no carts to remove from!\n");
            return;
        }
        cart_nr.i = ask_question_int("\nWhat cart would you like to remove from?\n");

    } while (!ioopm_hash_table_has_key(carts, cart_nr));
    elem_t result;
    ioopm_hash_table_lookup(carts, cart_nr, &result);
    do
    {
        if (ioopm_hash_table_size(result.p) == 0)
        {
            printf("\nThere are no items to remove in this cart!\n");
            return;
        }
        char *input_str2 = ask_question_string("\nWhat item would you like to remove from your cart?\n");
        merch.p = strdup(input_str2);
        free(input_str2);

    } while (!ioopm_hash_table_has_key(result.p, merch));

    int quantity;
    do
    {
        quantity = ask_question_int("Quantity: \n");

    } while (quantity<0);

    ioopm_remove_from_cart_db(db, storage, carts, cart_nr.i, merch, quantity);
}

void calculate_cost(ioopm_hash_table_t *db, ioopm_hash_table_t *storage, ioopm_hash_table_t *carts)
{
    ioopm_print_carts_db(carts);
    elem_t cart_nr = int_elem(0);
    do
    {
        if (ioopm_hash_table_size(carts) == 0)
        {
            printf("\nThere are no carts to calculate!\n");
            return;
        }
        cart_nr.i = ask_question_int("\nWhich cart do you want to calculate the cost of?\n");

    } while (!ioopm_hash_table_has_key(carts, cart_nr));

    int sum = ioopm_calculate_cost_db(db, carts, cart_nr.i);
    printf("%d kr\n", sum);
}

void checkout(ioopm_hash_table_t *db, ioopm_hash_table_t *storage, ioopm_hash_table_t *carts)
{
    ioopm_print_carts_db(carts);
    elem_t cart_nr = int_elem(0);
    do
    {
        if (ioopm_hash_table_size(carts) == 0)
        {
            printf("\nThere are no carts to checkout!\n");
            return;
        }
        cart_nr.i = ask_question_int("\nWhat cart would you like to checkout?\n");
       

    } while (!ioopm_hash_table_has_key(carts, cart_nr));

    ioopm_checkout_db(db, storage, carts, cart_nr.i);
}

int quit(ioopm_hash_table_t *db, ioopm_hash_table_t *storage, ioopm_hash_table_t *carts, int *total_carts, ioopm_list_t *shelves)
{
    char *confirmation = NULL;
    do
    {
        char *input_str = ask_question_string("\ny/n\n");
        confirmation = strdup(input_str);

        free(input_str);
        
    } while (strcmp(confirmation, "y") != 0 && strcmp(confirmation, "n") != 0);
    if (strcmp(confirmation, "y") == 0)
    {
        free(confirmation);
        return ioopm_quit_db(db, storage, carts, total_carts, shelves);
    }
    else
    {
        free(confirmation);
        return 1;
    }
    
}

void event_loop(ioopm_hash_table_t *db, ioopm_hash_table_t *storage, ioopm_hash_table_t *carts, int *total_carts, ioopm_list_t *shelves)
{
    bool running = true;
    char *menu = ("\n[A] Add Merchandise\n"
                  "[L] List Merchandise\n"
                  "[D] Remove Merchandise\n"
                  "[E] Edit Merchandise\n"
                  "[S] Show Stock\n"
                  "[P] Replenish\n"
                  "[C] Create Cart\n"
                  "[R] Remove Cart\n"
                  "[+] Add To Cart\n"
                  "[-] Remove From Cart\n"
                  "[=] Calculate Cost\n"
                  "[O] Checkout\n"
                  "[Q] Quit\n");

    char action = ask_question_menu(menu);
    while (running)
    {
        switch (action)
        {
        case 'A':
            add_merch(db);
            action = ask_question_menu(menu);
            
            break;

        case 'L':
            list_merch(db);
            action = ask_question_menu(menu);
            break;

        case 'D':
            remove_merch(db, storage, carts);
            action = ask_question_menu(menu);
            break;

        case 'E':
            edit_merch(db, storage, carts);
            action = ask_question_menu(menu);
            break;

        case 'S':
            show(db);
            action = ask_question_menu(menu);
            break;

        case 'P':
            replenish_stock(db, storage, carts, shelves);
            action = ask_question_menu(menu);
            break;

        case 'C':
            create_cart(carts, total_carts);
            action = ask_question_menu(menu);
            break;

        case 'R':
            remove_cart(db, storage, carts);
            action = ask_question_menu(menu);
            break;

        case '+':
            add_to_cart(db, storage, carts);
            action = ask_question_menu(menu);
            break;

        case '-':
            remove_from_cart(db, storage, carts);
            action = ask_question_menu(menu);
            break;

        case '=':
            calculate_cost(db, storage, carts);
            action = ask_question_menu(menu);
            break;

        case 'O':
            checkout(db, storage, carts);
            action = ask_question_menu(menu);
            break;
        
        case 'Q':
            int confirmation = quit(db, storage, carts, total_carts, shelves);
            if (confirmation == 0)
            {
                running = false;
                break;
            }
            else
            {
                action = ask_question_menu(menu);
                break;
            }
        }
    }
}

int main()
{
    // HTn->S maps names of merch to its information
    ioopm_hash_table_t *db = ioopm_create_database_part();

    // HTsl->n maps storage location to the merch stored in that location as a char * or void *
    ioopm_hash_table_t *storage = ioopm_create_database_part();

    // create shopping cart linked list with hash_tables as values
    ioopm_hash_table_t *carts = ioopm_create_carts();

    // create a list fro all shelf inputs
    ioopm_list_t *shelves = ioopm_create_shelves();

    int *total_carts = calloc(1, sizeof(int));
    *total_carts = 0;
    

    event_loop(db, storage, carts, total_carts, shelves);

    return 0;
}