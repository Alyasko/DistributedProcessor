#include "Worker.h"

#include "stdio.h"
#include "mpi.h"

#include "DistributedArrayProcessor.h"

Worker::Worker()
{
}


Worker::~Worker()
{
}

void Worker::Run()
{
	int worldSize = 0;
	int rank = 0;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

	DistributedArrayProcessor *processor = new DistributedArrayProcessor(worldSize, rank);
	processor->Start();
}
