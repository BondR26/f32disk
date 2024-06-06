#pragma once
#include <stdlib.h>


#define DEFAULT_CAPACITY 512

typedef struct string
{
	unsigned int _size;
	char data[DEFAULT_CAPACITY];
}string;
