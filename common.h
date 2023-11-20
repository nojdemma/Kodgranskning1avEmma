/**
 * @file common.h
 * @author Maja Nyberg & (Fadi Zemzemi)
 * @date 6 november 2023
 * @brief A header file which contains the common declarations used in the files concerning
 * hash_table, linked_list, iterator, backend and frontend.
 */

#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <limits.h>
#include <assert.h>
#include <float.h>

#define No_Buckets 17

typedef union elem elem_t; 
#define int_elem(x) (elem_t) { .i=(x) }
#define ptr_elem(x) (elem_t) { .p=(x) }

union elem
{
    int i;
    unsigned int u;
    bool b;
    float f;
    void *p;
};

typedef bool(*ioopm_eq_function)(elem_t a, elem_t b);
typedef bool(*ioopm_predicate)(elem_t key, elem_t value, void *extra, void *compare_func);
typedef void(*ioopm_apply_function)(elem_t key, elem_t *value, void *extra);


typedef struct list ioopm_list_t;
typedef struct link link_t;
typedef struct iter ioopm_list_iterator_t;

struct list
{
    link_t *head;
    link_t *last;
    size_t size;
    ioopm_eq_function func_p;
};
struct link
{
    elem_t value;
    struct link *next;
};

struct iter 
{
    link_t *current;
    ioopm_list_t *list;
};

typedef struct merch merch_t;
typedef struct shelf shelf_t;
typedef struct cart cart_t;

struct shelf
{
    int quantity;
    char *shelf;
};

struct cart
{
    merch_t *item;
    int cart_id;
    int number_of_items;
};

struct merch // ska detta vara elem_t?
{
    elem_t name; // key
    char *desc;
    int price;
    ioopm_list_t *location;
};

#endif 


