#include "stdio.h"
#include "Worker.h"
#include "mpi.h"
#include "Master.h"

int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);

	// Default.
	Master::ArraySize = 100;
	Master::ThreadsCount = 1;

	if(argc >= 2)
	{
		sscanf(argv[1], "%d", &Master::ArraySize);
	}

	if(argc >= 3)
	{
		sscanf(argv[2], "%d", &Master::ThreadsCount);
	}

	Worker *worker = new Worker();
	worker->Run();

	MPI_Finalize();

	return 0;
}
