#pragma once
#include "RelationalEntity.h"
#include "DistributedArrayProcessor.h"

class Master : RelationalEntity
{
public:
	Master();
	~Master();
	void Run() override;
	static int ArraySize;
private:
	void WaitForSlaves(byte requiredCode, char* errorMessage);
};

