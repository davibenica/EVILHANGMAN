#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "my_string.h"

struct my_string
{
	int size;
	int capacity;
	char* data;
};
typedef struct my_string My_string;

MY_STRING my_string_init_default(void)
{
	My_string* pString = (My_string*) malloc(sizeof(My_string));

	if (pString != NULL)
	{
		pString->size = 0;
		pString->capacity = 7;
		pString->data = (char*) malloc(sizeof(char) * pString->capacity);
		
		if (pString->data  == NULL)
		{
			free(pString);
			return NULL;
		}
	}
	
	return pString;
}

MY_STRING my_string_init_c_string(const char* c_string)
{
	My_string* pString = (My_string*) malloc(sizeof(My_string));
	
	if  (pString != NULL) 
	{
		// Get size of c_string
		int len = 0;
		while(c_string[len])
		{
			len++;
		}
		
		pString->size = len;	
		pString->capacity = len + 1;
		
		pString->data = (char*) malloc(sizeof(char)* pString->capacity);
		
		if (pString->data == NULL)
		{
			return NULL;		
		}
		
		for (int i = 0; i < pString->size; i++)
		{
			pString->data[i] = c_string[i];
						
		}
	}
	return(pString);
}



void my_string_destroy(MY_STRING* phMy_string)
{
	My_string* pString = (My_string*) *phMy_string;
	free(pString->data);
	free(pString);
	*phMy_string = NULL;
	
}

int my_string_get_capacity(MY_STRING hMy_string)
{
	My_string* pString = (My_string*) hMy_string;
	return pString->capacity;
}

int my_string_get_size(MY_STRING hMy_string)
{
	My_string* pString = (My_string*) hMy_string;
	return pString->size;
}

int my_string_compare(MY_STRING hLeft_string, MY_STRING hRight_string)
{
	
	My_string* pLeft_String = (My_string*) hLeft_string;
	My_string* pRight_String = (My_string*) hRight_string;
	
	char* left_string_data = pLeft_String->data;
	char* right_string_data = pRight_String->data;
	
	// get size of each string
	int left_size = my_string_get_size(hLeft_string);
	int right_size = my_string_get_size(hRight_string);
	
	
	// Comparing the string
	int i = 0;
	
	while (i <  left_size || i < right_size)
	{
		if (left_string_data[i] < right_string_data[i])
		{
			return -1;
		}
		if (left_string_data[i] > right_string_data[i])
		{
			return 1;
		}
		i++;
	}
	
	if (left_size < right_size)
	{
		return -1;
	}
	if (left_size > right_size)
	{
		return 1;	
	}
	return 0;
			
	}
	
	Status my_string_push_back(MY_STRING hMy_string, char character)
	{
		My_string* pMy_string =(My_string*) hMy_string;
		
		
		
		char* temp;
		
		if (pMy_string->size >= pMy_string->capacity)
		{
			temp = (char*)malloc(sizeof(int) * pMy_string->capacity * 2);
			if (temp == NULL)
			{
				return FAILURE;			
			}
			
			for(int i=0; i < pMy_string->size; i++)
			{
				temp[i] = pMy_string->data[i];		
			}
			free(pMy_string->data);
			pMy_string->data = temp;
			pMy_string->capacity *= 2;
		}
		
		pMy_string->data[pMy_string->size] = character;
		pMy_string->size++;
		return SUCCESS;
	}
	
	Status my_string_extraction(MY_STRING hMy_string, FILE* fp)
	{
		My_string* pMy_string =(My_string*) hMy_string;
		
		if (pMy_string == NULL)
		{
			return FAILURE;
		}
		
		//Get string from stdin
		
		// Keep track of a int to check if the non white space is found
		int non_white_space = 0;
		// Erase previous string in my_String
		pMy_string->size = 0;
		 		
		char c;
		while ((c = fgetc(fp)) != EOF)
		{
			if (!isspace(c))
			{
				non_white_space = 1;
				my_string_push_back(hMy_string, (char) c);
			
			}
			else if(non_white_space == 1)
			{
				ungetc(c, fp);
				return SUCCESS;			
			}
								
		}
		
		
		return FAILURE;
	}
	
	Status my_string_insertion(MY_STRING hMy_string, FILE* fp)
	{
		My_string* pMy_string =(My_string*) hMy_string;
		
		if (pMy_string == NULL)
		{
			return FAILURE;
		}
		
		for(int i = 0; i < pMy_string->size; i++)
		{
			fputc(pMy_string->data[i], fp);		
		}
		
		return SUCCESS;	
	}
	
	Status my_string_pop_back(MY_STRING hMy_string)
	{
		
		My_string* pMy_string =(My_string*) hMy_string;
		
		if (pMy_string == NULL || pMy_string->size == 0)
		{
			return FAILURE;
		}
		
		pMy_string->size --;
		return SUCCESS;
			
	}
	
	char* my_string_at(MY_STRING hMy_string, int index)
	{
		My_string* pMy_string =(My_string*) hMy_string;
		
		if (pMy_string == NULL || index >= pMy_string->size || index < 0)
		{
			return NULL;
		}
		
		return &(pMy_string->data[index]);
			
	}
	
	char* my_string_c_str(MY_STRING hMy_string)
	{
		
		My_string* pMy_string =(My_string*) hMy_string;
		
		if (pMy_string->size >=  pMy_string->capacity)
		{
			pMy_string->capacity ++;
			char* temp = (char*) malloc(sizeof(char) * pMy_string->capacity);
			
			for(int i = 0; i < pMy_string->size; i++)
			{
				temp[i] = pMy_string->data[i];
			}
			free(pMy_string->data);
			pMy_string->data = temp;
		
		}
		pMy_string->data[pMy_string->size] = '\0';
		
		return pMy_string->data;
			
	}
	
	Status my_string_concat(MY_STRING hResult, MY_STRING hAppend)
	{
		My_string* pResult = (My_string*) hResult;
		My_string* pAppend = (My_string*) hAppend;
		
		if (pResult == NULL ||pAppend == NULL)
		{
			return FAILURE;
		}
		
		for(int i = 0 ; i < pAppend->size; i++)
		{
			
	  		 char* character = my_string_at(hAppend, i);
	  		 
	  		 
	  		 if(my_string_push_back(hResult, *character) == FAILURE)
	  		 {
		 			return FAILURE;
	  		 }
		}
		
		return SUCCESS;
		
		
		
	}
	Boolean my_string_empty(MY_STRING hMy_string)
	{
		My_string* pMy_string =(My_string*) hMy_string;
		
		return(pMy_string->size == 0) ? TRUE:FALSE;
	}
	
	Status my_string_assignment(MY_STRING hLeft, MY_STRING hRight)
	{
		My_string* pLeft = (My_string*) hLeft;
		My_string* pRight = (My_string*) hRight;
		
		if (pLeft == pRight)
		{
			return SUCCESS;		
		}
		
		
		// Erase the first string
		pLeft->size = 0;
		
		for(int i = 0; i < pRight->size; i++)
		{
			if (!my_string_push_back(hLeft, pRight->data[i]))
			{
				return FAILURE;			
			}
					
		}
		return SUCCESS;
	}
	
	MY_STRING my_string_init_copy(MY_STRING hMy_string)
	{
		My_string* pString = (My_string*) hMy_string;
		
		My_string* pCopy = (My_string*) malloc(sizeof(My_string));
		
		if (pCopy != NULL)
		{
			pCopy->size = pString->size;
			pCopy->capacity = pString->capacity;
			pCopy->data = (char*) malloc(sizeof(char) * pCopy->capacity);
			
			if (pCopy->data == NULL)
			{
				free(pCopy);
				return NULL;			
			}
			
			for (int i = 0; i < pString->size; i++)
			{
				pCopy->data[i] = pString->data[i];
			}
			
		}
		return pCopy;
	}
	
	
	void my_string_swap(MY_STRING hLeft, MY_STRING hRight)
	{
		
		My_string* pLeft = (My_string*) hLeft;
		My_string* pRight = (My_string*) hRight;
		
		int temp_size = pLeft->size;
		int temp_capacity = pLeft->capacity;
		char* temp_data = pLeft->data;
		
		pLeft->size = pRight->size;
		pLeft->capacity = pRight->capacity;
		pLeft->data = pRight->data;
		
		pRight->size = temp_size;
		pRight->capacity = temp_capacity;
		pRight->data = temp_data;
		
	
	}
	
	
	