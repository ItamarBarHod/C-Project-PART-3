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
	const Customer cust1 = *(const Customer*)pC1;
	const Customer cust2 = *(const Customer*)pC2;
	return strcmp(cust1.name, cust2.name);
}

int compareByShopTimes(const void* pC1, const void* pC2)
{
	const Customer cust1 = *(const Customer*)pC1;
	const Customer cust2 = *(const Customer*)pC2;
	if (cust1.shopTimes == cust2.shopTimes)
		return 0;
	else if (cust1.shopTimes > cust2.shopTimes)
		return 1;
	else
		return -1;
}

int compareByTotalSpend(const void* pC1, const void* pC2)
{
	const Customer cust1 = *(const Customer*)pC1;
	const Customer cust2 = *(const Customer*)pC2;
	if (cust1.totalSpend == cust2.totalSpend)
		return 0;
	else if (cust1.totalSpend > cust2.totalSpend)
		return 1;
	else
		return -1;
}


void	pay(Customer* pCustomer)
{
	if (!pCustomer->pCart)
		return;
	printf("!!! --- Payment was received!!!! --- \n");
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

eSortAttribute getCustomerSortAttribute()
{
	int option;
	printf("\n\n");
	printf("What would you like to sort by?\nName: 1\nShopTimes: 2\ntotalSpend: 3\n");
	do {
		scanf("%d", &option);
		if (option < 1 || option >= eNofSortAttribute)
		{
			printf("wrong option\n");
			printf("What would you like to sort by?\nName: 1\nShopTimes: 2\ntotalSpend: 3\n");
		}
	} while (option < 1 || option >= eNofSortAttribute);
	getchar();
	return (eSortAttribute)option;
}

void writeCustomerToFile(Customer* pCustomer, FILE* file)
{
	size_t uNameLength = strlen(pCustomer->name) + 1; // +1 for /0
	fprintf(file, "%zu %s\n%d %f\n", uNameLength, pCustomer->name, pCustomer->shopTimes, pCustomer->totalSpend);
}

Customer* readCustomerFromFile(Customer* pCustomer, FILE* file)
{
	fscanf(file, "%s\n%d %f\n", pCustomer->name, &pCustomer->shopTimes, &pCustomer->totalSpend);
	pCustomer->pCart = NULL;
	return pCustomer;
}
