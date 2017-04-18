#pragma once
class Worker
{
public:
	Worker();
	~Worker();

	void Run();
	void CommandLineInit(int argc, char** argv);
};

