#include "common.h"
#include <CUnit/Basic.h>
#include "backend.h"


/**
 * @file backend_tests.c
 * @author Maja Nyberg
 * @date 6 november 2023
 * @brief A test file for testing a the backend use of a database
 */

int init_suite(void)
{
    // Change this function if you want to do something *before* you
    // run a test suite
    return 0;
}

int clean_suite(void)
{
    // Change this function if you want to do something *after* you
    // run a test suite
    return 0;
}

void test_create_database()
{
    ioopm_hash_table_t *db = ioopm_create_database_part();
    CU_ASSERT_PTR_NOT_NULL(db);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(db), 0);

    ioopm_hash_table_destroy(db);
}

void test_create_carts()
{
    ioopm_hash_table_t *carts = ioopm_create_carts();
    CU_ASSERT_PTR_NOT_NULL(carts);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(carts), 0);

    ioopm_hash_table_destroy(carts);
}

void test_add_remove_merch() // inga allokeringsfel
{
    ioopm_hash_table_t *db = ioopm_create_database_part();
    ioopm_hash_table_t *storage = ioopm_create_database_part();
    ioopm_hash_table_t *carts = ioopm_create_carts();

    elem_t name = ptr_elem("Tooth brush");
    char *desc = strdup("Soft");
    int price = 10;
    ioopm_add_merch_to_db(db, name, desc, price);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(db), 1);

    ioopm_remove_merch_db(db, storage, carts, name);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(db), 0);
    free(desc);
    ioopm_hash_table_destroy(db);
    ioopm_hash_table_destroy(carts);
    ioopm_hash_table_destroy(storage);
}

void test_add_remove_merch_several()
{
    ioopm_hash_table_t *db = ioopm_create_database_part();
    ioopm_hash_table_t *storage = ioopm_create_database_part();
    ioopm_hash_table_t *carts = ioopm_create_carts();

    elem_t name = ptr_elem("Tooth brush");
    char *desc = "Soft";
    int price = 10;
    ioopm_add_merch_to_db(db, name, desc, price);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(db), 1);

    elem_t name2 = ptr_elem("Tooth paste");
    char *desc2 = "Mint";
    int price2 = 10;
    ioopm_add_merch_to_db(db, name2, desc2, price2);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(db), 2);

    ioopm_remove_merch_db(db, storage, carts, name);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(db), 1);

    ioopm_remove_merch_db(db, storage, carts, name2);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(db), 0);

    ioopm_hash_table_destroy(db);
    ioopm_hash_table_destroy(carts);
    ioopm_hash_table_destroy(storage);
}

void test_remove_merch_cart()
{
    ioopm_hash_table_t *db = ioopm_create_database_part();
    ioopm_hash_table_t *storage = ioopm_create_database_part();
    ioopm_hash_table_t *carts = ioopm_create_carts();

    elem_t name = ptr_elem("Tooth brush");
    char *desc = "Soft";
    int price = 10;
    ioopm_add_merch_to_db(db, name, desc, price);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(db), 1);

    int *total_carts = calloc(1, sizeof(int));
    *total_carts = 0;
    shelf_t *test_shelf = calloc(1, sizeof(shelf_t));
    test_shelf->shelf = "A25";

    ioopm_replenish_db(db, storage, name, test_shelf, 5);
    ioopm_create_cart_db(carts, total_carts);
    ioopm_add_to_cart_db(db, storage, carts, *total_carts, name, 3);
    ioopm_remove_merch_db(db, storage, carts, name);
    ioopm_print_carts_db(carts);
    ioopm_calculate_cost_db(db, carts, *total_carts);

    ioopm_remove_cart_db(db, storage, carts, *total_carts);
    ioopm_hash_table_destroy(db);
    ioopm_hash_table_destroy(carts);
    ioopm_hash_table_destroy(storage);

    free(total_carts);


}

void test_add_to_cart_from_several_shelves()
{
    ioopm_hash_table_t *db = ioopm_create_database_part();
    ioopm_hash_table_t *storage = ioopm_create_database_part();
    ioopm_hash_table_t *carts = ioopm_create_carts();

    elem_t name = ptr_elem("Tooth brush");
    char *desc = "Soft";
    int price = 10;
    ioopm_add_merch_to_db(db, name, desc, price);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(db), 1);

    int *total_carts = calloc(1, sizeof(int));
    *total_carts = 0;
    shelf_t *test_shelf = calloc(1, sizeof(shelf_t));
    test_shelf->shelf = "A25";
    shelf_t *test_shelf2 = calloc(1, sizeof(shelf_t));
    test_shelf2->shelf = "C25";
    shelf_t *test_shelf3 = calloc(1, sizeof(shelf_t));
    test_shelf3->shelf = "C26";

    ioopm_replenish_db(db, storage, name, test_shelf, 5);
    ioopm_replenish_db(db, storage, name, test_shelf2, 6);
    ioopm_replenish_db(db, storage, name, test_shelf3, 1);
    ioopm_create_cart_db(carts, total_carts);
    ioopm_add_to_cart_db(db, storage, carts, *total_carts, name, 12);
    ioopm_print_carts_db(carts);
    ioopm_show_stock_db(db, name);
    //ioopm_calculate_cost_db(db, carts, *total_carts);

    ioopm_remove_cart_db(db, storage, carts, *total_carts);
    ioopm_remove_merch_db(db, storage, carts, name);
    ioopm_hash_table_destroy(db);
    ioopm_hash_table_destroy(carts);
    ioopm_hash_table_destroy(storage);
    free(test_shelf3);
    free(test_shelf);
    free(test_shelf2);
    free(total_carts);
}
void test_show_stock()
{
    ioopm_hash_table_t *db = ioopm_create_database_part(); 
    ioopm_hash_table_t *storage = ioopm_create_database_part();
    ioopm_hash_table_t *carts = ioopm_create_carts();

    elem_t name = ptr_elem("Tooth brush");
    char *desc = "Soft";
    int price = 10;
    ioopm_add_merch_to_db(db, name, desc, price);

    ioopm_show_stock_db(db, name);

    shelf_t *test_shelf = calloc(1, sizeof(shelf_t));
    test_shelf->shelf = "A25";
    shelf_t *test_shelf2 = calloc(1, sizeof(shelf_t));
    test_shelf2->shelf = "H89";
    ioopm_replenish_db(db, storage, name, test_shelf, 3);
    ioopm_replenish_db(db, storage, name, test_shelf2, 9);

    ioopm_show_stock_db(db, name);

    ioopm_remove_merch_db(db, storage, carts, name);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(db), 0);
    ioopm_hash_table_destroy(storage);
    ioopm_hash_table_destroy(carts);
    ioopm_hash_table_destroy(db);
}

void test_replenish()
{
    ioopm_hash_table_t *db = ioopm_create_database_part();
    ioopm_hash_table_t *storage = ioopm_create_database_part();
    ioopm_hash_table_t *carts = ioopm_create_carts();

    elem_t result;
    elem_t name = ptr_elem("Tooth brush");
    char *desc = "Soft";
    int price = 10;
    ioopm_add_merch_to_db(db, name, desc, price);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(db), 1);
    shelf_t *test_shelf = calloc(1, sizeof(shelf_t));
    test_shelf->shelf = "A25";
    ioopm_replenish_db(db, storage, name, test_shelf, 3);

    ioopm_hash_table_lookup(db, name, &result);
    merch_t *m = (merch_t *) result.p;
    CU_ASSERT_FALSE(ioopm_linked_list_is_empty(m->location));
    CU_ASSERT_TRUE(ioopm_linked_list_contains(m->location, ptr_elem(test_shelf)));
    CU_ASSERT_TRUE(ioopm_hash_table_has_key(storage, ptr_elem("A25")));

    ioopm_remove_merch_db(db, storage, carts, name);
    ioopm_hash_table_destroy(db);
    ioopm_hash_table_destroy(carts);
    ioopm_hash_table_destroy(storage);
    
}

void test_create_and_remove_cart() // TODO: addera remove_cart läcker minne tills dess
{
    ioopm_hash_table_t *db = ioopm_create_database_part();
    ioopm_hash_table_t *storage = ioopm_create_database_part();
    ioopm_hash_table_t *carts = ioopm_create_carts();

    int *total_carts = calloc(1, sizeof(int));
    *total_carts = 0;
    ioopm_create_cart_db(carts, total_carts);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(carts), 1);

    ioopm_remove_cart_db(db, storage, carts, *total_carts);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(carts), 0);

    free(total_carts);
    ioopm_hash_table_destroy(carts);
    ioopm_hash_table_destroy(storage);
    ioopm_hash_table_destroy(db);
}

void test_add_to_cart()
{
    ioopm_hash_table_t *db = ioopm_create_database_part();
    ioopm_hash_table_t *storage = ioopm_create_database_part();
    ioopm_hash_table_t *carts = ioopm_create_carts();

    int *total_carts = calloc(1, sizeof(int));
    *total_carts = 0;
    ioopm_create_cart_db(carts, total_carts);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(carts), 1);

    elem_t name = ptr_elem("Tooth brush");
    char *desc = "Soft";
    int price = 10;
    ioopm_add_merch_to_db(db, name, desc, price);

    shelf_t *test_shelf = calloc(1, sizeof(shelf_t));
    test_shelf->shelf = "A25";
    elem_t result;
    ioopm_replenish_db(db, storage, name, test_shelf, 5);

    ioopm_add_to_cart_db(db, storage, carts, *total_carts, name, 4); // should work
    ioopm_print_carts_db(carts);
    ioopm_hash_table_lookup(carts, int_elem(*total_carts), &result);
    ioopm_hash_table_t *single_cart = (ioopm_hash_table_t *) result.p;
    CU_ASSERT_EQUAL(ioopm_hash_table_size(single_cart), 1);

    ioopm_add_to_cart_db(db, storage, carts, *total_carts, name, 3); // should not be right until changed quantity
    ioopm_print_carts_db(carts);

    ioopm_remove_merch_db(db, storage, carts, name);

    ioopm_remove_cart_db(db, storage, carts, *total_carts);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(carts), 0);

    free(test_shelf);
    free(total_carts);
    ioopm_hash_table_destroy(carts);
    ioopm_hash_table_destroy(storage);
    ioopm_hash_table_destroy(db);
    
}

void test_remove_from_cart()
{
    ioopm_hash_table_t *db = ioopm_create_database_part();
    ioopm_hash_table_t *storage = ioopm_create_database_part();
    ioopm_hash_table_t *carts = ioopm_create_carts();

    int *total_carts = calloc(1, sizeof(int));
    *total_carts = 0;
    ioopm_create_cart_db(carts, total_carts);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(carts), 1);

    elem_t name = ptr_elem("Tooth brush");
    char *desc = "Soft";
    int price = 10;
    ioopm_add_merch_to_db(db, name, desc, price);

    shelf_t *test_shelf = calloc(1, sizeof(shelf_t));
    test_shelf->shelf = "A25";
    elem_t result;
    ioopm_replenish_db(db, storage, name, test_shelf, 5);

    ioopm_add_to_cart_db(db, storage, carts, *total_carts, name, 4); // should work
    ioopm_print_carts_db(carts);
    ioopm_hash_table_lookup(carts, int_elem(*total_carts), &result);
    ioopm_hash_table_t *single_cart = (ioopm_hash_table_t *) result.p;
    CU_ASSERT_EQUAL(ioopm_hash_table_size(single_cart), 1);

    ioopm_print_carts_db(carts);

    ioopm_remove_from_cart_db(db, storage, carts, *total_carts, name, 3);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(single_cart), 1);
    ioopm_remove_from_cart_db(db, storage, carts, *total_carts, name, 1);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(single_cart), 0);
    ioopm_remove_cart_db(db, storage, carts, *total_carts);

    ioopm_remove_merch_db(db, storage, carts, name);
    free(total_carts);
    ioopm_hash_table_destroy(carts);
    ioopm_hash_table_destroy(storage);
    ioopm_hash_table_destroy(db);
}

void test_calculate_cost()
{
    ioopm_hash_table_t *db = ioopm_create_database_part();
    ioopm_hash_table_t *storage = ioopm_create_database_part();
    ioopm_hash_table_t *carts = ioopm_create_carts();

    int *total_carts = calloc(1, sizeof(int));
    *total_carts = 0;
    ioopm_create_cart_db(carts, total_carts);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(carts), 1);

    // Add 1
    elem_t name = ptr_elem("Tooth brush");
    char *desc = "Soft";
    int price = 10;
    ioopm_add_merch_to_db(db, name, desc, price);

    shelf_t *test_shelf = calloc(1, sizeof(shelf_t));
    test_shelf->shelf = "A25";
    ioopm_replenish_db(db, storage, name, test_shelf, 5);
    
    // Add two
    elem_t name2 = ptr_elem("iPhone");
    char *desc2 = "X";
    int price2 = 6000;
    ioopm_add_merch_to_db(db, name2, desc2, price2);

    shelf_t *test_shelf2 = calloc(1, sizeof(shelf_t));
    test_shelf2->shelf = "B78";
    ioopm_replenish_db(db, storage, name2, test_shelf2, 9);

    ioopm_add_to_cart_db(db, storage, carts, *total_carts, name, 4);
    ioopm_add_to_cart_db(db, storage, carts, *total_carts, name2, 1);

    ioopm_print_carts_db(carts); 
    CU_ASSERT_EQUAL(ioopm_calculate_cost_db(db, carts, *total_carts), 6040);

    ioopm_remove_cart_db(db, storage, carts, *total_carts);

    ioopm_remove_merch_db(db, storage, carts, name);
    ioopm_remove_merch_db(db, storage, carts, name2);
    free(total_carts);
    ioopm_hash_table_destroy(carts);
    ioopm_hash_table_destroy(storage);
    ioopm_hash_table_destroy(db);
}

void test_checkout()
{
    ioopm_hash_table_t *db = ioopm_create_database_part();
    ioopm_hash_table_t *storage = ioopm_create_database_part();
    ioopm_hash_table_t *carts = ioopm_create_carts();

    int *total_carts = calloc(1, sizeof(int));
    *total_carts = 0;
    ioopm_create_cart_db(carts, total_carts);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(carts), 1);

    elem_t name = ptr_elem("Tooth brush");
    char *desc = "Soft";
    int price = 10;
    ioopm_add_merch_to_db(db, name, desc, price);

    shelf_t *test_shelf = calloc(1, sizeof(shelf_t));
    test_shelf->shelf = "A25";
    ioopm_replenish_db(db, storage, name, test_shelf, 5);

    ioopm_add_to_cart_db(db, storage, carts, *total_carts, name, 4); // should work
    ioopm_add_to_cart_db(db, storage, carts, *total_carts, name, 1); // should not be right until changed quantity

    ioopm_checkout_db(db, storage, carts, *total_carts);

    ioopm_remove_merch_db(db, storage, carts, name);
    free(test_shelf);
    free(total_carts);
    ioopm_hash_table_destroy(carts);
    ioopm_hash_table_destroy(storage);
    ioopm_hash_table_destroy(db);

}

// void test_quit()
// {
//     ioopm_hash_table_t *db = ioopm_create_database_part();
//     ioopm_hash_table_t *storage = ioopm_create_database_part();
//     ioopm_hash_table_t *carts = ioopm_create_carts();

//     int *total_carts = calloc(1, sizeof(int));
//     *total_carts = 0;
//     ioopm_create_cart_db(carts, total_carts);
//     CU_ASSERT_EQUAL(ioopm_hash_table_size(carts), 1);

//     elem_t name = ptr_elem("Tooth brush");
//     char *desc = "Soft";
//     int price = 10;
//     ioopm_add_merch_to_db(db, name, desc, price);

//     shelf_t *test_shelf = calloc(1, sizeof(shelf_t));
//     test_shelf->shelf = "A25";
//     ioopm_replenish_db(db, storage, name, test_shelf, 5);

//     ioopm_add_to_cart_db(db, storage, carts, *total_carts, name, 4); // should work
//     ioopm_add_to_cart_db(db, storage, carts, *total_carts, name, 1); // should not be right until changed quantity

//     free(test_shelf);
//     ioopm_quit_db(db, storage, carts, total_carts);
// }
int main()
{
    // First we try to set up CUnit, and exit if we fail
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();

    // We then create an empty test suite and specify the name and
    // the init and cleanup functions
    CU_pSuite my_test_suite = CU_add_suite("My awesome test suite", init_suite, clean_suite);
    if (my_test_suite == NULL)
    {
        // If the test suite could not be added, tear down CUnit and exit
        CU_cleanup_registry();
        return CU_get_error();
    }

    // This is where we add the test functions to our test suite.
    // For each call to CU_add_test we specify the test suite, the
    // name or description of the test, and the function that runs
    // the test in question. If you want to add another test, just
    // copy a line below and change the information
    if (
        //(CU_add_test(my_test_suite, "Create database", test_create_database) == NULL) ||
        //(CU_add_test(my_test_suite, "Create carts", test_create_carts) == NULL) ||
        //(CU_add_test(my_test_suite, "Add and remove merch", test_add_remove_merch) == NULL) ||
       //(CU_add_test(my_test_suite, "Add and remove merch several", test_add_remove_merch_several) == NULL) ||
        (CU_add_test(my_test_suite, "Test remove merch cart", test_remove_merch_cart) == NULL) ||
        (CU_add_test(my_test_suite, "Test add to cart from several shelves", test_add_to_cart_from_several_shelves) == NULL) ||
        //(CU_add_test(my_test_suite, "Replenish", test_replenish) == NULL) ||
        //(CU_add_test(my_test_suite, "Show stock", test_show_stock) == NULL) ||
        //(CU_add_test(my_test_suite, "Create and remove cart", test_create_and_remove_cart) == NULL) ||
        //(CU_add_test(my_test_suite, "Add to cart", test_add_to_cart) == NULL) ||
        //(CU_add_test(my_test_suite, "Remove from cart", test_remove_from_cart) == NULL) ||
        //(CU_add_test(my_test_suite, "Calculate cost", test_calculate_cost) == NULL) ||
        //(CU_add_test(my_test_suite, "Checkout", test_checkout) == NULL) ||
        //(CU_add_test(my_test_suite, "Quit", test_quit) == NULL) ||
        0
    )
    {
        // If adding any of the tests fails, we tear down CUnit and exit
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Set the running mode. Use CU_BRM_VERBOSE for maximum output.
    // Use CU_BRM_NORMAL to only print errors and a summary
    CU_basic_set_mode(CU_BRM_VERBOSE);

    // This is where the tests are actually run!
    CU_basic_run_tests();

    // Tear down CUnit before exiting
    CU_cleanup_registry();
    return CU_get_error();
} 