#pragma once

typedef unsigned char byte;

class DistributedArrayProcessor
{
public:
	DistributedArrayProcessor(int worldSize, int rank);
	~DistributedArrayProcessor();
	void Start();
	int WorldSize;
	int Rank;
private:
	void StartSlave();
	void StartMaster();

	void WaitForSlaves(byte requiredCode, char* errorMessage);
	void SendToAll(byte *data);
	int ProcessData(int *array, int size);
};

