#include "stdio.h"
#include "Worker.h"
#include "mpi.h"

int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);

	Worker *worker = new Worker();
	worker->Run();

	MPI_Finalize();

	return 0;
}
