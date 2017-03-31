#include "Master.h"
#include <iostream>
#include <omp.h>
#include "DistributedArrayProcessor.h"
#include <mpi.h>

#include "ArrayGenerator.h"
#include "Utils.h"

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
	//cout << "\n--- Master started ---\n";

	// Wait for slaves.
	//byte buffer1;
	//MPI_Recv(&buffer1, 1, MPI_BYTE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	byte buffer = SLAVE_LISTEN_FOR_ARRAY_SIZE;

	if(DistributedArrayProcessor::WorldSize > 1)
	{
		WaitForSlaves(SLAVE_PRESENCE_CODE, "M: slave sent incorrect presence code\n");

		//cout << "M: everybody is present\n";

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

	ArrayGenerator *generator = new ArrayGenerator();
	generator->FrozenValues = true;
	generator->MaxValue = 10;
	//generator->Generate(array, ArraySize);

	double startTime = omp_get_wtime();

	if (ArraySize < 10)
	{
		for (int i = 0; i < ArraySize; i++)
		{
			//cout << array[i] << " ";
		}

		//cout << "\n";
	}

	// Broadcast size.

	//cout << "M: sending array size\n";

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

		//cout << "M: sending arrays\n";

		int startIndex = unitSizes[0];

		for (int i = 1; i < DistributedArrayProcessor::WorldSize; i++)
		{
			MPI_Send(array + startIndex, unitSizes[i], MPI_INT, i, 0, MPI_COMM_WORLD);

			startIndex += unitSizes[i];
		}

		// Wait for slaves array received code.

		WaitForSlaves(SLAVE_ARRAY_RECEIVED_CODE, "M: slave sent incorrect code\n");

		// Start working.

		//cout << "M: start each slave\n";

		buffer = SLAVE_START_WORKING_CODE;

		for (int i = 1; i < DistributedArrayProcessor::WorldSize; i++)
		{
			MPI_Send(&buffer, 1, MPI_BYTE, i, 0, MPI_COMM_WORLD);
		}

	}

	// Do own work.

	cout << "M: array size is " << unitSizes[0] << endl;

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
			int threadNum = omp_get_thread_num();
			int threadsNum = omp_get_num_threads();

			//cout << "Start index: " << startIndex << endl;
			//cout << threadsNum << endl;
		}
	}
	else
	{
		myResult += ProcessData(array, unitSizes[0]);
	}

	//cout << "M: my work is done\n";

	// Calculate data itself.

	int masterResult = 0;//ProcessData(array, ArraySize);

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

	//cout << "M: results received!\n";

	double endTime = omp_get_wtime();
	//cout << "M: work time " << endTime - startTime << endl;
	cout << "Array size: " << ArraySize << ", World size: " << DistributedArrayProcessor::WorldSize << ", Time: " << endTime - startTime << endl;

	// Print results.

	/*for (int i = 1; i < WorldSize; i++)
	{
	//cout << "Result " << i << ": " << results[i] << endl;
	}*/

	cout << "M: control result is \t" << masterResult << endl;
	cout << "M: slaves result is \t" << slavesResult << endl;

	if (masterResult == slavesResult)
	{
		//cout << "M: results are correct!\n";
	}
	else
	{
		//cout << "M: results are NOT correct!\n";
	}

	//cout << "M: done!\n";
}

void Master::WaitForSlaves(byte requiredCode, char* errorMessage)
{
	byte *groupBuffer = new byte[DistributedArrayProcessor::WorldSize];

	for (int i = 1; i < DistributedArrayProcessor::WorldSize; i++)
	{
		MPI_Recv(&groupBuffer[i], 1, MPI_BYTE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		if (groupBuffer[i] != requiredCode)
		{
			//cout << errorMessage;
		}
	}

	delete[] groupBuffer;
}
