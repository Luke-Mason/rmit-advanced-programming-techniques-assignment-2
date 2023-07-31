/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2016 Assignment #2
 * Full Name        : Luke Mason
 * Student Number   : s3630120
 * Course Code      : 1076
 * Program Code     : BP096
 * Start up code provided by Paul Miller
 * Some codes are adopted here with permission by an anonymous author
 ***********************************************************************/
#include "ppd_stock.h"
#include <stdlib.h>
 /**
  * @file ppd_stock.c this is the file where you will implement the 
  * interface functions for managing the stock list.
  **/
struct ppd_list * init_item_list()
{
	struct ppd_list * itemList;
	itemList = malloc(sizeof(struct ppd_list)); /* allocate memory to store the struct(Malloc #1)*/
	itemList->head = NULL;
	if(itemList == NULL)
	{
		printf("Cannot create memory space for an item list\n");
		return NULL;
	}
	return itemList;
}
struct ppd_stock *  addToList(struct ppd_list * list)
{
	struct ppd_stock * data;
	struct ppd_node * node;
	struct ppd_node * current;
	current = list->head;
	if(current != NULL)
	{
		while(current->next != NULL)
		{
			current = current->next;
		}
	}
	node = malloc(sizeof(struct ppd_node));  /* allocate memory to store the struct*/
	if(node == NULL)
	{
		printf("Cannot create memory space for the item\n");
		return NULL;
	}
	data = malloc(sizeof(struct ppd_stock)); /* allocate memory to store the struct*/
	if(data == NULL)
	{
		free(node);
		printf("Cannot create memory space for the item's data\n");
		return NULL;
	}
	if(list->head == NULL)
	{
		list->head = node;
	}
	else
	{
		current->next = node;
	}
	node->data = data;
	node->next = NULL;
	list->count++;
	return data;
}
BOOLEAN findId(struct ppd_list * list, char * id)
{
	struct ppd_node * current;
	current = list->head;
	if(current != NULL)
	{
		while(current->next != NULL)
		{
			if(strcmp(id, current->data->id)==0)
			{
				return FALSE;
			}
			current = current->next;
		}
	}
	return TRUE;
}