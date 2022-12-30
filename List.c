#include "List.h"


//////////////////////////////////////////
// Init
// Aim:		create new list
// Input:	pointer to the list structure
// Output:	TRUE if succeeded
//////////////////////////////////////////
BOOL L_init(LIST* pList)
{
	if (pList == NULL) return False;	// no list to initialize

	pList->head.next = NULL;
	return True;
}


/////////////////////////////////////////////////////////////////
// Insert
// Aim:		add new node
// Input:	pointer to the node BEFORE the place for the new one
//			a value to be stored in the new node
// Output:	pointer to the new node
/////////////////////////////////////////////////////////////////
NODE* L_insert(NODE* pNode, void* Value)
{
	NODE* tmp;

	if (!pNode) return NULL;

	tmp = (NODE*)malloc(sizeof(NODE));	// new node

	if (tmp) {
		tmp->key = Value;
		tmp->next = pNode->next;
		pNode->next = tmp;
	}
	return tmp;
}


//////////////////////////////////////////////////////////////
// Delete
// Aim:		erase node
// Input:	pointer to the node BEFORE the node to be deleted 
// Output:	TRUE if succeeded
//////////////////////////////////////////////////////////////
BOOL L_delete(NODE* pNode, BOOL isFreeData, void(*freeData)(void*))
{
	NODE* tmp;

	if (!pNode || !(tmp = pNode->next)) return False;

	pNode->next = tmp->next;
	if (isFreeData)
	{
		freeData(tmp->key);
		free(tmp);
	}
	else
	{
		free(tmp);
	}
	return True;
}


/////////////////////////////////////////////////////////
// Find
// Aim:		search for a value
// Input:	pointer to the node to start with 
//			a value to be found
// Output:	pointer to the node containing the Value
/////////////////////////////////////////////////////////
NODE* L_find(NODE* pNode, void* Value, int(*compare)(const void*, const void*))
{
	NODE* temp = NULL;
	while (pNode != NULL)
	{
		if (compare(pNode->key, Value) == 0)
		{
			temp = pNode;
			break;
		}
		pNode = pNode->next;
	}
	return temp;
}


////////////////////////////////////////////////
// Free (additional function)
// Aim:		free the list memory
// Input:	pointer to the list structure
// Output:	TRUE if succeeded
////////////////////////////////////////////////
BOOL L_free(LIST* pList, BOOL isFreeData, void(*freeData)(void*))
{
	NODE* tmp;

	if (!pList) return False;

	for (tmp = &(pList->head); L_delete(tmp, isFreeData, freeData); );
	return True;
}


////////////////////////////////////////////////
// Print (additional function)
// Aim:		print the list content (assume the DATA is int)
// Input:	pointer to the list structure
// Output:	a number of the printed elements
////////////////////////////////////////////////
int L_print(const LIST* pList, void(*printFunc)(void*))
{
	NODE* tmp;
	int		c = 0;

	if (!pList) return 0;

	printf("\n");
	for (tmp = pList->head.next; tmp; tmp = tmp->next, c++)
		printFunc(tmp->key);
	printf("\n");
	return c;
}

int L_count(const LIST* pList)
{
	if (pList->head.next == NULL)
	{
		return 0;
	}
	int count = 0;
	NODE* tmp = pList->head.next;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	return count;
}