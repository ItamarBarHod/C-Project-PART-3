#pragma once

#include <stdlib.h>
#include <stdio.h>

typedef enum { False, True } BOOL;	// a boolean type
typedef void* DATA;

typedef struct node
{
	DATA key;
	struct node* next;
}NODE;

// List
typedef struct
{
	NODE head;
}LIST;


/*** Function prototypes ***/

int L_init(LIST* pList);					// create new list
NODE* L_insert(NODE* pNode, DATA value);	// add new node after *pNode
NODE* L_insertSorted(NODE* pNode, DATA value, int(*compare)(const void*, const void*));
BOOL L_delete(NODE* pNode, void(*freeData)(void*));				// erase node after *pNode
NODE* L_find(NODE* pNode, DATA value, int(*compare)(const void*, const void*));		// return a pointer to the node 
BOOL L_free(LIST* pList, void(*freeData)(void*));				// free list memory
int L_print(const LIST* pList, void(*print)(const void*));					// print the list content