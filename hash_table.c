#include "common.h"
#include "hash_table.h"

/**
 * @file hash_table.c
 * @author Maja Nyberg & Emma Nöjd
 * @date 9 oktober 2023
 * @brief A hash table that implements the function
 * declarations given in hash_table.h
 */


typedef struct entry entry_t;

static entry_t *entry_create(elem_t key, elem_t value, entry_t *first_entry);

struct entry {
    elem_t key;         // holds the key
    elem_t value;       // holds the value
    entry_t *next;      // points to the next entry (possibly NULL)
};

struct hash_table
{
    entry_t buckets[No_Buckets];        // number of buckets
    size_t size;                        
    ioopm_hash_function hash_function;
    ioopm_eq_function equal_key;
    ioopm_eq_function equal_value;
};

ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function hash_function, ioopm_eq_function eq_key, ioopm_eq_function eq_value)
{
    ioopm_hash_table_t *ht = calloc(1, sizeof(ioopm_hash_table_t));
    
    ht->hash_function = hash_function;
    ht->equal_key = eq_key;
    ht->equal_value = eq_value;
    
    return ht;
}

static entry_t *find_previous_entry_for_key(ioopm_hash_table_t *ht, entry_t *bucket, elem_t key) 
{
    entry_t *entry = bucket;
    while (entry->next && !ht->equal_key(entry->next->key, key))
    {
        entry = entry->next;
    }
    return entry;
}

static entry_t *entry_create(elem_t key, elem_t value, entry_t *next_entry) 
{
    entry_t *inserted_entry = calloc(1, sizeof(entry_t));

    // Error handling in case memory allocation fails
    if (inserted_entry == NULL)
    {
        return NULL;
    }

    inserted_entry->key = key;
    inserted_entry->value = value;
    inserted_entry->next = next_entry;

    return inserted_entry;
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value) 
{
    // Search for an existing entry for a key
    entry_t *previous = find_previous_entry_for_key(ht, &ht->buckets[ht->hash_function(key)], key);
    entry_t *entry = previous->next;

    // Check if the next entry should be updated or not
    if (entry != NULL && ht->equal_key(entry->key, key))
    {
        entry->value = value;
    }
    else
    {
        previous->next = entry_create(key, value, entry);
        ht->size += 1;
    }
}

bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key, elem_t *result) 
{
    entry_t *previous = find_previous_entry_for_key(ht, &ht->buckets[ht->hash_function(key)], key);
   
    if (previous->next)
    {
        // Update result
        *result = previous->next->value;
        return true;
    }
    else
    {
        return false;
    }
}

elem_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key) 
{
    // Find previous entry to the one we would like to remove
    entry_t *previous = find_previous_entry_for_key(ht, &ht->buckets[ht->hash_function(key)], key);
    
    // If previous entry cannot be found, do nothing
    if (!previous)
    {
        return ptr_elem(NULL);
    }
    else
    {
         entry_t *entry = previous->next;

        // Unlink the entry by updating the previous entry's next field so that it skips that entry 
        if (entry) // Middle or first
        {   
            elem_t value = entry->value;
            previous->next = entry->next;
            free(entry);
            ht->size -= 1;
            return value;
        }
        else // Entry doesn't exist
        {
            return ptr_elem(NULL);
        }
    }
}

size_t ioopm_hash_table_size(ioopm_hash_table_t *ht) 
{
    return ht->size;
}

bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht) 
{
    return ioopm_hash_table_size(ht) == 0;
}

void ioopm_hash_table_clear(ioopm_hash_table_t *ht) 
{
    for (int i = 0; i < No_Buckets; i++)
    {
        entry_t *dummy = &ht->buckets[i];
        entry_t *entry = dummy->next;
        while (entry != NULL)
        {
            entry_t *next = entry->next;
            free(entry);
            entry = next;
        }
        // After clearing the bucket, set its next pointer to NULL
        dummy->next = NULL;
    }
    // Reset the size to zero
    ht->size = 0;
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht) 
{     
    ioopm_hash_table_clear(ht);
    free(ht);
}

ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht)
{
    ioopm_list_t *keys_list = calloc(1, sizeof(ioopm_list_t));

    for (int i = 0; i < No_Buckets; i++)
    {
        entry_t *dummy = &ht->buckets[i];
        entry_t *entry = dummy->next;

        while (entry != NULL)
        {
            ioopm_linked_list_append(keys_list, entry->key);
            entry = entry->next;
        }
    }
    return keys_list;
}

ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht)
{
    ioopm_list_t *values_list = calloc(1, sizeof(ioopm_list_t));

    for (int i = 0; i < No_Buckets; i++)
    {
        entry_t *dummy = &ht->buckets[i];
        entry_t *entry = dummy->next;

        while (entry != NULL)
        {
            ioopm_linked_list_append(values_list, entry->value);
            
            entry = entry->next;
        }
    }
    return values_list;
}

static bool has_value(elem_t key, elem_t value, void *x, void *eq) 
{
    elem_t *lookup_value_ptr = x;
    elem_t lookup_value = *lookup_value_ptr;

    ioopm_eq_function *eq_func_ptr = eq;
    ioopm_eq_function eq_func = *eq_func_ptr;

    return eq_func(lookup_value, value);
}

bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key)
{
    entry_t *previous = find_previous_entry_for_key(ht, &ht->buckets[ht->hash_function(key)], key);
    if (previous->next != NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value)
{
    return ioopm_hash_table_any(ht, has_value, &value);
}

bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg) 
{
    for (int i = 0; i < No_Buckets; i++)
    {
        entry_t *dummy = &ht->buckets[i];
        entry_t *entry = dummy->next;   

        while(entry != NULL)
        {
            elem_t key = entry->key;
            elem_t value = entry->value;

            if (!pred(key, value, arg, &ht->equal_key))
            {
                return false;
            }
            entry = entry->next;
        }   
    }
    return true;
}

bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg) 
{
    for (int i = 0; i < No_Buckets; i++)
    {
        entry_t *dummy = &ht->buckets[i];
        entry_t *entry = dummy->next;
        
        while(entry != NULL)
        {
            elem_t key = entry->key;
            elem_t value = entry->value;

            if (pred(key, value, arg, &ht->equal_key))
            {
                return true;
            }
            entry = entry->next;
        }
    }
    
    return false;
}

void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function apply_fun, void *arg) 
{
    for (int i = 0; i < No_Buckets; i++)
    {
        entry_t *dummy = &ht->buckets[i];
        entry_t *entry = dummy->next;
        
        while(entry != NULL)
        {
            elem_t key = entry->key;
            elem_t *value = &entry->value;

            apply_fun(key, value, arg);

            entry = entry->next;
        }
    } 
}