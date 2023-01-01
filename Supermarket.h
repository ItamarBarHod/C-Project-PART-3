#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "Address.h"
#include "Customer.h"
#include "Product.h"
#include "Address.h"
#include "General.h"
#include "listGen.h"
#include "ShoppingCart.h"

typedef struct
{
	char* name;
	Address		location;
	Customer* customerArr;
	int			customerCount;
	LIST		products;
}SuperMarket;

int		initSuperMarket(SuperMarket* pMarket, FILE* nameAndProductsFile, FILE* customerFile);
void	printSuperMarket(const SuperMarket* pMarket);
int		addProduct(SuperMarket* pMarket);
int		addNewProduct(SuperMarket* pMarket, const char* barcode);
int		addCustomer(SuperMarket* pMarket);
int		isCustomerInMarket(SuperMarket* pMarket, Customer* pCust);
int		doShopping(SuperMarket* pMarket);
Customer* doPrintCart(SuperMarket* pMarket);
int		doPayment(SuperMarket* pMarket);
Customer* getCustomerShopPay(SuperMarket* pMarket);
void fillCart(SuperMarket* pMarket, ShoppingCart* pCart);

void	printProductByType(const SuperMarket* pMarket);
void	printAllProducts(const SuperMarket* pMarket);
void	printAllCustomers(const SuperMarket* pMarket);
Customer* initCustomerFromFile(SuperMarket* pMarket, FILE* file);


// int		getProductIndexByBarcode(SuperMarket* pMarket, const char* barcode);
Product* getProductByBarcode(SuperMarket* pMarket, const char* barcode);
Product* getProductFromUser(SuperMarket* pMarket, char* barcode);
// void	getUniquBarcode(char* barcode, SuperMarket* pMarket);
Customer* findCustomerByName(const SuperMarket* pMarket, const char* name);

Customer* getCustomerWhoShop(SuperMarket* pMarket);
Product* getProductAndCount(SuperMarket* pMarket, int* pCount);

void freeMarket(SuperMarket* pMarket);
int saveMarket(SuperMarket* pMarket, FILE* binMarketFile, FILE* customerFile);
void SortCustomersByAttribute(SuperMarket* pMarket);
void findCustomer(const SuperMarket* pMarket);
void writeCustomersToFile(const SuperMarket* pMarket, FILE* file);
int writeMarketToBinFile(SuperMarket* pMarket, FILE* file);
int writeProductsToBinFile(SuperMarket* pMarket, FILE* file);
int initMarketNameAndAddress(SuperMarket* pMarket, FILE* binMarketFile);
int readMarketNameAndAddressFromBinFile(SuperMarket* pMarket, FILE* file);
void initMarketCustomers(SuperMarket* pMarket, FILE* customerFile);
int readCustomersFromFile(SuperMarket* pMarket, FILE* file);
int readProductsFromBinFile(SuperMarket* pMarket, FILE* file);
int buildMarketListFromBinFile(SuperMarket* pMarket, FILE* file, size_t listSize);
void searchByName(const SuperMarket* pMarket);
void searchByNumbers(const SuperMarket* pMarket, const Customer* cust, int(*compareFunc)(const void*, const void*));

