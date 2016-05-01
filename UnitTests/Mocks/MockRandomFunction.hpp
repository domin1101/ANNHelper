#pragma once

#ifndef _MOCKRANDOMFUNCTION_H_
#define _MOCKRANDOMFUNCTION_H_

#include "gmock/gmock.h"
#include <Function/AbstractRandomFunction.hpp>

class MockRandomFunction : public AbstractRandomFunction
{
public:
	MOCK_METHOD1(execute, int (int));
};

#endif
