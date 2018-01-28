#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

static struct list_element * new_list_element(void *element, struct list_element *before)
{
	struct list_element *l_element = malloc(sizeof(struct list_element));

	if(l_element == NULL)
		return l_element;

	l_element->element = element;
	l_element->next = NULL;
	l_element->before = before;

	return l_element;
}

static struct list_element * delete_list_element(struct list_element *l_element)
{
	struct list_element *temp = l_element->next;
	free(l_element);

	return temp;
}

list_t *new_list()
{
	list_t *lista = malloc(sizeof(list_t));

	if(lista == NULL)
		return lista;

	lista->front = NULL;
	lista->back = NULL;

	return lista;
}


void delete_list(list_t *ll)
{
	struct list_element *temp = ll->front;

	while(temp != NULL)
	{
		temp = delete_list_element(temp);
	}

	free(ll);
}

void push_back_list(list_t *ll, void *element)
{
  struct list_element *temp = new_list_element(element,ll->back);

  if(ll->front == NULL)
     ll->front = temp;

  if(ll->back != NULL)
    ll->back->next = temp;

  ll->back = temp;
}

void pop_front_list(list_t *ll)
{
	int one_element = 0;

	if(ll->front == NULL)
		return;

	if(ll->front == ll->back)
		one_element = 1;

	struct list_element *temp = delete_list_element(ll->front);

	if(temp != NULL)
		temp->before = NULL;

	ll->front = temp;

	if(one_element)
		ll->back = NULL;
}
