#include "Master.h"
#include <iostream>
#include <omp.h>
#include "DistributedArrayProcessor.h"
#include <mpi.h>

#include "ArrayGenerator.h"
#include "Utils.h"
#include "Logger.h"

using namespace std;

#define ARRAY_SIZE 100

int Master::ArraySize;
int Master::ThreadsCount;

Master::Master() {

}


Master::~Master()
{
}

void Master::Run()
{
	Logger::Log("\n--- Master started ---\n");

	// Wait for slaves.
	//byte buffer1;
	//MPI_Recv(&buffer1, 1, MPI_BYTE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	byte buffer = SLAVE_LISTEN_FOR_ARRAY_SIZE;

	if(DistributedArrayProcessor::WorldSize > 1)
	{
		WaitForSlaves(SLAVE_PRESENCE_CODE, "M: slave sent incorrect presence code\n");

		Logger::Log("M: everybody is present\n");

		// Slaves listen for array size.

		for (int i = 1; i < DistributedArrayProcessor::WorldSize; i++)
		{
			MPI_Send(&buffer, 1, MPI_BYTE, i, 0, MPI_COMM_WORLD);
		}
	}

	// Create array.

	int *array = new int[ArraySize];

	int worldSize = DistributedArrayProcessor::WorldSize;
	int *unitSizes = new int[worldSize];

	double startTime = omp_get_wtime();

	if (ArraySize < 10)
	{
		PrintArray(array, ArraySize);
	}

	// Send array size.

	Logger::Log("M: sending array size\n"); 

	Utils::SplitArray(unitSizes, worldSize, ArraySize);

	if (DistributedArrayProcessor::WorldSize > 1)
	{
		for (int i = 0; i < DistributedArrayProcessor::WorldSize; i++)
		{
			if (i != 0)
			{
				MPI_Send(&unitSizes[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			}
		}

		// Wait for slaves array size received code.

		WaitForSlaves(SLAVE_ARRAY_SIZE_RECEIVED_CODE, "M: slave sent incorrect code\n");

		// Send array parts.

		Logger::Log("M: sending arrays\n"); // //cout << "M: sending arrays\n";

		int startIndex = unitSizes[0];

		for (int i = 1; i < DistributedArrayProcessor::WorldSize; i++)
		{
			MPI_Send(array + startIndex, unitSizes[i], MPI_INT, i, 0, MPI_COMM_WORLD);

			startIndex += unitSizes[i];
		}

		// Wait for slaves array received code.

		WaitForSlaves(SLAVE_ARRAY_RECEIVED_CODE, "M: slave sent incorrect code\n");

		// Start working.

		Logger::Log("M: start each slave\n");

		buffer = SLAVE_START_WORKING_CODE;

		for (int i = 1; i < DistributedArrayProcessor::WorldSize; i++)
		{
			MPI_Send(&buffer, 1, MPI_BYTE, i, 0, MPI_COMM_WORLD);
		}

	}

	// Do own work.

	Logger::Log("M: array size is %d\n", DebugPrint, unitSizes[0]);

	int myResult = 0;

	if (ThreadsCount > 1)
	{
		int *threadUnitSizes = new int[ThreadsCount];

		Utils::SplitArray(threadUnitSizes, ThreadsCount, unitSizes[0]);

		omp_set_num_threads(ThreadsCount);
		#pragma omp parallel for reduction(+:myResult)
		for (int i = 0; i < ThreadsCount; i++)
		{
			int startIndex = 0;

			for (int j = 0; j < i; j++)
			{
				startIndex += threadUnitSizes[j];
			}

			myResult += ProcessData(array + startIndex, threadUnitSizes[i]);
		}
	}
	else
	{
		myResult += ProcessData(array, unitSizes[0]);
	}

	Logger::Log("M: my work is done\n");

	// Calculate data itself.

	int masterResult = 0; //ProcessData(array, ArraySize);

	// Wait for results.

	int *results = new int[DistributedArrayProcessor::WorldSize];
	int slavesResult = myResult;

	for (int i = 1; i < DistributedArrayProcessor::WorldSize; i++)
	{
		int receivedValue = 0;
		MPI_Status status;

		MPI_Recv(&receivedValue, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
		results[status.MPI_SOURCE] = receivedValue;
		slavesResult += receivedValue;
	}

	Logger::Log("M: results received!\n"); // //cout << "M: results received!\n";

	double endTime = omp_get_wtime();

	Logger::Log("M: work time %f\n", ProductionPrint, endTime - startTime);
	Logger::Log("arr: %d pr: %d th: %d t: %f\n", StatisticsPrint, ArraySize, DistributedArrayProcessor::WorldSize, Master::ThreadsCount, endTime - startTime);

	Logger::Log("Array size: %d, World size %d, Time: %d\n", DebugPrint, ArraySize, DistributedArrayProcessor::WorldSize, 5.5);

	// Print results.

	Logger::Log("M: control result is \t%d\n", DebugPrint, masterResult);
	Logger::Log("M: slaves result is \t%d\n", DebugPrint, slavesResult);

	if (masterResult == slavesResult)
	{
		Logger::Log("M: results are correct!\n", DebugPrint); 
	}
	else
	{
		Logger::Log("M: results are NOT correct!\n"); 
	}

	Logger::Log("M: done!\n"); 
}

void Master::WaitForSlaves(byte requiredCode, char* errorMessage)
{
	byte *groupBuffer = new byte[DistributedArrayProcessor::WorldSize];

	for (int i = 1; i < DistributedArrayProcessor::WorldSize; i++)
	{
		MPI_Recv(&groupBuffer[i], 1, MPI_BYTE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		if (groupBuffer[i] != requiredCode)
		{
			Logger::Log(errorMessage);
		}
	}

	delete[] groupBuffer;
}

void Master::PrintArray(int* array, int size)
{
	for (int i = 0; i < size; i++)
	{
		Logger::Log("%d ", DebugPrint, array[i]);
	}
	Logger::Log("\n");
}
