#include "TCProblemEvolutionExample.hpp"
#include <Learning/Evolution/RateDifferenceCondition.hpp>
#include <Learning/Evolution/ConstantMutationCommand.hpp>
#include <Learning/Evolution/EvolutionStrategy/MutationAlgorithm.hpp>
#include <Learning/Evolution/BestSelectionCommand.hpp>
#include <Learning/Evolution/ConstantReuseCommand.hpp>
#include <Learning/Evolution/ConstantRecombinationCommand.hpp>
#include <Learning/Evolution/EvolutionStrategy/RecombinationAlgorithm.hpp>
#include <Learning/Evolution/ConstantCreationCommand.hpp>
#include <Learning/Evolution/EvolutionLearningRule.hpp>
#include <Teaching/TeachingLessonBooleanInput.hpp>
#include <Teaching/Teacher.hpp>
#include <NetworkTopology/LayeredNetwork.hpp>
#include <NeuronFactory/DifferentNeuronDescriptionFactory.hpp>
#include <Function/WeightedSumFunction.hpp>
#include <Function/FermiFunction.hpp>
#include <Learning/Evolution/TeachingEvolutionWorld.hpp>
#include <Learning/Evolution/RandomSelector.hpp>
#include <Neuron/NeuronDescription.hpp>
#include <Learning/Evolution/BestReuseSelector.hpp>
#include <Learning/Evolution/ScoreCondition.hpp>

AbstractLearningRule* TCProblemEvolutionExample::createLearningRate()
{

	EvolutionLearningRuleOptions options;
	RateDifferenceCondition* rateDifferenceCondition = new RateDifferenceCondition(0.00001, 50);
	options.exitConditions.push_back(rateDifferenceCondition);
	options.exitConditions.push_back(new ScoreCondition(-0.001));
	ConstantCreationCommand* constantCreationCommand = new ConstantCreationCommand(80);
	options.creationCommands.push_back(constantCreationCommand);
	options.reuseCommands.push_back(new ConstantReuseCommand(new BestReuseSelector(), 1));
	BestSelectionCommand* bestSelectionCommand = new BestSelectionCommand(80);
	options.selectionCommands.push_back(bestSelectionCommand);
	MutationAlgorithm* mutationAlgorithm = new MutationAlgorithm(1.6);
	ConstantMutationCommand* constantMutationCommand = new ConstantMutationCommand(mutationAlgorithm, new RandomSelector(new RankBasedRandomFunction()), 2.0);
	options.mutationsCommands.push_back(constantMutationCommand);
	options.recombinationCommands.push_back(new ConstantRecombinationCommand(new RecombinationAlgorithm(), new RandomSelector(new RankBasedRandomFunction()), 0));
	options.maxTries = 100;
	fillDefaultLearningRuleOptions(&options);

	return new EvolutionLearningRule(options);
}

AbstractEvolutionWorld* TCProblemEvolutionExample::createWorld()
{
	LayeredNetworkOptions* layeredNetworkOptions = new LayeredNetworkOptions();
	layeredNetworkOptions->descriptionFactory = new DifferentNeuronDescriptionFactory(new NeuronDescription(new WeightedSumFunction(), new FermiFunction(1)), new NeuronDescription(new WeightedSumFunction(), new FermiFunction(1)));
	layeredNetworkOptions->neuronsPerLayerCount = std::vector<unsigned int>(3);
	layeredNetworkOptions->neuronsPerLayerCount[0] = 16;
	layeredNetworkOptions->neuronsPerLayerCount[1] = 16;
	layeredNetworkOptions->neuronsPerLayerCount[2] = 1;
	layeredNetworkOptions->useBiasNeuron = true;
	layeredNetworkOptions->enableShortcuts = true;

	std::vector<Eigen::MatrixXi> lernExamples(40, Eigen::MatrixXi(4, 4));

	// Ts
	lernExamples[0] << 
		1, 0, 0, 0,
		1, 1, 1, 0,
		1, 0, 0, 0,
		0, 0, 0, 0;

	lernExamples[1] << 
		0, 0, 0, 0,
		1, 0, 0, 0,
		1, 1, 1, 0,
		1, 0, 0, 0;

	lernExamples[2] <<
		0, 0, 0, 0,
		0, 1, 0, 0,
		0, 1, 1, 1,
		0, 1, 0, 0;

	lernExamples[3] <<
		0, 1, 0, 0,
		0, 1, 1, 1,
		0, 1, 0, 0,
		0, 0, 0, 0;

	//Cs
	lernExamples[4] <<
		1, 1, 0, 0,
		1, 0, 0, 0,
		1, 1, 0, 0,
		0, 0, 0, 0;

	lernExamples[5] <<
		0, 1, 1, 0,
		0, 1, 0, 0,
		0, 1, 1, 0,
		0, 0, 0, 0;

	lernExamples[6] <<
		0, 0, 1, 1,
		0, 0, 1, 0,
		0, 0, 1, 1,
		0, 0, 0, 0;

	lernExamples[7] <<
		0, 0, 0, 0,
		1, 1, 0, 0,
		1, 0, 0, 0,
		1, 1, 0, 0;

	lernExamples[8] <<
		0, 0, 0, 0,
		0, 1, 1, 0,
		0, 1, 0, 0,
		0, 1, 1, 0;

	lernExamples[9] <<
		0, 0, 0, 0,
		0, 0, 1, 1,
		0, 0, 1, 0,
		0, 0, 1, 1;

	for (int i = 10; i < 20; i++)
	{
		lernExamples[i] = lernExamples[i - 10].rowwise().reverse();
	}

	for (int i = 20; i < 30; i++)
	{
		lernExamples[i] = lernExamples[i - 10].transpose();
	}

	for (int i = 30; i < 40; i++)
	{
		lernExamples[i] = lernExamples[i - 10].colwise().reverse();
	}

	Teacher* teacher = new Teacher();
	for (int i = 0; i < 40; i ++)
	{
		std::vector<std::vector<double>> teachingPattern(1, std::vector<double>(16));
		NeuralNetworkIO<bool>* teachingInput = new NeuralNetworkIO<bool>(1);
		for (int l = 0; l < 16; l += 1)
		{
			teachingPattern[0][l] = lernExamples[i](l % 4, l / 4);
		}
		(*teachingInput).set(0, 0, (i % 10) >= 4);

		teacher->addTeachingLesson(new TeachingLessonBooleanInput(teachingPattern, teachingInput));
	}


	return new TeachingEvolutionWorld(teacher, *layeredNetworkOptions);
}

std::string TCProblemEvolutionExample::getDefaultName()
{
	return "TC-problem evolution example";
}

std::string TCProblemEvolutionExample::getDescription()
{
	return "Teaches a network how to distinguish Ts and Cs with the help of evoultion.";
}

AbstractTrainingPlan* TCProblemEvolutionExample::getCopy()
{
	return new TCProblemEvolutionExample();
}

std::string TCProblemEvolutionExample::getLearningRuleName()
{
	return EvolutionLearningRule::getName();
}
