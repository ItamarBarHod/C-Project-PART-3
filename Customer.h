#pragma once
#include "ShoppingCart.h"

typedef enum {
	eName = 1, eShopTimes, eTotalSpend, eNofSortAttribute
} eSortAttribute;

typedef struct
{
	char* name;
	int shopTimes;
	float totalSpend;
	ShoppingCart* pCart;
}Customer;

int compareByName(const void* pC1, const void* pC2);
int compareByShopTimes(const void* pC1, const void* pC2);
int compareByTotalSpend(const void* pC1, const void* pC2);

static int (*sortAttributeArr[eNofSortAttribute])(const void*, const void*) = { compareByName, compareByShopTimes, compareByTotalSpend };
static int attributeIndex;

void writeCustomerToFile(Customer* pCustomer, FILE* file);
Customer* readCustomerFromFile(Customer* pCustomer, FILE* file);
int	initCustomer(Customer* pCustomer);
void printCustomer(const Customer* pCustomer);
int	isCustomer(const Customer* pCust, const char* name);
void pay(Customer* pCustomer);
void freeCustomer(Customer* pCust);
eSortAttribute getCustomerSortAttribute();
