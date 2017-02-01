// Includes
#include "LightBulb/LightBulbPrec.hpp"
#include "LightBulb/Function/RandomFunction/EqualRandomFunction.hpp"
// Library includes

namespace LightBulb
{
	int EqualRandomFunction::execute(int elementCount) const
	{
		return randomGenerator->randInt(0, elementCount - 1);
	}

	AbstractCloneable* EqualRandomFunction::clone() const
	{
		return new EqualRandomFunction(*this);
	}
}
