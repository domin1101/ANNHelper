// Includes
#include "Function/RandomFunction/RankBasedRandomFunction.hpp"
// Library includes
#include <random>
#include <stdexcept>

namespace LightBulb
{
	int RankBasedRandomFunction::execute(int elementCount)
	{
		double probabilitySum = 0;
		for (int i = 1; i <= elementCount; i++)
		{
			probabilitySum += 1 + (selectivePressure - 1) * exp(-1.0 * (selectivePressure - 1) * i / elementCount);
		}

		double randomLine = randomGenerator->randDouble() * probabilitySum;
		double partialSum = 0;
		for (int i = 1; i <= elementCount; i++)
		{
			partialSum += 1 + (selectivePressure - 1) * exp(-1.0 * (selectivePressure - 1) * i / elementCount);
			if (partialSum >= randomLine)
			{
				return i - 1;
			}
		}
		throw std::logic_error("No element has been selected!");
	}

	RankBasedRandomFunction::RankBasedRandomFunction(double selectivePressure_)
	{
		selectivePressure = selectivePressure_;
	}
}
