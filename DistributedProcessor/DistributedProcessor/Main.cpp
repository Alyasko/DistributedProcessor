#include "stdio.h"
#include "Worker.h"
#include "mpi.h"
#include "Master.h"
#include "LoggerLevel.h"
#include <cstdarg>
#include "Logger.h"


int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);

	// Defaults.
	Master::ArraySize = 1000000;
	Master::ThreadsCount = 1;

	DistributedArrayProcessor::DebugMode = false;

	Logger::LoggerEnabled = true;
	Logger::LoggingLevel = StatisticsPrint;

	Worker *worker = new Worker();
	worker->CommandLineInit(argc, argv);
	worker->Run();

	MPI_Finalize();

	return 0;
}
