#pragma once

typedef enum { eFruitVegtable, eFridge, eFrozen, eShelf, eNofProductType } eProductType;
static const char* typeStr[eNofProductType] = { "Fruit Vegtable", "Fridge", "Frozen", "Shelf" };

#define NAME_LENGTH 20
#define BARCODE_LENGTH 7

typedef struct
{
	char			name[NAME_LENGTH + 1];
	char			barcode[BARCODE_LENGTH + 1];
	eProductType	type;
	float			price;
	int				count;
}Product;

void	initProduct(Product* pProduct);
void	initProductNoBarcode(Product* pProduct);
void	initProductName(Product* pProduct);
void	printProduct(const Product* pProduct);

int compareByBarcode(const void* pP1, const void* pP2);
int isSameType(const void* pP1, const void* pP2);
int writeProductToBinFile(Product* pProduct, FILE* file);
Product* readProductFromFile(FILE* file);



void	getBorcdeCode(char* code);
int		isProduct(const Product* pProduct, const char* barcode);
eProductType getProductType();
const char* getProductTypeStr(const eProductType type);

void	updateProductCount(Product* pProduct);
void	freeProduct(Product* pProduct);
