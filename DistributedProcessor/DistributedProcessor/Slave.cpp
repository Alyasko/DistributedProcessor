#include "Slave.h"
#include <iostream>
#include <omp.h>
#include <mpi.h>

#include "DistributedArrayProcessor.h"

using namespace std;

Slave::Slave() {
	
}


Slave::~Slave()
{
	
}

void Slave::Run()
{
	//cout << "\n--- Slave started ---\n";

	double startTime = omp_get_wtime();

	// Send presence.

	byte buffer = SLAVE_PRESENCE_CODE;
	MPI_Send(&buffer, 1, MPI_BYTE, 0, 0, MPI_COMM_WORLD);

	//cout << "S: presence sent\n";

	// Wait for command to listen array size.

	MPI_Recv(&buffer, 1, MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	if (buffer == SLAVE_LISTEN_FOR_ARRAY_SIZE)
	{
		//cout << "S: waiting for array size...\n";
	}
	else
	{
		//cout << "S: broadcast error\n";
	}


	// Wait for array size.

	int arraySize = 0;
	MPI_Recv(&arraySize, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	//cout << "S: size is " << arraySize << "\n";


	// Send array size received code.

	buffer = SLAVE_ARRAY_SIZE_RECEIVED_CODE;
	MPI_Send(&buffer, 1, MPI_BYTE, 0, 0, MPI_COMM_WORLD);


	// Wait for array.

	int *array = new int[arraySize];
	MPI_Recv(array, arraySize, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	//cout << "S: array received\n";

	// Send array received code.

	buffer = SLAVE_ARRAY_RECEIVED_CODE;
	MPI_Send(&buffer, 1, MPI_BYTE, 0, 0, MPI_COMM_WORLD);

	// Wait for start.

	MPI_Recv(&buffer, 1, MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	if (buffer == SLAVE_START_WORKING_CODE)
	{
		//cout << "S: working...\n";

		/*for(int i = 0; i < arraySize; i++)
		{
		//cout << array[i] << " ";
		}

		//cout << "\n";*/

		// Processing.

		int result = 0;

		result = ProcessData(array, arraySize);

		//cout << "S: ready\n";

		// End of processing.

		MPI_Send(&result, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

		double endTime = omp_get_wtime();
		//cout << "S: work time " << endTime - startTime << endl;
	}
	else
	{
		//cout << "S: incorrect start working code received\n";
	}
}
