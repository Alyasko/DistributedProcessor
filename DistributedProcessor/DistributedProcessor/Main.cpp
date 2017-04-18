#include "stdio.h"
#include "Worker.h"
#include "mpi.h"
#include "Master.h"

int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);

	// Defaults.
	Master::ArraySize = 100;
	Master::ThreadsCount = 1;

	Worker *worker = new Worker();
	worker->CommandLineInit(argc, argv);
	worker->Run();

	MPI_Finalize();

	return 0;
}
