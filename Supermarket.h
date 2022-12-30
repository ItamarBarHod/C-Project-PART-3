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
Customer* FindCustomerByName(SuperMarket* pMarket, const char* name);

Customer* getCustomerWhoShop(SuperMarket* pMarket);
Product* getProductAndCount(SuperMarket* pMarket, int* pCount);

void	freeMarket(SuperMarket* pMarket);
void SortCustomersByAttribute(SuperMarket* pMarket);
void findCustomer(const SuperMarket* pMarket);
void writeCustomersToFile(const SuperMarket* pMarket, FILE* file);
void writeMarketAndProductsToBinFile(SuperMarket* pMarket, FILE* file);
int readMarketNameAndAddressFromBinFile(SuperMarket* pMarket, FILE* file);
int readCustomersFromFile(SuperMarket* pMarket, FILE* file);
int readProductsFromBinFile(SuperMarket* pMarket, FILE* file);
int insertProductSorted(SuperMarket* pMarket, Product* pProd, const char* barcode);

