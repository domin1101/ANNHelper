#pragma once

#ifndef _PONGREINFORCEMENTWORLD_H_
#define _PONGREINFORCEMENTWORLD_H_

// Library Includes

// Include
#include <Learning/Reinforcement/AbstractReinforcementWorld.hpp>
#include <Examples/PongEvolution/AbstractPongWorld.hpp>
#include "IO/UseParentSerialization.hpp"

// Forward declarations

#define DATASET_PONG_RATING "Pong rating"

class PongReinforcementWorld : public AbstractReinforcementWorld, public AbstractPongWorld
{
private:
	int time;
protected:
	void getNNInput(std::vector<double>& input);
	void interpretNNOutput(std::vector<bool>& output);
public:
	PongReinforcementWorld(FeedForwardNetworkTopologyOptions& options_, bool epsilonGreedly = false, double epsilon = 0.1);
	PongReinforcementWorld() = default;
	double doSimulationStep();
	void executeCompareAI();
	void initializeForLearning();
	int rateKI();
	std::vector<std::string> getDataSetLabels();
	bool isTerminalState();
	void setRandomGenerator(AbstractRandomGenerator* randomGenerator_);
};


USE_EXISTING_PARENT_SERIALIZATION(PongReinforcementWorld, AbstractReinforcementWorld, AbstractReinforcementWorld)

#endif
