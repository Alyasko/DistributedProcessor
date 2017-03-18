#include "DistributedArrayProcessor.h"
#include <iostream>
#include <mpi.h>

#include "ArrayGenerator.h"

using namespace std;

#define SLAVE_LISTEN_FOR_ARRAY_SIZE 1
#define SLAVE_ARRAY_SIZE_RECEIVED_CODE 2
#define SLAVE_ARRAY_RECEIVED_CODE 3
#define SLAVE_START_WORKING_CODE 4

#define SLAVE_PRESENCE_CODE 100
#define ARRAY_SIZE 100000

DistributedArrayProcessor::DistributedArrayProcessor(int worldSize, int rank)
{
	WorldSize = worldSize;
	Rank = rank;
}

DistributedArrayProcessor::~DistributedArrayProcessor()
{
}

void DistributedArrayProcessor::Start()
{
	if(Rank != 0)
	{
		StartSlave();
	}
	else
	{
		StartMaster();
	}
}

void DistributedArrayProcessor::StartSlave()
{
	cout << "\n--- Slave started ---\n";

	// Send presence.

	byte buffer = SLAVE_PRESENCE_CODE;
	MPI_Send(&buffer, 1, MPI_BYTE, 0, 0, MPI_COMM_WORLD);

	cout << "S: presence sent\n";

	// Wait for command to listen array size.

	MPI_Recv(&buffer, 1, MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	if (buffer == SLAVE_LISTEN_FOR_ARRAY_SIZE) 
	{
		cout << "S: waiting for array size...\n";
	} 
	else
	{
		cout << "S: broadcast error\n";
	}


	// Wait for array size.

	int arraySize = 0;
	MPI_Recv(&arraySize, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	cout << "S: size is " << arraySize << "\n";


	// Send array size received code.

	buffer = SLAVE_ARRAY_SIZE_RECEIVED_CODE;
	MPI_Send(&buffer, 1, MPI_BYTE, 0, 0, MPI_COMM_WORLD);


	// Wait for array.

	int *array = new int[arraySize];
	MPI_Recv(array, arraySize, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	cout << "S: array received\n";

	// Send array received code.

	buffer = SLAVE_ARRAY_RECEIVED_CODE;
	MPI_Send(&buffer, 1, MPI_BYTE, 0, 0, MPI_COMM_WORLD);

	// Wait for start.

	MPI_Recv(&buffer, 1, MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	if(buffer == SLAVE_START_WORKING_CODE)
	{
		cout << "S: working...\n";

		/*for(int i = 0; i < arraySize; i++)
		{
			cout << array[i] << " ";
		}

		cout << "\n";*/

		int result = 1;
		MPI_Send(&result, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	else
	{
		cout << "S: incorrect start working code received\n";
	}
}

void DistributedArrayProcessor::StartMaster()
{
	cout << "\n--- Master started ---\n";

	// Wait for slaves.
	//byte buffer1;
	//MPI_Recv(&buffer1, 1, MPI_BYTE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	WaitForSlaves(SLAVE_PRESENCE_CODE, "M: slave sent incorrect presence code\n");

	cout << "M: everybody is present\n";

	// Slaves listen for array size.

	byte buffer = SLAVE_LISTEN_FOR_ARRAY_SIZE;
	for(int i = 1; i < WorldSize; i++)
	{
		MPI_Send(&buffer, 1, MPI_BYTE, i, 0, MPI_COMM_WORLD);
	}


	// Create array.

	int *array = new int[ARRAY_SIZE];

	ArrayGenerator *generator = new ArrayGenerator();
	generator->MaxValue = 100;
	generator->Generate(array, ARRAY_SIZE);

	if (ARRAY_SIZE < 10) 
	{
		for (int i = 0; i < ARRAY_SIZE; i++)
		{
			cout << array[i] << " ";
		}

		cout << "\n";
	}

	// Broadcast size.

	cout << "M: sending array size\n";

	int arraySizePerSlave = ARRAY_SIZE / (WorldSize - 1);

	for (int i = 1; i < WorldSize; i++)
	{
		MPI_Send(&arraySizePerSlave, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
	}

	// Wait for slaves array size received code.

	WaitForSlaves(SLAVE_ARRAY_SIZE_RECEIVED_CODE, "M: slave sent incorrect code\n");

	// Send array parts.
	
	cout << "M: sending arrays\n";
	
	for(int i = 1; i < WorldSize; i++)
	{
		int countToSend = arraySizePerSlave;
		int startIndex = (i - 1) * arraySizePerSlave;

		if(startIndex + countToSend > ARRAY_SIZE - 1)
		{
			countToSend = ARRAY_SIZE - startIndex;
		}

		MPI_Send(array + startIndex, countToSend, MPI_INT, i, 0, MPI_COMM_WORLD);
	}

	// Wait for slaves array received code.

	WaitForSlaves(SLAVE_ARRAY_RECEIVED_CODE, "M: slave sent incorrect code\n");

	// Start working.

	cout << "M: start each slave\n";

	buffer = SLAVE_START_WORKING_CODE;

	for (int i = 1; i < WorldSize; i++)
	{
		MPI_Send(&buffer, 1, MPI_BYTE, i, 0, MPI_COMM_WORLD);
	}

	// Wait for results.

	int *results = new int[WorldSize];

	for (int i = 1; i < WorldSize; i++)
	{
		int receivedStatus = 0;
		MPI_Status status;

		MPI_Recv(&receivedStatus, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
		results[status.MPI_SOURCE] = receivedStatus;
	}

	cout << "M: results received!\n";

	// Print results.

	for (int i = 1; i < WorldSize; i++)
	{
		cout << "Result " << i << ": " << results[i] << endl;
	}

	cout << "M: done!\n";
}

void DistributedArrayProcessor::WaitForSlaves(byte requiredCode, char* errorMessage)
{
	byte *groupBuffer = new byte[WorldSize];

	for (int i = 1; i < WorldSize; i++)
	{
		MPI_Recv(&groupBuffer[i], 1, MPI_BYTE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		if (groupBuffer[i] != requiredCode)
		{
			cout << errorMessage;
		}
	}

	delete[] groupBuffer;
}

void DistributedArrayProcessor::SendToAll(byte* data)
{
	for(int i = 1; i < Rank; i++)
	{
		MPI_Send(data, 1, MPI_BYTE, i, 0, MPI_COMM_WORLD);
	}
}
