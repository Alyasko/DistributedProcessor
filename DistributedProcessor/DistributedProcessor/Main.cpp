#include "stdio.h"
#include "Worker.h"

int main()
{
	Worker *worker = new Worker();
	worker->Run();

	return 0;
}
