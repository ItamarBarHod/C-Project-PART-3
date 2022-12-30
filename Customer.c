#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Customer.h"
#include "General.h"

int	initCustomer(Customer* pCustomer)
{

	do {
		pCustomer->name = getStrExactLength("Enter customer name\n");
		if (!pCustomer->name)
			return 0;
	} while (checkEmptyString(pCustomer->name));

	pCustomer->pCart = NULL;
	pCustomer->shopTimes = 0;
	pCustomer->totalSpend = 0;
	return 1;
}

void	printCustomer(const Customer* pCustomer)
{
	printf("Name: %s, ShopTimes: %d, totalSpend: %.2f\t", pCustomer->name, pCustomer->shopTimes, pCustomer->totalSpend);

	if (pCustomer->pCart == NULL)
		printf("Shopping cart is empty!\n");
	else
	{
		printf("Doing shopping now!!!\n");
		//printShoppingCart(pCustomer->pCart);
	}
}

int compareByName(const void* pC1, const void* pC2)
{
	Customer* cust1 = (Customer*)pC1;
	Customer* cust2 = (Customer*)pC2;
	return (strcmp(cust1->name, cust2->name));
}

int compareByShopTimes(const void* pC1, const void* pC2)
{
	Customer* cust1 = (Customer*)pC1;
	Customer* cust2 = (Customer*)pC2;
	return cust1->shopTimes - cust2->shopTimes;
}

int compareByTotalSpend(const void* pC1, const void* pC2)
{
	Customer* cust1 = (Customer*)pC1;
	Customer* cust2 = (Customer*)pC2;
	if (cust1->totalSpend > cust2->totalSpend)
	{
		return 1;
	}
	else if (cust1->totalSpend < cust2->totalSpend)
	{
		return -1;
	}
	return 0;
}


void	pay(Customer* pCustomer)
{
	if (!pCustomer->pCart)
		return;
	printf("---------- Cart info and bill for %s ----------\n", pCustomer->name);
	pCustomer->totalSpend += printShoppingCart(pCustomer->pCart); // increase totalspend
	pCustomer->shopTimes++; // increase shoptimes
	printf("!!! --- Payment was recived!!!! --- \n");
	freeShoppingCart(pCustomer->pCart);
	free(pCustomer->pCart);
	pCustomer->pCart = NULL;
}




int isCustomer(const Customer* pCust, const char* name)
{
	if (strcmp(pCust->name, name) == 0)
		return 1;
	return 0;
}

void	freeCustomer(Customer* pCust)
{
	if (pCust->pCart)
		pay(pCust); //will free every thing
	free(pCust->name);
}

eCustomerAttribute getCustomerAttribute()
{
	int option;
	printf("\n\n");
	printf("What would you like to sort by?\nName: 0\nShopTimes: 1\ntotalSpend: 2\n");
	do {
		scanf("%d", &option);
		if (option < 0 || option >= eNofCustomerAttribute)
		{
			printf("wrong option\n");
			printf("What would you like to sort by?\nName: 0\nShopTimes: 1\ntotalSpend: 2\n");
		}
	} while (option < 0 || option >= eNofCustomerAttribute);
	getchar();
	return (eCustomerAttribute)option;
}

void writeCustomerToFile(Customer* pCustomer, FILE* file)
{
	fprintf(file, "%d %s\n%d %f\n", (strlen(pCustomer->name) + 1), pCustomer->name, pCustomer->shopTimes, pCustomer->totalSpend);
}

void readCustomerFromFile(Customer* pCustomer, FILE* file)
{
	fscanf(file, "%s\n%d %f\n", pCustomer->name, &pCustomer->shopTimes, &pCustomer->totalSpend);
	pCustomer->pCart = NULL;
}