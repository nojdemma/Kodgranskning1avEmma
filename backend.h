#pragma once
#ifndef BACKEND_H
#define BACKEND_H

#include "hash_table.h"
#include "common.h"
/**
 * @file backend.h
 * @author Maja Nyberg & (Fadi Zemzemi)
 * @date 6 november 2023
 * @brief A backend interface of a store
 */

/// @brief Add a merchandise entry to the database
/// @param db Database hash table
/// @param merch_n Name of the merchandise, key
/// @param desc Description of the merchandise
/// @param price Price of the merchandise
void ioopm_add_merch_to_db(ioopm_hash_table_t *db, elem_t merch_n, char *desc, int price);

/// @brief List merchandise entries in the database
/// @param db Database hash table
void ioopm_list_merch_db(ioopm_hash_table_t *db);

/// @brief Remove a merchandise entry from the database
/// @param db Database hash table
/// @param storage Storage hash table
/// @param carts Carts hash table
/// @param merch_n Key of the merchandise entry to remove
void ioopm_remove_merch_db(ioopm_hash_table_t *db, ioopm_hash_table_t *storage, ioopm_hash_table_t *carts, elem_t merch_n);

/// @brief Edit a merchandise entry in the database
/// @param db Database hash table
/// @param storage Storage hash table
/// @param carts Carts hash table
/// @param merch_n Key of the merchandise entry to remove
void ioopm_edit_merch_db(ioopm_hash_table_t *db, ioopm_hash_table_t *storage, ioopm_hash_table_t *carts, elem_t old_key, elem_t new_key, char *new_desc, int new_price);

/// @brief Show stock for a specific merchandise in the database
/// @param db Database hash table
/// @param merch_n Key of the merchandise entry to display stock for
void ioopm_show_stock_db(ioopm_hash_table_t *db, elem_t merch_n);

/// @brief Replenish stock in the database for a specific merchandise
/// @param db Database hash table
/// @param storage Storage hash table
/// @param merch_n Key of the merchandise entry
/// @param shelf Shelf of the merchandise
/// @param stock_increase Increase in stock quantity
void ioopm_replenish_db(ioopm_hash_table_t *db, ioopm_hash_table_t *storage, elem_t merch_n, shelf_t *shelf, int stock_increase);

/// @brief Create a cart in the cart database
/// @param carts Cart database hash table
/// @param total_carts Pointer to the total number of carts
void ioopm_create_cart_db(ioopm_hash_table_t *carts, int *total_carts);

/// @brief Remove a cart from the cart database
/// @param db Database hash table
/// @param storage Storage hash table
/// @param carts Cart database hash table
/// @param cart_nr Cart number to remove
void ioopm_remove_cart_db(ioopm_hash_table_t *db, ioopm_hash_table_t *storage, ioopm_hash_table_t *carts, int cart_nr);

/// @brief Add merchandise to a cart in the cart database
/// @param db Database hash table
/// @param storage Storage hash table
/// @param carts Cart database hash table
/// @param cart_nr Cart number to add merchandise to
/// @param merch_n Merchandise to add
/// @param quantity Quantity of merchandise to add
void ioopm_add_to_cart_db(ioopm_hash_table_t *db, ioopm_hash_table_t *storage, ioopm_hash_table_t *carts, int cart_nr, elem_t merch_n, int quantity);

/// @brief Remove merchandise from a cart in the cart database
/// @param db Database hash table
/// @param storage Storage hash table
/// @param carts Cart database hash table
/// @param cart_nr Cart number to remove merchandise from
/// @param merch_n Merchandise to remove
/// @param quantity Quantity of merchandise to remove
void ioopm_remove_from_cart_db(ioopm_hash_table_t *db, ioopm_hash_table_t *storage, ioopm_hash_table_t *carts, int cart_nr, elem_t merch_n, int quantity);

/// @brief Calculate the total cost of items in a cart in the cart database
/// @param db Database hash table
/// @param carts Cart database hash table
/// @param cart_nr Cart number to calculate the cost for
/// @return Total cost of items in the cart
int ioopm_calculate_cost_db(ioopm_hash_table_t *db, ioopm_hash_table_t *carts, int cart_nr);

/// @brief Checkout a cart in the cart database
/// @param db Database hash table
/// @param storage Storage hash table
/// @param carts Cart database hash table
/// @param cart_nr Cart number to checkout
void ioopm_checkout_db(ioopm_hash_table_t *db, ioopm_hash_table_t *storage, ioopm_hash_table_t *carts, int cart_nr);

/// @brief Quit and clean up resources in the database
/// @param db Database hash table
/// @param storage Storage hash table
/// @param carts Cart database hash table
/// @param total_carts Pointer to the total number of carts
/// @param ioopm_list_t pointer to all inut shelves collected during the program
/// @return 0 if quitting the database, otherwise 1
int ioopm_quit_db(ioopm_hash_table_t *db, ioopm_hash_table_t *storage, ioopm_hash_table_t *carts, int *total_carts, ioopm_list_t *shelves);

/// @brief Print the contents of the cart database
/// @param carts Cart database hash table
void ioopm_print_carts_db(ioopm_hash_table_t *carts);

/// @brief Create a cart database
/// @return A pointer to a newly created cart database
ioopm_hash_table_t *ioopm_create_carts();

/// @brief Create a database part
/// @return A pointer to a newly created database part
ioopm_hash_table_t *ioopm_create_database_part();

/// @brief Create a list containing all the input shelves for easy frees
/// @return a pointer the list
ioopm_list_t *ioopm_create_shelves();

#endif