#include "DistributedArrayProcessor.h"
#include <iostream>
#include <mpi.h>
#include <omp.h>

#include "ArrayGenerator.h"
#include "Slave.h"
#include "Master.h"

using namespace std;

int DistributedArrayProcessor::WorldSize;
int DistributedArrayProcessor::Rank;

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


void DistributedArrayProcessor::SendToAll(byte* data)
{
	for (int i = 1; i < Rank; i++)
	{
		MPI_Send(data, 1, MPI_BYTE, i, 0, MPI_COMM_WORLD);
	}
}

