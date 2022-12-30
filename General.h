#ifndef __GENERAL__
#define __GENERAL__

#define MAX_STR_LEN 255


char* getStrExactLength(const char* msg);
char* getsStrFixSize(char* buffer, int size, const char* msg);
char* myGets(char* buffer, int size);
char* getDynStr(char* str);
char** splitCharsToWords(char* str, int* pCount, const char* del, int* pTotalLength);
void	 generalArrayFunction(void* arr, size_t arrSize, size_t structSize, void(*f)(void*));
float	getPositiveFloat(const char* msg);
int		getPositiveInt(const char* msg);
int		countCharInString(const char* str, char tav);
int		checkEmptyString(char* str);

int writeDynStrToBinFile(char* str, FILE* file);
char* readDynStrFromBinFile(FILE* file);

#endif

