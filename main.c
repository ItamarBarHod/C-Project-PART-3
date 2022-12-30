#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "General.h"
#include "Supermarket.h"




// can i use static bool in supermarket for sort indicator?
// can i create L_count even though L_print returns a number (but prints before)
// can i create L_insertToEnd in List.c?
// general L_delete + L_free, just add bool? --- didnt see generic List file
// can i use isInit in main? (added logic in main)
// should the program work if the user only deletes the binary file?
// why check binary files write but not regular file write?

int menu();



const char* menuStrings[eNofOptions] = { "Show SuperMarket", "Add Product",
								"Add Customer", "Customer Shopping","Print Shopping Cart","Customer Pay",
								"Print Product By Type", "Sort Customers by Attribute" , "Find Customer" };

int main()
{
	SuperMarket	market;

	FILE* binMarketFile = fopen("SuperMarket", "rb");
	if (binMarketFile == NULL)
	{
		printf("Error open company file\n");
	}
	FILE* customerFile = fopen("Customers.txt", "r");
	if (customerFile == NULL)
	{
		printf("Error opening the write customers file\n");
	}

	if (!initSuperMarket(&market, binMarketFile, customerFile))
	{
		printf("error init  Super Market");
		return 0;
	}

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

		case ePrintProductByType:
			printProductByType(&market);
			break;

		case eSortCustomersByAttribute:
			SortCustomersByAttribute(&market);
			break;

		case eFindCustomer:
			findCustomer(&market);
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

	binMarketFile = fopen("SuperMarket", "wb");
	if (binMarketFile == NULL)
	{
		printf("Error open company file\n");
		return 0;
	}
	customerFile = fopen("Customers.txt", "w");
	if (customerFile == NULL)
	{
		printf("Error opening the write customers file\n");
		return 0;
	}

	writeMarketAndProductsToBinFile(&market, binMarketFile);
	writeCustomersToFile(&market, customerFile);
	freeMarket(&market);
	fclose(customerFile);
	fclose(binMarketFile);

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

