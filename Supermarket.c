#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Supermarket.h"
#include "Product.h"
#include "Customer.h"
#include "Address.h"
#include "General.h"
#include "ShoppingCart.h"



int		initSuperMarket(SuperMarket* pMarket)
{
	pMarket->customerCount = 0;
	pMarket->customerArr = NULL;
	L_init(&pMarket->Products);
	if (!&pMarket->Products)
	{
		return 0;
	}
	pMarket->name = getStrExactLength("Enter market name");
	return initAddress(&pMarket->location);
}

void	printSuperMarket(const SuperMarket* pMarket)
{
	printf("Super Market Name: %s\t", pMarket->name);
	printf("Address: ");
	printAddress(&pMarket->location);
	printf("\n");
	printAllProducts(pMarket);
	printf("\n");
	printAllCustomers(pMarket);
}

int		addProduct(SuperMarket* pMarket)
{
	char barcode[BARCODE_LENGTH + 1];
	Product* pProd = getProductFromUser(pMarket, barcode);
	if (pProd != NULL) //This barcode exist in stock
		updateProductCount(pProd);
	else { //new product to stock
		if (!addNewProduct(pMarket, barcode))
			return 0;
	}

	return 1;
}

int		addNewProduct(SuperMarket* pMarket, const char* barcode)
{
	Product* pProd = (Product*)calloc(1, sizeof(Product));
	if (!pProd)
	{
		return 0;
	}
	strcpy(pProd->barcode, barcode);
	initProductNoBarcode(pProd);

	NODE* newNode = L_find(pMarket->Products.head.next, pProd, compareByBarcode);
	if (newNode) // exists
	{
		free(pProd);
		return 0;
	}
	if (pMarket->Products.head.next == NULL) // first product
	{
		if (!L_insert(&pMarket->Products.head, pProd)) // if insert fails
		{
			free(pProd);
			return 0;
		}
		return 1;
	}
	if (!insertProductSorted(pMarket, pProd, barcode))
	{
		free(pProd);
		return 0;
	}
	return 1;
}

int insertProductSorted(SuperMarket* pMarket, Product* pProd, char* barcode)
{
	Product* tmp = (Product*)pMarket->Products.head.next->key;
	if (strcmp(tmp->barcode, barcode) > 0) // insert to start O(1)
	{
		if (!L_insert(&pMarket->Products.head, pProd)) // if insert fails
		{
			return 0;
		}
	}
	else { // insert to end O(n)
		NODE* newNode = (NODE*)malloc(sizeof(NODE));
		if (!newNode)
		{
			return 0;
		}
		newNode->key = pProd;

		NODE* tmp = pMarket->Products.head.next; // start
		while (tmp)
		{
			if (tmp->next == NULL)
			{
				tmp->next = newNode;
				newNode->next = NULL;
			}
			tmp = tmp->next;
		} // reached end
	}
	return 1;
}

int		addCustomer(SuperMarket* pMarket)
{
	Customer cust;
	if (!initCustomer(&cust))
		return 0;

	if (isCustomerInMarket(pMarket, &cust))
	{
		printf("This customer already in market\n");
		free(cust.name);
		return 0;
	}

	pMarket->customerArr = (Customer*)realloc(pMarket->customerArr, (pMarket->customerCount + 1) * sizeof(Customer));
	if (!pMarket->customerArr)
	{
		free(cust.name);
		return 0;
	}

	pMarket->customerArr[pMarket->customerCount] = cust;
	pMarket->customerCount++;
	return 1;
}

int		isCustomerInMarket(SuperMarket* pMarket, Customer* pCust)
{
	for (int i = 0; i < pMarket->customerCount; i++)
	{
		if (strcmp(pMarket->customerArr[i].name, pCust->name) == 0)
			return 1;
	}
	return 0;
}

int	doShopping(SuperMarket* pMarket)
{
	Customer* pCustomer = getCustomerShopPay(pMarket);
	if (!pCustomer)
		return 0;

	if (pCustomer->pCart == NULL)
	{
		pCustomer->pCart = (ShoppingCart*)malloc(sizeof(ShoppingCart));
		if (!pCustomer->pCart)
			return 0;
		initCart(pCustomer->pCart);
	}
	fillCart(pMarket, pCustomer->pCart);
	printf("---------- Shopping ended ----------\n");
	return 1;
}

Customer* doPrintCart(SuperMarket* pMarket)
{
	Customer* pCustomer = getCustomerShopPay(pMarket);
	if (!pCustomer)
		return NULL;
	if (pCustomer->pCart == NULL)
	{
		printf("Customer cart is empty\n");
		return NULL;
	}
	printShoppingCart(pCustomer->pCart);
	return pCustomer;
}

int	doPayment(SuperMarket* pMarket)
{
	Customer* pCustomer = doPrintCart(pMarket);
	if (!pCustomer)
		return 0;
	pay(pCustomer);
	return 1;
}

Customer* getCustomerShopPay(SuperMarket* pMarket)
{
	if (pMarket->customerCount == 0)
	{
		printf("No customer listed to market\n");
		return NULL;
	}

	if (pMarket->Products.head.next == NULL)
	{
		printf("No products in market - cannot shop\n");
		return NULL;
	}

	Customer* pCustomer = getCustomerWhoShop(pMarket);
	if (!pCustomer)
	{
		printf("this customer not listed\n");
		return NULL;
	}

	return pCustomer;
}

void	printAllProducts(const SuperMarket* pMarket)
{
	int listLength = L_count(&pMarket->Products); // L_print also counts but returns the value after prints.
	printf("There are %d products\n", listLength);
	printf("%-20s %-10s\t", "Name", "Barcode");
	printf("%-20s %-10s %s\n", "Type", "Price", "Count In Stoke");
	printf("--------------------------------------------------------------------------------\n");

	L_print(&pMarket->Products, printProduct);
}

void	printAllCustomers(const SuperMarket* pMarket)
{
	printf("There are %d listed customers\n", pMarket->customerCount);
	generalArrayFunction(pMarket->customerArr, pMarket->customerCount, sizeof(Customer), printCustomer);
}


Customer* getCustomerWhoShop(SuperMarket* pMarket)
{
	printAllCustomers(pMarket);
	char name[MAX_STR_LEN];
	getsStrFixSize(name, sizeof(name), "Who is shopping? Enter cutomer name\n");

	Customer* pCustomer = FindCustomerByName(pMarket, name);

	return pCustomer;
}


void fillCart(SuperMarket* pMarket, ShoppingCart* pCart)
{
	printAllProducts(pMarket);
	char op;
	while (1)
	{
		printf("Do you want to shop for a product? y/Y, anything else to exit!!\t");
		do {
			scanf("%c", &op);
		} while (isspace(op));
		getchar(); //clean the enter
		if (op != 'y' && op != 'Y')
			break;
		int count;
		Product* pProd = getProductAndCount(pMarket, &count);
		if (pProd)
		{
			if (!addItemToCart(pCart, pProd->barcode, pProd->price, count))
			{
				printf("Error adding item\n");
				return;
			}
			pProd->count -= count; //item bought!!!
		}
	}
}

Product* getProductAndCount(SuperMarket* pMarket, int* pCount)
{
	char barcode[BARCODE_LENGTH + 1];
	Product* pProd = getProductFromUser(pMarket, barcode);
	if (pProd == NULL)
	{
		printf("No such product\n");
		return NULL;
	}

	if (pProd->count == 0)
	{
		printf("This product out of stock\n");
		return NULL;
	}

	int count;
	do {
		printf("How many items do you want? max %d\n", pProd->count);
		scanf("%d", &count);
	} while (count <= 0 || count > pProd->count);
	*pCount = count;
	return pProd;
}

void	printProductByType(SuperMarket* pMarket)
{
	if (pMarket->Products.head.next == NULL)
	{
		printf("No products in market\n");
		return;
	}
	eProductType	type = getProductType();
	int count = 0;

	NODE* tmpProd = pMarket->Products.head.next;
	while (tmpProd)
	{
		if (isSameType(tmpProd->key, type))
		{
			count++;
			printProduct(tmpProd->key);
		}
		tmpProd = tmpProd->next;
	}
	//for (int i = 0; i < pMarket->productCount; i++)
	//{
	//	if (pMarket->Products == type)
	//	{
	//		count++;
	//		printProduct(pMarket->productArr[i]);
	//	}
	//}
	if (count == 0)
		printf("There are no product of type %s in market %s\n", getProductTypeStr(type), pMarket->name);
}

Product* getProductFromUser(SuperMarket* pMarket, char* barcode)
{
	getBorcdeCode(barcode);
	return getProductByBarcode(pMarket, barcode);
}

void SortCustomersByAttribute(SuperMarket* pMarket)
{
	if (pMarket->customerCount == 0)
	{
		printf("Error: no customers exist yet\n");
		return;
	}
	int result = getCustomerAttribute();
	qsort(pMarket->customerArr, pMarket->customerCount, sizeof(Customer), customerAttr[result]);
	isSorted = True;
}

void findCustomer(const SuperMarket* pMarket)
{
	if (isSorted == False)
	{
		printf("The search cannot be performed, array not sorted\n");
		return;
	}
	char* name = getStrExactLength("Enter customer name\n");
	if (!name)
	{
		printf("error init name\n");
		return;
	}
	Customer tempCust;
	tempCust.name = name;
	Customer* result = (Customer*)bsearch(&tempCust, pMarket->customerArr, pMarket->customerCount, sizeof(Customer), compareByName);
	if (!result)
	{
		printf("Customer not found\n");
	}
	else
	{
		printCustomer(result);
	}
}

void	freeMarket(SuperMarket* pMarket)
{
	free(pMarket->name);
	freeAddress(&pMarket->location);
	L_free(&pMarket->Products, True, free);
	//for (int i = 0; i < pMarket->productCount; i++)
	//{
	//	freeProduct(pMarket->productArr[i]);
	//	free(pMarket->productArr[i]);
	//}
	//free(pMarket->productArr);

	generalArrayFunction(pMarket->customerArr, pMarket->customerCount, sizeof(Customer), freeCustomer);
	free(pMarket->customerArr);
}

//void	getUniquBarcode(char* barcode, SuperMarket* pMarket)
//{
//	int cont = 1;
//	while (cont)
//	{
//		getBorcdeCode(barcode);
//		int index = getProductIndexByBarcode(pMarket, barcode);
//		if (index == -1)
//			cont = 0;
//		else
//			printf("This product already in market\n");
//	}
//}

//int getProductIndexByBarcode(SuperMarket* pMarket, const char* barcode)
//{
//	if (pMarket->Products.head.next == NULL)
//	{
//		return -1;
//	}
//	L_find
//	/*for (int i = 0; i < pMarket->productCount; i++)
//	{
//		if (isProduct(pMarket->productArr[i], barcode))
//			return i;
//	}
//	return -1;*/
//
//}


Product* getProductByBarcode(SuperMarket* pMarket, const char* barcode)
{
	if (pMarket->Products.head.next == NULL)
	{
		return NULL;
	}
	NODE* tmp = pMarket->Products.head.next;
	Product tempProd;
	strcpy(&tempProd.barcode, barcode);
	tmp = L_find(tmp, &tempProd, compareByBarcode);
	if (!tmp) // not found
	{
		return NULL;
	}
	return tmp->key;
	//for (int i = 0; i < pMarket->productCount; i++)
	//{
	//	if (isProduct(pMarket->productArr[i], barcode))
	//		return pMarket->productArr[i];
	//}

}

Customer* FindCustomerByName(SuperMarket* pMarket, const char* name)
{
	for (int i = 0; i < pMarket->customerCount; i++)
	{
		if (isCustomer(&pMarket->customerArr[i], name))
			return &pMarket->customerArr[i];
	}
	return  NULL;
}

void writeMarketAndProductsToFile(const SuperMarket* pMarket, FILE* file)
{
	fwrite(pMarket->name, sizeof(pMarket->name) + 1, 1, file); // name
	writeAddressToFile(&pMarket->location, file); // address
	if (pMarket->Products.head.next == NULL)
	{
		return;
	}
	int productCount = L_count(&pMarket->Products);
	fwrite(&productCount, sizeof(int), 1, file);
	NODE* tmp = pMarket->Products.head.next;
	while (tmp)
	{
		writeProductToFile(tmp->key, file);
		tmp = tmp->next;
	}
}

void readMarketAndProductsFromFile(SuperMarket* pMarket, FILE* file)
{
	size_t len;
	fread(&len, sizeof(int), 1, file);
	char* marketName = (char*)malloc(len * sizeof(char));
	if (!marketName)
	{
		return;
	}
	fread(marketName, sizeof(char), len, file);
	marketName[len - 1] = '\0'; // needed?
	if (!readAddressFromFile(&pMarket->location, file))
	{
		free(marketName);
		return;
	}
	if (!readProductsFromFile(&pMarket, file))
	{
		free(marketName);
		return;
	}
}

int readProductsFromFile(SuperMarket* pMarket, FILE* file)
{
	size_t len;
	fread(&len, sizeof(int), 1, file);
	NODE* pMarketNodes = NULL;
	for (int i = 0; i < len; i++) // while !eof  ?
	{
		Product* newProd = (Product*)malloc(sizeof(Product));
		if (!newProd)
		{
			return 0;
		}
		readProductFromFile(newProd, file);
		NODE* newNode = (NODE*)malloc(sizeof(NODE));
		if (!newNode)
		{
			free(newProd);
			return 0;
		}
		L_insert(pMarketNodes, newProd);
	}
	pMarket->Products.head.next = pMarketNodes;
	return 1;
}

void writeCustomersToFile(const SuperMarket* pMarket, FILE* file)
{
	fprintf(file, "%d\n", pMarket->customerCount);
	for (int i = 0; i < pMarket->customerCount; i++)
	{
		Customer tempCustomer = pMarket->customerArr[i];
		writeCustomerToFile(&tempCustomer, file);
	}
}

void readCustomersFromFile(SuperMarket* pMarket, FILE* file)
{
	size_t result = 0;
	fscanf(file, "%d", &result);
	if (result == 0)
	{
		return;
	}
	pMarket->customerCount = result;
	Customer* customerArr = (Customer*)malloc(pMarket->customerCount * sizeof(Customer));
	if (!customerArr)
	{
		printf("error initializing customers from file\n");
		return;
	}
	for (int i = 0; i < pMarket->customerCount; i++)
	{
		size_t customerNameLength = 0;
		fscanf(file, "%d", &customerNameLength);
		Customer* tempCustomer = (Customer*)malloc(sizeof(Customer));
		if (!tempCustomer)
		{
			return;
		}
		tempCustomer->name = (char*)malloc(customerNameLength * sizeof(char));
		if (!tempCustomer->name)
		{
			free(tempCustomer);
			printf("error initializing customer name from file\n");
			return;
		}
		tempCustomer->name[customerNameLength - 1] = '\0'; // temp fix, why strlen 14?
		//printf("tempCustomer: %s length: %d sizet: %d\n", tempCustomer->name, strlen(tempCustomer->name, customerNameLength)); 
		// system("pause\n");
		readCustomerFromFile(tempCustomer, file);
		customerArr[i] = *tempCustomer;
	}
	pMarket->customerArr = customerArr;
}