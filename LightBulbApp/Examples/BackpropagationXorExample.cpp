#include "BackpropagationXorExample.hpp"
#include <NetworkTopology/LayeredNetwork.hpp>
#include <NeuralNetwork/NeuralNetwork.hpp>
#include <Learning/BackpropagationLearningRule.hpp>
#include <NeuronFactory/DifferentNeuronDescriptionFactory.hpp>
#include <Function/WeightedSumFunction.hpp>
#include <Teaching/TeachingLessonBooleanInput.hpp>
#include <Teaching/Teacher.hpp>
#include <ActivationOrder/TopologicalOrder.hpp>
#include <Function/FermiFunction.hpp>
#include <Neuron/NeuronDescription.hpp>
#include <Graphics/NeuralNetworkResultChart.hpp>


void BackpropagationXorExample::run()
{
	BackpropagationLearningRuleOptions options;
	options.enableDebugOutput = true;
	options.maxTotalErrorValue = 4;
	options.maxIterationsPerTry = 1000000;
	options.totalErrorGoal = 0.001f;
	options.maxTries = 1000;
	options.weightDecayFac = 0;
	options.learningRate = 0.1;
	options.momentum = 0;
	options.resilientLearningRate = false;
	BackpropagationLearningRule learningRule(options);

	Teacher teacher;
	for (int i = 0; i < 2; i += 1)
	{
		for (int l = 0; l < 2; l += 1)
		{
			std::vector<std::vector<double>> teachingPattern(1, std::vector<double>(2));
			NeuralNetworkIO<bool>* teachingInput = new NeuralNetworkIO<bool>(1);

			teachingPattern[0][0] = i;
			teachingPattern[0][1] = l;
			(*teachingInput).set(0, 0, (i != l));
			teacher.addTeachingLesson(new TeachingLessonBooleanInput(teachingPattern, teachingInput));
		}
	}

	bool success = learningRule.doLearning(*network, teacher);
	finished();
}

AbstractNeuralNetwork* BackpropagationXorExample::createNeuralNetwork()
{
	LayeredNetworkOptions layeredNetworkOptions;
	layeredNetworkOptions.descriptionFactory = new DifferentNeuronDescriptionFactory(new NeuronDescription(new WeightedSumFunction(), new FermiFunction(1)), new NeuronDescription(new WeightedSumFunction(), new FermiFunction(1)));
	layeredNetworkOptions.neuronsPerLayerCount = std::vector<unsigned int>(3);
	layeredNetworkOptions.neuronsPerLayerCount[0] = 2;
	layeredNetworkOptions.neuronsPerLayerCount[1] = 2;
	layeredNetworkOptions.neuronsPerLayerCount[2] = 1;

	LayeredNetwork* layeredNetwork = new LayeredNetwork(layeredNetworkOptions);

	return new NeuralNetwork(layeredNetwork);
}

void BackpropagationXorExample::tryToPause()
{
	shouldPause = true;
}

std::string BackpropagationXorExample::getName()
{
	return "Backpropagation xor example";
}

std::string BackpropagationXorExample::getDescription()
{
	return "Trains a network to simulate the xor function!";
}

std::string BackpropagationXorExample::getLearningRateName()
{
	return "Backpropagation";
}

AbstractTrainingPlan* BackpropagationXorExample::getCopy()
{
	return new BackpropagationXorExample();
}

int BackpropagationXorExample::getRequiredInputSize()
{
	return 2;
}

int BackpropagationXorExample::getRequiredOutputSize()
{
	return 1;
}
