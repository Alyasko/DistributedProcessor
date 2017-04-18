#include "Slave.h"
#include <iostream>
#include <omp.h>
#include <mpi.h>

#include "DistributedArrayProcessor.h"
#include "Master.h"
#include "Utils.h"

#include "Logger.h"

using namespace std;

Slave::Slave() {
	
}


Slave::~Slave()
{
	
}

void Slave::Run()
{
	Logger::Log("\n--- Slave started ---\n");

	double startTime = omp_get_wtime();

	// Send presence.

	byte buffer = SLAVE_PRESENCE_CODE;
	MPI_Send(&buffer, 1, MPI_BYTE, 0, 0, MPI_COMM_WORLD);

	Logger::Log("S: presence sent\n");

	// Wait for command to listen array size.

	MPI_Recv(&buffer, 1, MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	if (buffer == SLAVE_LISTEN_FOR_ARRAY_SIZE)
	{
		Logger::Log("S: waiting for array size...\n"); // //cout << "S: waiting for array size...\n";

	}
	else
	{
		Logger::Log("S: broadcast error\n"); // //cout << "S: broadcast error\n";
	}


	// Wait for array size.

	int arraySize = 0;
	MPI_Recv(&arraySize, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	Logger::Log("S: size is %d\n", DebugPrint, arraySize); // //cout << "S: size is " << arraySize << "\n";

	// Send array size received code.

	buffer = SLAVE_ARRAY_SIZE_RECEIVED_CODE;
	MPI_Send(&buffer, 1, MPI_BYTE, 0, 0, MPI_COMM_WORLD);


	// Wait for array.

	int *array = new int[arraySize];
	MPI_Recv(array, arraySize, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	Logger::Log("S: array received\n"); // //cout << "S: array received\n";

	// Send array received code.

	buffer = SLAVE_ARRAY_RECEIVED_CODE;
	MPI_Send(&buffer, 1, MPI_BYTE, 0, 0, MPI_COMM_WORLD);

	// Wait for start.

	MPI_Recv(&buffer, 1, MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	if (buffer == SLAVE_START_WORKING_CODE)
	{
		Logger::Log("S: working...\n"); // //cout << "S: working...\n";

		/*for(int i = 0; i < arraySize; i++)
		{
		//cout << array[i] << " ";
		}

		Logger::Log("\n"); // //cout << "\n";*/

		// Processing.

		int result = 0;

		if (arraySize > 0)
		{
			if(Master::ThreadsCount > 1)
			{
				int *unitSizes = new int[Master::ThreadsCount];

				Utils::SplitArray(unitSizes, Master::ThreadsCount, arraySize);

				omp_set_num_threads(Master::ThreadsCount);
				#pragma omp parallel for reduction(+:result)
				for(int i = 0; i < Master::ThreadsCount; i++)
				{
					int startIndex = 0;

					for(int j = 0; j < i; j++)
					{
						startIndex += unitSizes[j];
					}

					result += ProcessData(array + startIndex, unitSizes[i]);
				}
			} 
			else
			{
				result = ProcessData(array, arraySize);
			}
		}

		Logger::Log("S: ready\n"); // //cout << "S: ready\n";

		// End of processing.

		MPI_Send(&result, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

		double endTime = omp_get_wtime();

		Logger::Log("S: work time %f\n", ProductionPrint, endTime - startTime);
	}
	else
	{
		Logger::Log("S: incorrect start working code received\n"); // //cout << "S: incorrect start working code received\n";
	}
}
