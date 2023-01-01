#define  _CRT_SECURE_NO_WARNINGS
#include "Supermarket.h"

int		initSuperMarket(SuperMarket* pMarket, FILE* binMarketFile, FILE* customerFile)
{
	L_init(&pMarket->products); // init list
	if (!&pMarket->products)
	{
		printf("failed init list\n");
		return 0;
	}
	BOOL isBinFileError = False;
	if (!initMarketNameAndAddress(pMarket, binMarketFile)) // init market if binary file failed
	{
		isBinFileError = True;
	}
	if (!isBinFileError) // no error in file, can try to continue reading
	{
		if (!readProductsFromBinFile(pMarket, binMarketFile)) // failed to read products
		{
			printf("failed reading products from binary file\n");
		}
	}
	initMarketCustomers(pMarket, customerFile);
	return 1;
}

int initMarketNameAndAddress(SuperMarket* pMarket, FILE* binMarketFile)
{
	if (!readMarketNameAndAddressFromBinFile(pMarket, binMarketFile)) // failed to read from binary
	{
		pMarket->name = getStrExactLength("Enter market name"); // init name
		if (!pMarket->name)
		{
			printf("market name memory error\n");
			return 0;
		}
		if (!initAddress(&pMarket->location)) // init address
		{
			printf("address memory error\n");
			return 0;
		}
		return 0;
	}
	printf("Name and Address loaded from files\n");
	return 1;
}

void initMarketCustomers(SuperMarket* pMarket, FILE* customerFile) // void since market doesnt depend on it to initialzie
{
	if (!readCustomersFromFile(pMarket, customerFile))
	{
		printf("Error reading customer file\n");
		pMarket->customerCount = 0;
		pMarket->customerArr = NULL;
		return;
	}
	printf("%d customers loaded from file\n", pMarket->customerCount);
}

void printSuperMarket(const SuperMarket* pMarket)
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

	L_insertSorted(&pMarket->products.head, pProd, compareByBarcode);
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

	/*pMarket->customerArr = (Customer*)realloc(pMarket->customerArr, (pMarket->customerCount + 1) * sizeof(Customer));
	if (!pMarket->customerArr)
	{
		free(cust.name);
		return 0;
	}
	original code.. bad realloc will delete array, instead:*/
	Customer* tempArr = (Customer*)realloc(pMarket->customerArr, (pMarket->customerCount + 1) * sizeof(Customer));
	if (!tempArr)
	{
		free(cust.name);
		return 0;
	}

	tempArr[pMarket->customerCount] = cust;
	//pMarket->customerArr[pMarket->customerCount] = cust;
	pMarket->customerCount++;
	pMarket->customerArr = tempArr;
	attributeIndex = -1;
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
	if (pCustomer->pCart->count == 0)
	{
		free(pCustomer->pCart);
		pCustomer->pCart = NULL;
	}
	else
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

	if (isEmptyList(&pMarket->products))
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
	int listLength = L_print(&pMarket->products, NULL);
	printf("There are %d products\n", listLength);
	printf("%-20s %-10s\t", "Name", "Barcode");
	printf("%-20s %-10s %s\n", "Type", "Price", "Count In Stoke");
	printf("--------------------------------------------------------------------------------\n");

	L_print(&pMarket->products, printProduct);
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

	Customer* pCustomer = findCustomerByName(pMarket, name);

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

void	printProductByType(const SuperMarket* pMarket)
{
	if (isEmptyList(&pMarket->products))
	{
		printf("No products in market\n");
		return;
	}
	Product prodType;
	prodType.type = getProductType();
	int count = 0;

	NODE* tmpProd = pMarket->products.head.next;
	while (tmpProd)
	{
		if (isSameType(tmpProd->key, &prodType))
		{
			count++;
			printProduct(tmpProd->key);
		}
		tmpProd = tmpProd->next;
	}
	if (count == 0)
		printf("There are no product of type %s in market %s\n", getProductTypeStr(prodType.type), pMarket->name);
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
	printf("Base on what field do you want to sort?\n");
	attributeIndex = getCustomerSortAttribute();
	qsort(pMarket->customerArr, pMarket->customerCount, sizeof(Customer), sortAttributeArr[attributeIndex - 1]);
	printf("\n\n");
}

void findCustomer(const SuperMarket* pMarket)
{
	if (attributeIndex == -1)
	{
		printf("The search cannot be performed, array not sorted\n");
		return;
	}
	Customer tempCustomer;
	if (attributeIndex == 2) // shop times
	{
		tempCustomer.shopTimes = getPositiveInt("Enter time in market");
		searchByNumbers(pMarket, &tempCustomer, compareByShopTimes);

	}
	else if (attributeIndex == 3) // total spend
	{
		tempCustomer.totalSpend = getPositiveFloat("Enter spent amount");
		searchByNumbers(pMarket, &tempCustomer, compareByTotalSpend);
	}
	else // name 
	{
		searchByName(pMarket);
	}
}

void searchByNumbers(const SuperMarket* pMarket, const Customer* cust, int(*compareFunc)(const void*, const void*))
{
	Customer* result = (Customer*)bsearch(cust, pMarket->customerArr, pMarket->customerCount, sizeof(Customer), compareFunc);
	if (!result)
	{
		printf("Customer was not found\n");
	}
	else
	{
		printCustomer(result);
	}
}

void searchByName(const SuperMarket* pMarket)
{
	Customer tempCust;
	if (!initCustomer(&tempCust))
		return;
	Customer* isExist = findCustomerByName(pMarket, tempCust.name);
	if (!isExist)
	{
		printf("customer doesnt exist, returning\n");
		return;
	}
	Customer* result = (Customer*)bsearch(isExist, pMarket->customerArr, pMarket->customerCount, sizeof(Customer), compareByName);
	if (!result)
	{
		printf("Customer was not found\n");
	}
	else
	{
		printCustomer(result);
	}
}

int saveMarket(SuperMarket* pMarket, FILE* binMarketFile, FILE* customerFile)
{
	if (binMarketFile != NULL)
	{
		if (!writeMarketToBinFile(pMarket, binMarketFile)) // name and address
		{
			fclose(binMarketFile);
		}
	}
	if (customerFile != NULL)
	{
		writeCustomersToFile(pMarket, customerFile); // customers
		fclose(customerFile);
	}
	return 1;
}

void freeMarket(SuperMarket* pMarket)
{
	free(pMarket->name);
	freeAddress(&pMarket->location);
	L_free(&pMarket->products, free);
	generalArrayFunction(pMarket->customerArr, pMarket->customerCount, sizeof(Customer), freeCustomer);
	free(pMarket->customerArr);
}

Product* getProductByBarcode(SuperMarket* pMarket, const char* barcode)
{
	if (isEmptyList(&pMarket->products))
	{
		return NULL;
	}
	NODE* tmp = pMarket->products.head.next;
	Product tempProd;
	strcpy(tempProd.barcode, barcode);
	tmp = L_find(tmp, &tempProd, compareByBarcode);
	if (!tmp) // not found
	{
		return NULL;
	}
	return tmp->key;
}

Customer* findCustomerByName(const SuperMarket* pMarket, const char* name)
{
	for (int i = 0; i < pMarket->customerCount; i++)
	{
		if (isCustomer(&pMarket->customerArr[i], name))
			return &pMarket->customerArr[i];
	}
	return  NULL;
}

int writeMarketToBinFile(SuperMarket* pMarket, FILE* file)
{
	if (!writeDynStrToBinFile(pMarket->name, file)) // name
	{
		printf("FILE ERROR: failed saving market name\n");
		return 0;
	}
	if (!writeAddressToFile(&pMarket->location, file)) // address
	{
		printf("FILE ERROR: failed saving address\n");
		return 0;
	}
	if (isEmptyList(&pMarket->products)) // empty product list
	{
		printf("product list is empty, not saving any products\n");
		return 1;
	}
	if (!writeProductsToBinFile(pMarket, file)) // products
	{
		printf("FILE ERROR: failed saving products\n");
		return 0;
	}
	return 1;
}

int writeProductsToBinFile(SuperMarket* pMarket, FILE* file)
{
	if (isEmptyList(&pMarket->products))
	{
		return 0;
	}
	NODE* tmp = pMarket->products.head.next; // products
	int count = L_print(&pMarket->products, NULL);
	if (fwrite(&count, sizeof(int), 1, file) != 1)
	{
		return 0;
	}
	while (tmp)
	{
		if (writeProductToBinFile(tmp->key, file) != 1)
		{
			return 0;
		}
		tmp = tmp->next;
	}
	return 1;
}

int readMarketNameAndAddressFromBinFile(SuperMarket* pMarket, FILE* file)
{
	if (file == NULL)
	{
		return 0;
	}
	pMarket->name = readDynStrFromBinFile(file);
	if (!pMarket->name)
	{
		printf("company file is empty\n");
		return 0;
	}
	if (!readAddressFromFile(&pMarket->location, file))
	{
		free(pMarket->name);
		return 0;
	}
	return 1;
}

int readProductsFromBinFile(SuperMarket* pMarket, FILE* file)
{
	if (file == NULL)
	{
		return 0;
	}
	size_t listSize = 0;
	if (fread(&listSize, sizeof(int), 1, file) != 1)
	{
		return 0;
	}
	if (!buildMarketListFromBinFile(pMarket, file, listSize))
	{
		printf("error build market list from file\n");
		return 0;
	}
	return 1;
}

int buildMarketListFromBinFile(SuperMarket* pMarket, FILE* file, size_t listSize)
{
	for (size_t i = 0; i < listSize; i++) // build market List
	{
		Product* newProd = readProductFromFile(file);
		if (!newProd)
		{
			return 0;
		}
		L_insertSorted(&pMarket->products.head, newProd, compareByBarcode);
	}
	printf("%zu products loaded from file\n", listSize);
	return 1;
}

void writeCustomersToFile(const SuperMarket* pMarket, FILE* file)
{
	fprintf(file, "%d\n", pMarket->customerCount);
	for (int i = 0; i < pMarket->customerCount; i++)
	{
		Customer tempCustomer = pMarket->customerArr[i];
		if (tempCustomer.pCart)
		{
			printf("---------- Cart info and bill for %s ----------\n", tempCustomer.name);
			tempCustomer.shopTimes++;
			tempCustomer.totalSpend += printShoppingCart(tempCustomer.pCart);
		}
		writeCustomerToFile(&tempCustomer, file);
	}
}

int readCustomersFromFile(SuperMarket* pMarket, FILE* file)
{
	if (file == NULL)
	{
		return 0;
	}
	int size = 0;
	fscanf(file, "%d", &size);
	if (size == 0)
	{
		return 0;
	}
	Customer* tempCustomerArr = (Customer*)malloc(size * sizeof(Customer));
	if (!tempCustomerArr)
	{
		printf("error initializing customers memory\n");
		return 0;
	}
	for (int i = 0; i < size; i++)
	{
		Customer* tempCustomer = initCustomerFromFile(pMarket, file);
		if (!tempCustomer)
		{
			return 0;
		}
		tempCustomerArr[i] = *tempCustomer;
	}
	pMarket->customerCount = size;
	pMarket->customerArr = tempCustomerArr;
	return 1;
}

Customer* initCustomerFromFile(SuperMarket* pMarket, FILE* file)
{
	{
		size_t customerNameLength = 0;
		fscanf(file, "%zu", &customerNameLength);
		Customer* tempCustomer = (Customer*)malloc(sizeof(Customer));
		if (!tempCustomer)
		{
			printf("error initializing customer\n");
			return 0;
		}
		tempCustomer->name = (char*)calloc(customerNameLength, sizeof(char));
		if (!&tempCustomer->name)
		{
			printf("error initializing customer name from file\n");
			return 0;
		}
		tempCustomer = readCustomerFromFile(tempCustomer, file);
		return tempCustomer;
	}
}
