#pragma once
#include "RelationalEntity.h"

class Slave : RelationalEntity
{
public:
	Slave();
	~Slave();
	void Run() override;
};

