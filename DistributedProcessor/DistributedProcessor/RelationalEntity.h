
#define SLAVE_LISTEN_FOR_ARRAY_SIZE 1
#define SLAVE_ARRAY_SIZE_RECEIVED_CODE 2
#define SLAVE_ARRAY_RECEIVED_CODE 3
#define SLAVE_START_WORKING_CODE 4

#define SLAVE_PRESENCE_CODE 100

#include "cmath"

#pragma once
class RelationalEntity {
public:
	virtual ~RelationalEntity()
	{
		
	}
	virtual void Run() = 0;
protected:
	int ProcessData(int *array, int size)
	{
		int result = 0;

		for (int i = 0; i < size; i++)
		{
			result += array[i] * sin(sqrt(i * 1000 / size)) * cos(sqrt(i * 100 / size)) * sqrt(tan(i));
		}

		return result;
	}
};