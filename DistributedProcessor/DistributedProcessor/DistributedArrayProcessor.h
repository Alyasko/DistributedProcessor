#pragma once

typedef unsigned char byte;

class DistributedArrayProcessor
{
public:
	DistributedArrayProcessor(int worldSize, int rank);
	~DistributedArrayProcessor();
	void Start();
	static int WorldSize;
	static int Rank;
	static bool DistributedArrayProcessor::DebugMode;
private:
	void CheckDebugMode();
	void Execute();
};

