#pragma once

#ifndef _ABSTRACTTEACHINGLESSON_H_
#define _ABSTRACTTEACHINGLESSON_H_

// Library Includes
#include <vector>

// Forward declarations
class NeuralNetwork;
class AbstractActivationOrder;
class AbstractActivationFunction;
class NeuralNetworkIO;

class AbstractTeachingLesson
{
public:
	// Put the teachingPattern into the neuralNetwork and refresh the network
	std::unique_ptr<NeuralNetworkIO> tryLesson(NeuralNetwork &neuralNetwork, AbstractActivationOrder &activationOrder);
	// This method should return a float vector of the teachingInput 
	virtual std::vector<float>* getTeachingInput(AbstractActivationFunction* activationFunction) = 0;
	// This method should return a float vector of the teachingPattern
	virtual NeuralNetworkIO* getTeachingPattern() = 0;
	virtual ~AbstractTeachingLesson() {}
	// Calculate the Errorvector
	std::unique_ptr<std::vector<float>> getErrorvector(NeuralNetwork &neuralNetwork, AbstractActivationOrder &activationOrder);
	// Calculate the euclidient distance
	float getEuclidienDistance(NeuralNetwork &neuralNetwork, AbstractActivationOrder &activationOrder);
	// Calculate the RootMeanSquare
	float getRMS(NeuralNetwork &neuralNetwork, AbstractActivationOrder &activationOrder);
	// Calculate the specific error
	float getSpecificError(NeuralNetwork &neuralNetwork, AbstractActivationOrder &activationOrder);
};

#endif

