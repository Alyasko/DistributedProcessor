#include "Utils.h"



Utils::Utils()
{
}


Utils::~Utils()
{
}

void Utils::SplitArray(int* partsArray, int partsArraySize, int inputArraySize)
{
	int sizeForUnit = inputArraySize / partsArraySize;
	int remainder = inputArraySize % partsArraySize;

	for (int i = 0; i < partsArraySize; i++)
	{
		int localSizeForUnit = sizeForUnit;
		if (remainder > 0)
		{
			remainder--;
			localSizeForUnit++;
		}

		int subtract = inputArraySize - (i * localSizeForUnit);
		int unitSize = subtract >= localSizeForUnit ? localSizeForUnit : subtract;

		partsArray[i] = unitSize;
	}
}
