#include "Master.h"
#include <iostream>
#include <omp.h>
#include "DistributedArrayProcessor.h"
#include <mpi.h>

#include "ArrayGenerator.h"

using namespace std;

#define ARRAY_SIZE 100

int Master::ArraySize;

Master::Master() {

}


Master::~Master()
{
}

void Master::Run()
{
	//cout << "\n--- Master started ---\n";

	double startTime = omp_get_wtime();

	// Wait for slaves.
	//byte buffer1;
	//MPI_Recv(&buffer1, 1, MPI_BYTE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	WaitForSlaves(SLAVE_PRESENCE_CODE, "M: slave sent incorrect presence code\n");

	//cout << "M: everybody is present\n";

	// Slaves listen for array size.

	byte buffer = SLAVE_LISTEN_FOR_ARRAY_SIZE;
	for (int i = 1; i < DistributedArrayProcessor::WorldSize; i++)
	{
		MPI_Send(&buffer, 1, MPI_BYTE, i, 0, MPI_COMM_WORLD);
	}

	// Create array.


	int *array = new int[ArraySize];

	ArrayGenerator *generator = new ArrayGenerator();
	generator->FrozenValues = true;
	generator->MaxValue = 10;
	generator->Generate(array, ArraySize);

	if (ArraySize < 10)
	{
		for (int i = 0; i < ArraySize; i++)
		{
			//cout << array[i] << " ";
		}

		//cout << "\n";
	}

	if (DistributedArrayProcessor::WorldSize == 1)
	{
		// Only master exists.

		//cout << "M: master is alone, doing by itself.\n";

		int result = ProcessData(array, ArraySize);

		//cout << "Result is " << result << endl;

		double endTime = omp_get_wtime();
		//cout << "M: work time " << endTime - startTime << endl;
	}
	else
	{
		// Broadcast size.

		//cout << "M: sending array size\n";

		int worldSize = DistributedArrayProcessor::WorldSize;

		int sizeForUnit = ArraySize / worldSize;
		int remainder = ArraySize % worldSize;

		if(remainder != 0)
		{
			sizeForUnit++;
		}

		int *unitSizes = new int[sizeForUnit];

		for (int i = 0; i < DistributedArrayProcessor::WorldSize; i++)
		{
			int subtract = ArraySize - i * sizeForUnit;
			int unitSize = subtract >= sizeForUnit ? sizeForUnit : subtract;

			unitSizes[i] = unitSize;

			if (i != 0) 
			{
				MPI_Send(&unitSize, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
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

		// Do own work.

		//cout << "M: array size is " << unitSizes[0] << endl;
		int myResult = ProcessData(array, unitSizes[0]);
		//cout << "M: my work is done\n";

		// Calculate data itself.

		int masterResult = 0;//ProcessData(array, ARRAY_SIZE);

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

		// Print results.

		/*for (int i = 1; i < WorldSize; i++)
		{
		//cout << "Result " << i << ": " << results[i] << endl;
		}*/

		//cout << "M: my result is \t" << masterResult << endl;
		//cout << "M: slaves result is \t" << slavesResult << endl;

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
