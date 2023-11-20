#include "linked_list.h"
#include "common.h"


link_t *link_create(elem_t value, link_t *next)
{
    link_t *link = calloc(1, sizeof(link_t));
    link->value = value;
    link->next = next;
    return link;
}


ioopm_list_t *ioopm_linked_list_create(ioopm_eq_function eq)
{
    ioopm_list_t *list = calloc(1, sizeof(ioopm_list_t));
    list->func_p = eq;
    return list;
}

void ioopm_linked_list_destroy(ioopm_list_t *list)
{
    assert(list);
    ioopm_linked_list_clear(list);
    free(list);
}

void ioopm_linked_list_append(ioopm_list_t *list, elem_t value)
{
    assert(list);
    if (!list->head)
    {
        list->head = link_create(value, NULL);
    }
    else
    {
        link_t *curr = list->head;
        while (curr && curr->next)
        {
            curr = curr->next;
        }
        curr->next = link_create(value, NULL);
    }
    list->size+=1;
}

void ioopm_linked_list_prepend(ioopm_list_t *list, elem_t value)
{
    assert(list);
    list->head = link_create(value, list->head);
    list->size+=1;
}

void ioopm_linked_list_insert(ioopm_list_t *list, int32_t index, elem_t value)
{
    assert(list);
    assert(index <= list->size && index >= 0);

    if (list->size == 0)
    {
        list->head = link_create(value, NULL);
    }
    else
    {
        link_t *curr = list->head;
        for (int i = 0; i < index - 1; i++)
        {
            curr = curr->next;
        }
        link_t *new = link_create(value, curr->next);
        curr->next = new;
    }
    list->size+=1;
}

elem_t ioopm_linked_list_remove(ioopm_list_t *list, int32_t index)
{
    assert(list);
    assert(list->head);
    assert(index < list->size && index >= 0);
    if (index == 0)
    {
        link_t *tmp = list->head;
        elem_t value = tmp->value;
        list->head = tmp->next;
        free(tmp);
        list->size-=1;
        return value;
    }
    // index > 0
    assert(list->head);
    link_t *prev = list->head;
    link_t *current = prev->next;
    for (int i = 1; i < index; i++)
    {
        assert(current);
        prev = current;
        current = current->next;
    }
    prev->next = current->next;
    elem_t value = current->value;
    free(current);
    list->size-=1;
    return value;
}

elem_t ioopm_linked_list_get(ioopm_list_t *list, int32_t index)
{
    assert(list);

    link_t *curr = list->head;
    for (int i = 0; i < index; i++)
    {
        curr = curr->next;
    }
    if (curr)
    {
        return curr->value;
    }

    return ptr_elem(NULL);
}

bool ioopm_linked_list_contains(ioopm_list_t *list, elem_t element)
{
    assert(list);
    link_t *curr = list->head;

    while (curr)
    {
        if (list->func_p(curr->value, element))
        {
            return true;
        }
        curr = curr->next;
    }
    return false;
}

size_t ioopm_linked_list_size(ioopm_list_t *list)
{
    return list->size;
}

bool ioopm_linked_list_is_empty(ioopm_list_t *list)
{
    return list->size == 0;
}

void ioopm_linked_list_clear(ioopm_list_t *list)
{
    assert(list);
    link_t *curr = list->head;
    while (curr)
    {
        link_t *tmp = curr;
        curr = curr->next;
        free(tmp);
    }
    list->head = NULL;
    list->size = 0;
}

bool ioopm_linked_list_all(ioopm_list_t *list, ioopm_predicate prop, void *extra)
{
    assert(list);

    if (!list->head)
    {
        return false;
    }
    link_t *curr = list->head;
    while (curr)
    {
        if (!prop(curr->value, curr->value, extra, list->func_p))
        {
            return false;
        }
        curr = curr->next;
    }
    return true;
}

bool ioopm_linked_list_any(ioopm_list_t *list, ioopm_predicate prop, void *extra)
{
    assert(list);

    if (!list->head)
    {
        return false;
    }
    link_t *curr = list->head;
    while (curr)
    {
        if (prop(curr->value, curr->value, extra, list->func_p))
        {
            return true;
        }
        curr = curr->next;
    }
    return false;
}

void ioopm_linked_list_apply_to_all(ioopm_list_t *list, ioopm_apply_function fun, void *extra)
{
    assert(list);
    link_t *curr = list->head;
    elem_t *value = NULL;
    while (curr)
    {
        value = &curr->value;
        fun(curr->value, value, extra);
        curr = curr->next;
    }
}