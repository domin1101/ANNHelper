#pragma once

#ifndef _TRAININGCONTROLLER_H_
#define _TRAININGCONTROLLER_H_
#include <vector>
#include <NeuralNetwork/AbstractNeuralNetwork.hpp>
#include <Teaching/AbstractTrainingPlan.hpp>

// Includes

// Forward declarations
class TrainingWindow;

class TrainingController
{
private:
	std::vector<AbstractNeuralNetwork*> neuralNetworks;
	std::vector<AbstractTrainingPlan*> trainingPlanPatterns;
	std::vector<AbstractTrainingPlan*> trainingPlans;
	TrainingWindow* window;
public:
	TrainingController(TrainingWindow* window_);
	std::vector<AbstractNeuralNetwork*>* getNeuralNetworks();
	std::vector<AbstractTrainingPlan*>* getTrainingPlanPatterns();
	std::vector<AbstractTrainingPlan*>* getTrainingPlans();
	void startTrainingPlanPattern(int trainingPlanPatternIndex, int neuralNetworkIndex);
	int getIndexOfNeuralNetwork(AbstractNeuralNetwork* network);
	int getIndexOfTrainingPlanPattern(AbstractTrainingPlan* trainingPlanPattern);
};

#endif
