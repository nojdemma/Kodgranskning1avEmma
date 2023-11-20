#include "iterator.h"
#include "common.h"

ioopm_list_iterator_t *ioopm_list_iterator(ioopm_list_t *list)
{
	ioopm_list_iterator_t *result = calloc(1, sizeof(ioopm_list_iterator_t));
	result->current = list->head;
	result->list = list;
	return result;
}

bool ioopm_iterator_has_next(ioopm_list_iterator_t *iter)
{
	return iter->current->next != NULL;
}

elem_t ioopm_iterator_next(ioopm_list_iterator_t *iter)
{
	iter->current = iter->current->next;
	return iter->current->value;
}

void ioopm_iterator_insert(ioopm_list_iterator_t *iter, elem_t element)
{
	link_t *current = iter->current;
	link_t *new = calloc(1, sizeof(link_t));
	new->value = current->value;
	new->next = current->next;
	current->value = element;
	current->next = new;

	if (current == iter->list->head)
	{
		iter->list->head = iter->current;
	}
	iter->list->size++;
}

void ioopm_iterator_reset(ioopm_list_iterator_t *iter)
{
	iter->current = iter->list->head;
}

elem_t ioopm_iterator_current(ioopm_list_iterator_t *iter)
{
	return iter->current->value;
}

void ioopm_iterator_destroy(ioopm_list_iterator_t *iter)
{
	free(iter);
}