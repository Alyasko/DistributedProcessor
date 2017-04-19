#include "DistributedArrayProcessor.h"
#include <iostream>
#include <mpi.h>
#include <omp.h>
#include "Windows.h"

#include "ArrayGenerator.h"
#include "Slave.h"
#include "Master.h"
#include "Logger.h"

using namespace std;

int DistributedArrayProcessor::WorldSize;
int DistributedArrayProcessor::Rank;
bool DistributedArrayProcessor::DebugMode;

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
	CheckDebugMode();
	Execute();
}

void DistributedArrayProcessor::CheckDebugMode()
{
	if (DebugMode == true)
	{
		if (Rank == 0)
		{
			uint32_t pid = GetCurrentProcessId();
			Logger::Log("M: My PID is %d\n", DebugPrint, pid);

			int number = 0;

			Logger::Log("Please input number: \n");
			cin >> number;
		}

		MPI_Barrier(MPI_COMM_WORLD);
	}
}

void DistributedArrayProcessor::Execute()
{
	if (Rank != 0)
	{
		Slave *slave = new Slave();
		slave->Run();
	}
	else
	{
		Master *master = new Master();
		master->Run();
	}
}
