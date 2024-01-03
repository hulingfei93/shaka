#ifndef _RANDOM_NUMBER_GENERATOR_H_
#define _RANDOM_NUMBER_GENERATOR_H_

#include <stdlib.h>
#include <limits.h>
#include <time.h>

class RandomNumberGenerator
{
public:
	RandomNumberGenerator();
	int GenerateInteger(int minValue, int maxValue);
};

RandomNumberGenerator::RandomNumberGenerator()
{
	time_t rawTime;
	time(&rawTime);
	srand(rawTime);
}

int RandomNumberGenerator::GenerateInteger(int minValue, int maxValue)
{
	return static_cast<int>(static_cast<double>(rand()) / static_cast<double>(RAND_MAX)* (maxValue - minValue));
}

#endif