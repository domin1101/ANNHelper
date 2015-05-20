#include "AbstractBackpropagationLearningRule.hpp"
#include "Teacher.hpp"
#include "TopologicalOrder.hpp"
#include "AbstractTeachingLesson.hpp"
#include "NeuralNetwork.hpp"
#include "LayeredNetwork.hpp"
#include "AbstractNeuron.hpp"
#include "AbstractNetworkTopology.hpp"
#include "StandardNeuron.hpp"
#include "Edge.hpp"
#include <iostream>
#include <iomanip>
#include <exception>

AbstractBackpropagationLearningRule::AbstractBackpropagationLearningRule(AbstractBackpropagationLearningRuleOptions *options_) 
	: AbstractLearningRule(options_)
{

}


void AbstractBackpropagationLearningRule::initializeLearningAlgoritm(NeuralNetwork &neuralNetwork, Teacher &teacher)
{
	// Check if all given parameters are correct
	if (!dynamic_cast<LayeredNetwork*>(neuralNetwork.getNetworkTopology()))
		throw std::invalid_argument("The given neuralNetwork has to contain a layeredNetworkTopology");

	// Create a vector which will contain all delta values of the neurons in the output layer
	for (std::vector<std::vector<AbstractNeuron*>>::iterator layer = neuralNetwork.getNetworkTopology()->getNeurons()->begin(); layer != neuralNetwork.getNetworkTopology()->getNeurons()->end(); layer++)
	{
		deltaVectorOutputLayer.push_back(std::vector<float>((*layer).size()));
	}	
}

float AbstractBackpropagationLearningRule::calculateDeltaWeightFromEdge(Edge* edge, int lessonIndex, int layerIndex, int neuronIndex, int edgeIndex, int layerCount, int neuronsInLayerCount, std::vector<float>* errorvector)
{
	// If its the last layer
	if (layerIndex == layerCount - 1)
	{		
		// Calculate the gradient
		// gradient = - Output(prevNeuron) * deltaValue
		float gradient = -1 * edge->getPrevNeuron()->getActivation() * deltaVectorOutputLayer[layerIndex][neuronIndex];	
		if (gradient == std::numeric_limits<double>::infinity())
			gradient = gradient;
		return gradient;
	}
	else if (neuronIndex + 1 < neuronsInLayerCount) // If its not the last layer and not a BiasNeuron
	{		
		// Calculate the gradient
		// gradient = - Output(prevNeuron) * deltaValue
		float gradient = -1 * edge->getPrevNeuron()->getActivation() * deltaVectorOutputLayer[layerIndex][neuronIndex];
								
		if (gradient == std::numeric_limits<double>::infinity())
			gradient = gradient;
		return gradient;
	}	
	return 0;
}

void AbstractBackpropagationLearningRule::initializeNeuronWeightCalculation(StandardNeuron* neuron, int lessonIndex, int layerIndex, int neuronIndex, int layerCount, int neuronsInLayerCount, std::vector<float>* errorvector)
{
	// If its the last layer
	if (layerIndex == layerCount - 1)
	{					
		// Compute the delta value: activationFunction'(netInput) * errorValue
		deltaVectorOutputLayer[layerIndex][neuronIndex] = (neuron->executeDerivationOnActivationFunction(neuron->getNetInput()) + getOptions()->flatSpotEliminationFac) * (*errorvector)[neuronIndex];
	}
	else
	{
		std::list<Edge*>* afferentEdges = (dynamic_cast<StandardNeuron*>(neuron))->getAfferentEdges();
		std::list<Edge*>* efferentEdges = neuron->getEfferentEdges();

		// Calc the nextLayerErrorValueFactor
		float nextLayerErrorValueFactor = 0;

		// Go through all efferentEdges of the actual neuron and add to the nextLayerErrorValueFactor: deltaValue * edgeWeight
		int efferentEdgeIndex = 0;
		for (std::list<Edge*>::iterator efferentEdge = efferentEdges->begin(); efferentEdge != efferentEdges->end(); efferentEdge++, efferentEdgeIndex++)
		{
			nextLayerErrorValueFactor += deltaVectorOutputLayer[layerIndex + 1][efferentEdgeIndex] * (*efferentEdge)->getWeight();
		}

		// Compute the delta value:  activationFunction'(netInput) * nextLayerErrorValueFactor
		deltaVectorOutputLayer[layerIndex][neuronIndex] = (neuron->executeDerivationOnActivationFunction(neuron->getNetInput()) + getOptions()->flatSpotEliminationFac) * nextLayerErrorValueFactor;					
		if (deltaVectorOutputLayer[layerIndex][neuronIndex] == std::numeric_limits<double>::infinity())
			layerIndex = layerIndex;
	}	
}

AbstractBackpropagationLearningRuleOptions* AbstractBackpropagationLearningRule::getOptions()
{
	return static_cast<AbstractBackpropagationLearningRuleOptions*>(options.get());
}

AbstractActivationOrder* AbstractBackpropagationLearningRule::getNewActivationOrder()
{
	return new TopologicalOrder();
}

void AbstractBackpropagationLearningRule::initializeTry(NeuralNetwork &neuralNetwork, Teacher &teacher)
{
	if (options->changeWeightsBeforeLearning)
	{
		// Randomize all weights
		neuralNetwork.getNetworkTopology()->randomizeWeights(options->minRandomWeightValue, options->maxRandomWeightValue);
	}
}
