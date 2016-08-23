#pragma once

#ifndef _ABSTRACTREINFORCEMENTWORLD_H_
#define _ABSTRACTREINFORCEMENTWORLD_H_

// Library Includes
#include <vector>
#include <memory>

// Includes
#include "IO/UseParentSerialization.hpp"
#include "NeuralNetwork/NeuralNetwork.hpp"

// Forward declarations
struct LayeredNetworkOptions;
struct LearningState;

class AbstractReinforcementWorld
{
private:
	std::vector<double> lastOutput;
	std::vector<double> lastInput;
	bool epsilonGreedly = false;
	double epsilon;
	void buildOutputBuffer();
	void buildNeuralNetwork(LayeredNetworkOptions &options);
protected:
	std::vector<bool> lastBooleanOutput;
	LearningState* learningState;
	// The NN of the object
	std::unique_ptr<NeuralNetwork> neuralNetwork;

	// This method should interpret and act depending on the given NN output
	virtual void interpretNNOutput(std::vector<bool>& output) = 0;

	
public:
	// This method should return the input for the neural network
	virtual void getNNInput(std::vector<double>& input) = 0;

	void doNNCalculation(bool resetInput = true);
	virtual ~AbstractReinforcementWorld() {}
	AbstractReinforcementWorld(LayeredNetworkOptions& options, bool epsilonGreedly = false, double epsilon = 0.1);
	// This method should execute one simulation step.
	// After each simulation step the evolution learning rule will execute each evolution command (selection, mutation, recombination...)
	virtual double doSimulationStep() = 0;
	virtual void initializeForLearning();
	virtual int rateKI() { return 0; };
	NeuralNetwork* getNeuralNetwork();
	void setEpsilon(double newEpsilon);
	double getEpsilon();
	void setLearningState(LearningState* learningState_);
	virtual std::vector<std::string> getDataSetLabels();
	std::vector<bool> getLastBooleanOutput();
	virtual bool isTerminalState() = 0;
};

EMPTY_SERIALIZATION(AbstractReinforcementWorld);

#endif