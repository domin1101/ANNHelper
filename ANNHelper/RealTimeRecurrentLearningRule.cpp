#include "RealTimeRecurrentLearningRule.hpp"
#include "Teacher.hpp"
#include "SynchronousOrder.hpp"
#include "AbstractTeachingLesson.hpp"
#include "NeuralNetwork.hpp"
#include "LayeredNetwork.hpp"
#include "AbstractNeuron.hpp"
#include "AbstractNetworkTopology.hpp"
#include "StandardNeuron.hpp"
#include "Edge.hpp"
#include "ResilientLearningRateHelper.hpp"

RealTimeRecurrentLearningRule::RealTimeRecurrentLearningRule(RealTimeRecurrentLearningRuleOptions& options_) 
	: AbstractLearningRule(new RealTimeRecurrentLearningRuleOptions(options_))
{
	
}


void RealTimeRecurrentLearningRule::initializeLearningAlgoritm(NeuralNetwork &neuralNetwork, Teacher &teacher, AbstractActivationOrder &activationOrder)
{
	// Remove all output values
	outputValuesInTime.clear();
	// Resize it to the new max time steps
	outputValuesInTime.resize(10);
	// Remove all output values
	netInputValuesInTime.clear();
	// Resize it to the new max time steps
	netInputValuesInTime.resize(10);

}


float RealTimeRecurrentLearningRule::calculateDeltaWeightFromEdge(Edge* edge, int lessonIndex, int layerIndex, int neuronIndex, int edgeIndex, int layerCount, int neuronsInLayerCount, ErrorMap_t* errormap)
{
	float outputNeuronsDependency = 0;
	for (ErrorMap_t::iterator outputNeurons = errormap->begin(); outputNeurons != errormap->end(); outputNeurons++)
	{
		for (std::map<StandardNeuron*, float>::iterator outputNeuron = outputNeurons->second.begin(); outputNeuron != outputNeurons->second.end(); outputNeuron++)
		{
			outputNeuronsDependency += outputNeuron->second * getDynamicSystemValueOfEdgeAtTime(edge, outputNeuron->first, outputNeurons->first, true, errormap);
		}
	}
	if (getOptions()->teacherForcing)
	{
		if (currentTeachingInputMap->count(currentTimeStep) != 0)
		{
			for (std::map<StandardNeuron*, float>::iterator outputNeuron = (*currentTeachingInputMap)[currentTimeStep].begin();  outputNeuron != (*currentTeachingInputMap)[currentTimeStep].end(); outputNeuron++)
			{
				outputNeuron->first->injectActivation(outputNeuron->first->getActivation() + outputNeuron->second);
			}
		}
	}
	float returnValue = getOptions()->learningRate * outputNeuronsDependency;
	return returnValue;
}

void RealTimeRecurrentLearningRule::initializeNeuronWeightCalculation(StandardNeuron* neuron, int lessonIndex, int layerIndex, int neuronIndex, int layerCount, int neuronsInLayerCount, ErrorMap_t* errormap)
{
	
}


AbstractActivationOrder* RealTimeRecurrentLearningRule::getNewActivationOrder(NeuralNetwork &neuralNetwork)
{
	return new SynchronousOrder();
}

void RealTimeRecurrentLearningRule::adjustWeight(Edge* edge, float deltaWeight)
{
	edge->setWeight(edge->getWeight() + deltaWeight);
}

void RealTimeRecurrentLearningRule::printDebugOutput()
{

}

bool RealTimeRecurrentLearningRule::learningHasStopped()
{
	return false;
}

RealTimeRecurrentLearningRuleOptions* RealTimeRecurrentLearningRule::getOptions()
{
	return static_cast<RealTimeRecurrentLearningRuleOptions*>(options.get());
}

void RealTimeRecurrentLearningRule::initializeTry(NeuralNetwork &neuralNetwork, Teacher &teacher)
{
	// If we can change the weights before learning
	if (options->changeWeightsBeforeLearning)
	{
		// Randomize all weights
		neuralNetwork.getNetworkTopology()->randomizeWeights(options->minRandomWeightValue, options->maxRandomWeightValue);
	}
}

void RealTimeRecurrentLearningRule::initializeAllWeightAdjustments(NeuralNetwork &neuralNetwork)
{
	for (std::map<Edge*, std::map<StandardNeuron*, std::map<int, std::pair<float, bool>>>>::iterator edge = dynamicSystemCache.begin(); edge != dynamicSystemCache.end(); edge++)
	{
		for (std::map<StandardNeuron*, std::map<int, std::pair<float, bool>>>::iterator neuron = edge->second.begin(); neuron != edge->second.end(); neuron++)
		{
			for (std::map<int, std::pair<float, bool>>::iterator time = neuron->second.begin(); time != neuron->second.end(); time++)
			{
				time->second.second = false;
			}
		}
	}
}

void RealTimeRecurrentLearningRule::initializeTeachingLesson(NeuralNetwork &neuralNetwork, AbstractTeachingLesson &teachingLesson)
{
	if (getOptions()->teacherForcing)
		currentTeachingInputMap = teachingLesson.getTeachingInputMap(neuralNetwork);
}

float RealTimeRecurrentLearningRule::getDynamicSystemValueOfEdgeAtTime(Edge* edge, StandardNeuron* neuron, int time, bool isInFirstCalculationLayer, ErrorMap_t* errormap)
{
	if (!dynamicSystemCache[edge][neuron][time].second)
	{
		if (time != 0 && (isInFirstCalculationLayer || !getOptions()->teacherForcing || currentTeachingInputMap->count(time) == 0 || (*currentTeachingInputMap)[time].count(neuron) == 0))
		{
			float previousValuesSum = 0;

			for (std::list<Edge*>::iterator afferentEdge = neuron->getAfferentEdges()->begin(); afferentEdge != neuron->getAfferentEdges()->end(); afferentEdge++)
			{
				StandardNeuron* prevNeuron = dynamic_cast<StandardNeuron*>((*afferentEdge)->getPrevNeuron());
				if (prevNeuron)
					previousValuesSum += (*afferentEdge)->getWeight() * getDynamicSystemValueOfEdgeAtTime(edge, prevNeuron, time - 1, false, errormap);
			}

			dynamicSystemCache[edge][neuron][time].first = neuron->executeDerivationOnActivationFunction(netInputValuesInTime[time][neuron]) * (previousValuesSum + (edge->getNextNeuron() == neuron ? outputValuesInTime[time - 1][edge->getPrevNeuron()] : 0));
		}
		else if (time == 0)
			dynamicSystemCache[edge][neuron][time].first = 0;
		else
			dynamicSystemCache[edge][neuron][time].first = 0;
		dynamicSystemCache[edge][neuron][time].second = true;
	}
	return dynamicSystemCache[edge][neuron][time].first;
}

bool RealTimeRecurrentLearningRule::configureNextErroMapCalculation(int* nextStartTime, int* nextTimeStepCount, AbstractTeachingLesson& teachingLesson)
{
	*nextTimeStepCount = 1;
	(*nextStartTime)++;
	currentTimeStep = *nextStartTime;
	return (*nextStartTime < teachingLesson.getMaxTimeStep() + 1);
}

std::vector<std::map<AbstractNeuron*, float>>* RealTimeRecurrentLearningRule::getOutputValuesInTime()
{
	return &outputValuesInTime;
}

std::vector<std::map<AbstractNeuron*, float>>* RealTimeRecurrentLearningRule::getNetInputValuesInTime()
{
	return &netInputValuesInTime;
}