#pragma once

#ifndef _WEIGHTEDSUMFUNCTION_H_
#define _WEIGHTEDSUMFUNCTION_H_

#include "InputFunction.hpp"

class WeightedSumFunction : InputFunction
{
private:
public:
	WeightedSumFunction();
	float execute(std::list<Edge*> input);
};

#endif
