#pragma once


#define EXIT -1

typedef enum
{
	eShowSuperMarket, eAddProduct, eAddCustomer, eCustomerDoShopping, ePrintCart, eCustomerPay,
	eSortCustomersByAttribute, eFindCustomer, ePrintProductByType, eNofOptions
} eMenuOptions;

const char* menuStrings[eNofOptions];

void openFilesToRead(FILE** binMarketFile, FILE** customerFile);
void openFilesToWrite(FILE** binMarketFile, FILE** customerFile);
void closeFiles(FILE* binMarketFile, FILE* customerFile);

int menu();