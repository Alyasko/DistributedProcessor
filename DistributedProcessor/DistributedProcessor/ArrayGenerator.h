#pragma once
class ArrayGenerator
{
public:
	ArrayGenerator();
	~ArrayGenerator();
	void Generate(int* array, int size);

	int MaxValue;
	bool FrozenValues;
};

