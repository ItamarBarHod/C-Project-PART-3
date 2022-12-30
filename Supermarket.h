#pragma once
#include "Address.h"
#include "Customer.h"
#include "Product.h"
#include "List.h"
#include "ShoppingCart.h"

static BOOL isSorted = False;

typedef struct
{
	char* name;
	Address		location;
	Customer* customerArr;
	int			customerCount;
	LIST		Products;
}SuperMarket;


int		initSuperMarket(SuperMarket* pMarket);
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

void	printProductByType(SuperMarket* pMarket);
void	printAllProducts(const SuperMarket* pMarket);
void	printAllCustomers(const SuperMarket* pMarket);


// int		getProductIndexByBarcode(SuperMarket* pMarket, const char* barcode);
Product* getProductByBarcode(SuperMarket* pMarket, const char* barcode);
Product* getProductFromUser(SuperMarket* pMarket, char* barcode);
// void	getUniquBarcode(char* barcode, SuperMarket* pMarket);
Customer* FindCustomerByName(SuperMarket* pMarket, const char* name);

Customer* getCustomerWhoShop(SuperMarket* pMarket);
Product* getProductAndCount(SuperMarket* pMarket, int* pCount);

void	freeMarket(SuperMarket* pMarket);
void SortCustomersByAttribute(SuperMarket* pMarket);
void findCustomer(const SuperMarket* pMarket);
void writeCustomersToFile(const SuperMarket* pMarket, FILE* file);
void writeMarketAndProductsToFile(const SuperMarket* pMarket, FILE* file);
void readMarketAndProductsFromFile(SuperMarket* pMarket, FILE* file);
void readCustomersFromFile(SuperMarket* pMarket, FILE* file);
int readProductsFromFile(SuperMarket* pMarket, FILE* file);

