#pragma once

typedef struct
{
	int		num;
	char* street;
	char* city;
}Address;

int		initAddress(Address* pAd);
void	printAddress(const Address* pAd);
void	freeAddress(Address* pAd);

int		checkElements(char** elements, int count);
char* fixAddressParam(char* param);
void	freeElements(char** elements, int count);


int readAddressFromFile(Address* pAdd, FILE* file);
void writeAddressToFile(Address* pAdd, FILE* file);
