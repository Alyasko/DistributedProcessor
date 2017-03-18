#include "ArrayGenerator.h"
#include "stdlib.h"


ArrayGenerator::ArrayGenerator()
{
	MaxValue = 100;
	FrozenValues = true;
}


ArrayGenerator::~ArrayGenerator()
{
}

void ArrayGenerator::Generate(int* array, int size)
{
	if(FrozenValues == true)
	{
		srand(100);
	}

	for (int i = 0; i < size; i++)
	{
		array[i] = rand() % MaxValue;
	}
}
