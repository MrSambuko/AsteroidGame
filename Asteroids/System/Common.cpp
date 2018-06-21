#include <cassert>
#include <random>

#include "Common.hpp"


namespace
{
	std::random_device d;
	std::default_random_engine generator{d()};
}

float generateRandomFloat( float minFloat, float maxFloat )
{
	assert(maxFloat > minFloat);	
	const std::uniform_real_distribution< float > distribution(minFloat, maxFloat);

	return distribution(generator);
}


int generateRandomInt(int minInt, int maxInt)
{
	assert(maxInt > minInt);
	const std::uniform_int_distribution<> distribution(minInt, maxInt);
	return distribution(generator);
}