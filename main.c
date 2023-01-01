#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "General.h"
#include "Supermarket.h"

// can i use the static sort variable for sort indicator?
// can i use the L_insertSorted in List.c?
// why check binary files write but not regular file write?
// why no include guards

// TODO - freeShoppingCart 

int menu();

const char* menuStrings[eNofOptions] = { "Show SuperMarket", "Add Product",
								"Add Customer", "Customer Shopping","Print Shopping Cart","Customer Pay",
								"Sort Customers by Attribute" , "Find Customer", "Print Product By Type" };

int main()
{
	SuperMarket	market;
	FILE* binMarketFile = NULL, * customerFile = NULL;

	openFilesToRead(&binMarketFile, &customerFile);
	if (!initSuperMarket(&market, binMarketFile, customerFile))
	{
		printf("error init Super Market");
		return 0;
	}
	closeFiles(binMarketFile, customerFile);

	int option;
	int stop = 0;
	do
	{
		option = menu();
		switch (option)
		{
		case eShowSuperMarket:
			printSuperMarket(&market);
			break;

		case eAddProduct:
			if (!addProduct(&market))
				printf("Error adding product\n");
			break;

		case eAddCustomer:
			if (!addCustomer(&market))
				printf("Error adding customer\n");
			break;

		case eCustomerDoShopping:
			if (!doShopping(&market))
				printf("Error in shopping\n");
			break;
		case ePrintCart:
			doPrintCart(&market);
			break;

		case eCustomerPay:
			if (!doPayment(&market))
				printf("Error in payment\n");
			break;

		case eSortCustomersByAttribute:
			SortCustomersByAttribute(&market);
			break;

		case eFindCustomer:
			findCustomer(&market);
			break;

		case ePrintProductByType:
			printProductByType(&market);
			break;

		case EXIT:
			printf("Bye bye\n");
			stop = 1;
			break;

		default:
			printf("Wrong option\n");
			break;
		}
	} while (!stop);

	openFilesToWrite(&binMarketFile, &customerFile);
	if (!saveMarket(&market, binMarketFile, customerFile)) // closes files inside
	{
		printf("failed saving to binary file\n");
	}
	freeMarket(&market);

	system("pause");
	return 1;
}

int menu()
{
	int option;
	printf("\n\n");
	printf("Please choose one of the following options\n");
	for (int i = 0; i < eNofOptions; i++)
		printf("%d - %s\n", i, menuStrings[i]);
	printf("%d - Quit\n", EXIT);
	scanf("%d", &option);
	//clean buffer
	char tav;
	scanf("%c", &tav);
	return option;
}

void openFilesToRead(FILE** binMarketFile, FILE** customerFile)
{
	*binMarketFile = fopen("SuperMarket", "rb");
	if (*binMarketFile == NULL)
	{
		printf("Error open company file to read\n");
	}
	*customerFile = fopen("Customers.txt", "r");
	if (*customerFile == NULL)
	{
		printf("Error opening the customers file to read\n");
	}
}

void openFilesToWrite(FILE** binMarketFile, FILE** customerFile)
{
	*binMarketFile = fopen("SuperMarket", "wb");
	if (*binMarketFile == NULL)
	{
		printf("Error open company file to write\n");
	}
	*customerFile = fopen("Customers.txt", "w");
	if (*customerFile == NULL)
	{
		printf("Error opening the customers file to write\n");
	}
}


void closeFiles(FILE* binMarketFile, FILE* customerFile)
{
	if (binMarketFile != NULL)
	{
		fclose(binMarketFile);
	}
	if (customerFile != NULL)
	{
		fclose(customerFile);
	}
}