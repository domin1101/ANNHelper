#pragma once

#ifndef _POSITION_H_
#define _POSITION_H_

// Library Includes

// Includes
#include "Learning/Evolution/AbstractSimpleEvolutionObject.hpp"

// Forward declarations
class EvolutionLearningRule;
class FunctionSimulator;

class Position : public AbstractSimpleEvolutionObject
{
protected:
	FunctionSimulator* functionSimulator;
	std::vector<float> position;
	NeuralNetworkIO<double> getNNInput();
	void interpretNNOutput(EvolutionLearningRule& learningRule, NeuralNetworkIO<double>* output);
public:	
	Position(FunctionSimulator* functionSimulator_);
	~Position();
	std::vector<float> getPosition();
};
#endif