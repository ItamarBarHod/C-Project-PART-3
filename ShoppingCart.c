#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "General.h"
#include "ShoppingCart.h"



void	initCart(ShoppingCart* pCart)
{
	pCart->count = 0;
	pCart->itemArr = NULL;
}

float	getTotalPrice(const ShoppingCart* pCart)
{
	float price = 0;
	ShoppingItem* pItem;
	for (int i = 0; i < pCart->count; i++)
	{
		pItem = pCart->itemArr[i];
		price += (pItem->price * pItem->count);
	}
	return price;
}

int		addItemToCart(ShoppingCart* pCart, const char* barcode, float price, int count)
{
	ShoppingItem* pItem = getItemByBarocde(pCart, barcode);
	if (!pItem) //new item
	{
		pItem = createItem(barcode, price, count);
		if (!pItem)
			return 0;
		/*pCart->itemArr = (ShoppingItem**)realloc(pCart->itemArr, (pCart->count + 1) * sizeof(ShoppingItem*));
		if (!pCart->itemArr)
			return 0;
		original code.. bad realloc will delete array, also pItem is allocated but not free'd if realloc fails, instead:*/
		ShoppingItem** tempArr = (ShoppingItem**)realloc(pCart->itemArr, (pCart->count + 1) * sizeof(ShoppingItem*));
		if (!tempArr)
		{
			free(pItem);
			return 0;
		}

		tempArr[pCart->count] = pItem;
		// pCart->itemArr[pCart->count] = pItem;
		pCart->count++;
		pCart->itemArr = tempArr;
	}
	else {
		pItem->count += count;
	}
	return 1;
}

float	printShoppingCart(const ShoppingCart* pCart)
{
	ShoppingItem* pItem;
	float price = 0;
	for (int i = 0; i < pCart->count; i++)
	{
		pItem = pCart->itemArr[i];
		printItem(pItem);
		price += (pItem->price * pItem->count);
	}

	printf("Total bill to pay: %.2f\n", price);
	return price;
}



ShoppingItem* getItemByBarocde(ShoppingCart* pCart, const char* barcode)
{
	for (int i = 0; i < pCart->count; i++)
	{
		if (strcmp(pCart->itemArr[i]->barcode, barcode) == 0)
			return pCart->itemArr[i];
	}
	return NULL;
}

void	freeShoppingCart(ShoppingCart* pCart)
{
	generalArrayFunction(pCart->itemArr, pCart->count, sizeof(ShoppingItem*), freeShoppingItem);
	free(pCart->itemArr);
}