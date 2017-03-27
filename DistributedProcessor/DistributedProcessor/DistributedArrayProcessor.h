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
private:
	void SendToAll(byte *data);
};

