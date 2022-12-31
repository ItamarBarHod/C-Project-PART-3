#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "Product.h"
#include "General.h"

#define MIN_DIG 3
#define MAX_DIG 5

void	initProduct(Product* pProduct)
{
	initProductNoBarcode(pProduct);
	getBorcdeCode(pProduct->barcode);
}

void	initProductNoBarcode(Product* pProduct)
{
	initProductName(pProduct);
	pProduct->type = getProductType();
	pProduct->price = getPositiveFloat("Enter product price\t");
	pProduct->count = getPositiveInt("Enter product number of items\t");
}

void initProductName(Product* pProduct)
{
	do {
		printf("enter product name up to %d chars\n", NAME_LENGTH);
		myGets(pProduct->name, sizeof(pProduct->name));
	} while (checkEmptyString(pProduct->name));
}

void	printProduct(const Product* pProduct)
{
	printf("%-20s %-10s\t", pProduct->name, pProduct->barcode);
	printf("%-20s %5.2f %10d\n", typeStr[pProduct->type], pProduct->price, pProduct->count);
}

void getBorcdeCode(char* code)
{
	char temp[MAX_STR_LEN];
	char msg[MAX_STR_LEN];
	sprintf(msg, "Code should be of %d length exactly\n"
		"UPPER CASE letter and digits\n"
		"Must have %d to %d digits\n"
		"First and last chars must be UPPER CASE letter\n"
		"For example A12B40C\n",
		BARCODE_LENGTH, MIN_DIG, MAX_DIG);
	int ok = 1;
	int digCount = 0;
	do {
		ok = 1;
		digCount = 0;
		printf("Enter product barcode ");
		getsStrFixSize(temp, MAX_STR_LEN, msg);
		if (strlen(temp) != BARCODE_LENGTH)
		{
			puts(msg);
			ok = 0;
		}
		else {
			//check and first upper letters
			if (!isupper(temp[0]) || !isupper(temp[BARCODE_LENGTH - 1]))
			{
				puts("First and last must be upper case letters\n");
				ok = 0;
			}
			else {
				for (int i = 1; i < BARCODE_LENGTH - 1; i++)
				{
					if (!isupper(temp[i]) && !isdigit(temp[i]))
					{
						puts("Only upper letters and digits\n");
						ok = 0;
						break;
					}
					if (isdigit(temp[i]))
						digCount++;
				}
				if (digCount < MIN_DIG || digCount > MAX_DIG)
				{
					puts("Incorrect number of digits\n");
					ok = 0;
				}
			}
		}

	} while (!ok);

	strcpy(code, temp);
}


eProductType getProductType()
{
	int option;
	printf("\n\n");
	do {
		printf("Please enter one of the following types\n");
		for (int i = 0; i < eNofProductType; i++)
			printf("%d for %s\n", i, typeStr[i]);
		scanf("%d", &option);
	} while (option < 0 || option >= eNofProductType);
	getchar();
	return (eProductType)option;
}

const char* getProductTypeStr(const eProductType type)
{
	if (type < 0 || type >= eNofProductType)
		return NULL;
	return typeStr[type];
}

int		isProduct(const Product* pProduct, const char* barcode)
{
	if (strcmp(pProduct->barcode, barcode) == 0)
		return 1;
	return 0;
}

void	updateProductCount(Product* pProduct)
{
	int count;
	do {
		printf("How many items to add to stock?");
		scanf("%d", &count);
	} while (count < 1);
	pProduct->count += count;
}

int compareByBarcode(const void* pP1, const void* pP2)
{
	const Product* prod1 = (const Product*)pP1;
	const Product* prod2 = (const Product*)pP2;
	return (strcmp(prod1->barcode, prod2->barcode));
}

int isSameType(const void* pP1, const void* pP2)
{
	Product* prod1 = (Product*)pP1;
	Product* prod2 = (Product*)pP2;
	return (prod1->type == prod2->type);
}

int writeProductToBinFile(Product* pProduct, FILE* file)
{
	if (fwrite(&pProduct->name, sizeof(char), NAME_LENGTH + 1, file) != NAME_LENGTH + 1)
	{
		return 0;
	}
	if (fwrite(&pProduct->barcode, sizeof(char), BARCODE_LENGTH + 1, file) != BARCODE_LENGTH + 1)
	{
		return 0;
	}
	if (fwrite(&pProduct->type, sizeof(int), 1, file) != 1)
	{
		return 0;
	}
	if (fwrite(&pProduct->price, sizeof(float), 1, file) != 1)
	{
		return 0;
	}
	if (fwrite(&pProduct->count, sizeof(int), 1, file) != 1)
	{
		return 0;
	}
	return 1;
}

Product* readProductFromFile(FILE* file)
{
	Product* newProd = (Product*)malloc(sizeof(Product));
	if (!newProd)
	{
		return 0;
	}
	if (fread(&newProd->name, sizeof(char), NAME_LENGTH + 1, file) != NAME_LENGTH + 1)
	{
		return NULL;
	}
	if (fread(&newProd->barcode, sizeof(char), BARCODE_LENGTH + 1, file) != BARCODE_LENGTH + 1)
	{
		return NULL;
	}
	if (fread(&newProd->type, sizeof(int), 1, file) != 1)
	{
		return NULL;
	}
	if (fread(&newProd->price, sizeof(float), 1, file) != 1)
	{
		return NULL;
	}
	if (fread(&newProd->count, sizeof(int), 1, file) != 1)
	{
		return NULL;
	}
	return newProd;
}

void	freeProduct(Product* pProduct)
{
	//nothing to free!!!!
}