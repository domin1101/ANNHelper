#pragma once

#ifndef _MOCKREUSESELECTOR_H_
#define _MOCKREUSESELECTOR_H_

#include "gmock/gmock.h"
#include <Learning/Evolution/AbstractReuseSelector.hpp>

class MockReuseSelector : public LightBulb::AbstractReuseSelector
{
public:
	MOCK_METHOD1(addObjectToReuse, void (LightBulb::AbstractEvolutionObject&));
	MOCK_METHOD2(selectForReuse, void (int, const std::vector<std::pair<double, LightBulb::AbstractEvolutionObject*>>&));
	MOCK_METHOD3(executeReuseSelection, void (int, const std::vector<std::pair<double, LightBulb::AbstractEvolutionObject*>>&, std::map<LightBulb::AbstractEvolutionObject*, int>&));
	MOCK_CONST_METHOD0(getReuseSelection, const std::vector<LightBulb::AbstractEvolutionObject*>& ());
	MOCK_CONST_METHOD0(clone, AbstractCloneable* ());
};

#endif