#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "General.h"



char* getStrExactLength(const char* msg)
{
	char* str;
	char temp[MAX_STR_LEN];
	getsStrFixSize(temp, MAX_STR_LEN, msg);

	str = getDynStr(temp);
	return str;
}

char* getDynStr(char* str)
{
	char* theStr;
	theStr = (char*)malloc((strlen(str) + 1) * sizeof(char));
	if (!theStr)
		return NULL;

	strcpy(theStr, str);
	return theStr;
}

char* getsStrFixSize(char* buffer, int size, const char* msg)
{
	puts(msg);
	return myGets(buffer, size);
}

char* myGets(char* buffer, int size)
{
	if (buffer != NULL && size > 0)
	{
		if (fgets(buffer, size, stdin))
		{
			buffer[strcspn(buffer, "\n")] = '\0';
			return buffer;
		}
		buffer[0] = '\0';
	}
	return NULL;
}

char** splitCharsToWords(char* str, int* pCount, const char* del, int* pTotalLength)
{
	char* word;
	int count = 0;
	char* temp = _strdup(str);
	*pTotalLength = 0;
	char** wordsArray = NULL;

	word = strtok(temp, del);
	while (word != NULL)
	{
		wordsArray = (char**)realloc(wordsArray, (count + 1) * sizeof(char*));
		if (!wordsArray)
			return 0;
		wordsArray[count] = getDynStr(word);
		count++;
		*pTotalLength += (int)strlen(word);
		word = strtok(NULL, del);
	}
	*pCount = count;
	free(temp);
	return wordsArray;
}

float	getPositiveFloat(const char* msg)
{
	float val;
	do {
		puts(msg);
		scanf("%f", &val);
	} while (val < 0);
	return val;
}

int		getPositiveInt(const char* msg)
{
	int val;
	do {
		puts(msg);
		scanf("%d", &val);
	} while (val < 0);
	return val;
}

int		countCharInString(const char* str, char tav)
{
	int count = 0;
	while (*str)
	{
		if (*str == tav)
			count++;
		str++;
	}
	return count;
}

int		checkEmptyString(char* str)
{
	while (*str)
	{
		if (!isspace(*str))
			return 0;
		str++;
	}
	return 1;
}

int writeDynStrToBinFile(char* str, FILE* file)
{
	size_t size = strlen(str) + 1; // '\0'
	if (fwrite(&size, sizeof(int), 1, file) != 1)
	{
		return 0;
	}
	if (fwrite(str, sizeof(char), size, file) != size)
	{
		return 0;
	}
	return 1;
}

char* readDynStrFromBinFile(FILE* file)
{
	int size = 0;
	if (fread(&size, sizeof(int), 1, file) != 1)
	{
		return NULL;
	}
	char* string = (char*)calloc(size, sizeof(char));
	if (!string)
	{
		return NULL;
	}
	if (fread(string, sizeof(char), size, file) != size)
	{
		free(string);
		return NULL;
	}
	return string;
}

void generalArrayFunction(void* arr, size_t arrSize, size_t structSize, void(*f)(void*))
{
	for (size_t i = 0; i < arrSize; i++)
	{
		char* item = (char*)(arr)+(i * structSize);
		f(item);
	}
}