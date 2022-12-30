#pragma once

#include <stdlib.h>

typedef enum { False, True } BOOL;	// a boolean type

typedef struct node
{
	void* key;
	struct node* next;
}NODE;

// List
typedef struct
{
	NODE head;
}LIST;


/*** Function prototypes ***/

int L_init(LIST* pList);					// create new list
NODE* L_insert(NODE* pNode, void* Value);	// add new node after *pNode
BOOL L_delete(NODE* pNode, BOOL isFreeData, void(*freeData)(void*));				// erase node after *pNode
NODE* L_find(NODE* pNode, void* Value, int(*compare)(const void*, const void*));		// return a pointer to the node 
BOOL L_free(LIST* pList, BOOL isFreeData, void(*freeData)(void*));				// free list memory
int L_print(const LIST* pList, void(*printFunc)(void*));					// print the list content
int L_count(const LIST* pList);