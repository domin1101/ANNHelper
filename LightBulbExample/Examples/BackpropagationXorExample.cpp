#include "BackpropagationXorExample.hpp"
#include <NetworkTopology/FeedForwardNetworkTopology.hpp>
#include <NeuralNetwork/NeuralNetwork.hpp>
#include <NeuronDescription/DifferentNeuronDescriptionFactory.hpp>
#include <Function/InputFunction/WeightedSumFunction.hpp>
#include <Teaching/TeachingLessonBooleanInput.hpp>
#include <Teaching/Teacher.hpp>
#include <Function/ActivationFunction/FermiFunction.hpp>
#include <NeuronDescription/NeuronDescription.hpp>
#include <TrainingPlans/Preferences/DoublePreference.hpp>
#include "Learning/Supervised/GradientDescentLearningRule.hpp"
#include "Learning/Supervised/GradientDescentAlgorithms/SimpleGradientDescent.hpp"
#include "Learning/Supervised/GradientDescentAlgorithms/RMSPropLearningRate.hpp"
#include "TrainingPlans/Preferences/GradientDescentLearningRulePreferenceGroup.hpp"

using namespace LightBulb;

BackpropagationXorExample::BackpropagationXorExample()
{
	GradientDescentLearningRuleOptions options;
	options.maxTotalErrorValue = 4;
	options.maxIterationsPerTry = 1000000;
	options.totalErrorGoal = 0.001f;
	options.maxTries = 1000;
	addPreferenceGroup(new GradientDescentLearningRulePreferenceGroup(options));
}

AbstractLearningRule* BackpropagationXorExample::createLearningRate()
{
	teacher.reset(new Teacher());
	for (int i = 0; i < 2; i += 1)
	{
		for (int l = 0; l < 2; l += 1)
		{
			std::vector<std::vector<double>> teachingPattern(1, std::vector<double>(2));
			NeuralNetworkIO<bool>* teachingInput = new NeuralNetworkIO<bool>(1);

			teachingPattern[0][0] = i;
			teachingPattern[0][1] = l;
			(*teachingInput).set(0, 0, (i != l));
			teacher->addTeachingLesson(new TeachingLessonBooleanInput(teachingPattern, teachingInput));
		}
	}
	
	GradientDescentLearningRuleOptions options = createOptions<GradientDescentLearningRuleOptions, GradientDescentLearningRulePreferenceGroup>();
	options.teacher = teacher.get();
	fillDefaultLearningRuleOptions(&options);

	return new GradientDescentLearningRule(options);
}


AbstractNeuralNetwork* BackpropagationXorExample::createNeuralNetwork()
{
	FeedForwardNetworkTopologyOptions networkTopologyOptions;
	networkTopologyOptions.descriptionFactory = new DifferentNeuronDescriptionFactory(new NeuronDescription(new WeightedSumFunction(), new FermiFunction(1)), new NeuronDescription(new WeightedSumFunction(), new FermiFunction(1)));
	networkTopologyOptions.neuronsPerLayerCount = std::vector<unsigned int>(3);
	networkTopologyOptions.neuronsPerLayerCount[0] = 2;
	networkTopologyOptions.neuronsPerLayerCount[1] = 3;
	networkTopologyOptions.neuronsPerLayerCount[2] = 1;

	FeedForwardNetworkTopology* networkTopology = new FeedForwardNetworkTopology(networkTopologyOptions);

	return new NeuralNetwork(networkTopology);
}


std::string BackpropagationXorExample::getDefaultName()
{
	return "Backpropagation xor example";
}

std::string BackpropagationXorExample::getDescription()
{
	return "Trains a network to simulate the xor function!";
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

std::string BackpropagationXorExample::getLearningRuleName()
{
	return GradientDescentLearningRule::getName();
}
