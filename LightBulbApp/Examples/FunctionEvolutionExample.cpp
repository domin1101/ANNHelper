#include "FunctionEvolutionExample.hpp"
#include <Learning/Evolution/EvolutionLearningRule.hpp>
#include <Examples/Nature.hpp>
#include <Learning/Evolution/ConstantCreationCommand.hpp>
#include <Learning/Evolution/RemainderStochasticSamplingSelector.hpp>
#include <Learning/Evolution/EvolutionStrategy/RecombinationAlgorithm.hpp>
#include <Learning/Evolution/EvolutionStrategy/MutationAlgorithm.hpp>
#include <Learning/Evolution/ConstantRecombinationCommand.hpp>
#include <Learning/Evolution/ConstantMutationCommand.hpp>
#include <Learning/Evolution/BestSelectionCommand.hpp>
#include <Diagnostic/LearningRuleAnalyser.hpp>
#include <Diagnostic/ChangeablePointer.hpp>
#include <Learning/Evolution/StochasticUniversalSamplingSelector.hpp>
#include <Function/EqualRandomFunction.hpp>
#include <Learning/Evolution/RandomSelector.hpp>
#include <Diagnostic/ChangeableNumber.hpp>
#include <Learning/Evolution/ConstantReuseCommand.hpp>
#include <Learning/Evolution/RateDifferenceCondition.hpp>
#include <Examples/FunctionSimulator.hpp>
#include <Learning/Evolution/BestReuseSelector.hpp>

static double sixHumpCamelFunction(std::vector<float> pos)
{
	return std::max(0.0, -1 * (4 * pow(pos[0], 2) - 2.1 * pow(pos[0], 4) + pow(pos[0], 6) / 3 + pos[0] * pos[1] - 4 * pow(pos[1], 2) + 4 * pow(pos[1], 4)) + 10000);
}

void doFunctionEvolutionExample()
{
	FunctionSimulatorOptions simulatorOptions;
	simulatorOptions.enableGraphics = false;

	FunctionSimulator simulator(simulatorOptions, sixHumpCamelFunction);

	EvolutionLearningRuleOptions options;
	RateDifferenceCondition* rateDifferenceCondition = new RateDifferenceCondition(0.00001, 10);
	options.exitConditions.push_back(rateDifferenceCondition);
	ConstantCreationCommand* constantCreationCommand = new ConstantCreationCommand(20);
	options.creationCommands.push_back(constantCreationCommand);
	options.reuseCommands.push_back(new ConstantReuseCommand(new BestReuseSelector(), 1));
	BestSelectionCommand* bestSelectionCommand = new BestSelectionCommand(20);
	options.selectionCommands.push_back(bestSelectionCommand);
	MutationAlgorithm* mutationAlgorithm = new MutationAlgorithm(1.6);
	ConstantMutationCommand* constantMutationCommand = new ConstantMutationCommand(mutationAlgorithm, new StochasticUniversalSamplingSelector(), 1.8, false);
	options.mutationsCommands.push_back(constantMutationCommand);
	ConstantRecombinationCommand* constantRecombinationCommand = new ConstantRecombinationCommand(new RecombinationAlgorithm(), new StochasticUniversalSamplingSelector(), 0.3, false);
	options.recombinationCommands.push_back(constantRecombinationCommand);
	options.world = &simulator;
	options.enableDebugOutput = false;
	options.scoreGoal = 1.031627 + 10000;
	EvolutionLearningRule learningRule(options);

	LearningRuleAnalyserOptions analyserOptions;
	analyserOptions.learningRule = &learningRule;
	//analyserOptions.changableParameters.push_back(new ChangeableNumber<double, MutationAlgorithm>(mutationAlgorithm, &MutationAlgorithm::setMutationStrengthChangeSpeed, 1.3, 0.1, 2.0, "mcs"));
	//analyserOptions.changableParameters.push_back(new ChangeableNumber<int, RateDifferenceCondition>(rateDifferenceCondition, &RateDifferenceCondition::setCount, 0, 10, 50, "cnt"));
	//analyserOptions.changableParameters.push_back(new ChangeableNumber<int, BestSelectionCommand>(bestSelectionCommand, &BestSelectionCommand::setObjectCount, 5, 5, 40, "sel"));
	analyserOptions.changableParameters.push_back(new ChangeableNumber<double, ConstantMutationCommand>(constantMutationCommand, &ConstantMutationCommand::setMutationPercentage, 0, 0.3, 2.0, "mut"));
	analyserOptions.changableParameters.push_back(new ChangeableNumber<double, ConstantRecombinationCommand>(constantRecombinationCommand, &ConstantRecombinationCommand::setRecombinationPercentage, 0, 0.3, 2.0, "rcb"));

	std::vector<std::pair<std::string, AbstractMutationSelector*>> possibleMutationSelectors;
	possibleMutationSelectors.push_back(std::make_pair<std::string, AbstractMutationSelector*>("rank", new RandomSelector(new RankBasedRandomFunction())));
	possibleMutationSelectors.push_back(std::make_pair<std::string, AbstractMutationSelector*>("equa", new RandomSelector(new EqualRandomFunction())));
	possibleMutationSelectors.push_back(std::make_pair<std::string, AbstractMutationSelector*>("rema", new RemainderStochasticSamplingSelector()));
	possibleMutationSelectors.push_back(std::make_pair<std::string, AbstractMutationSelector*>("stoc", new StochasticUniversalSamplingSelector()));

	analyserOptions.changableParameters.push_back(new ChangeablePointer<AbstractMutationSelector, ConstantMutationCommand>(constantMutationCommand, &ConstantMutationCommand::setMutationSelector, possibleMutationSelectors, "sel"));


	LearningRuleAnalyser learningRuleAnalyser(analyserOptions);

	learningRuleAnalyser.execute();

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

	BackpropagationLearningRuleOptions options;
	options.maxTotalErrorValue = 4;
	options.maxIterationsPerTry = 1000000;
	options.totalErrorGoal = 0.001f;
	options.maxTries = 1000;
	options.weightDecayFac = 0;
	options.learningRate = 0.1;
	options.momentum = 0;
	options.resilientLearningRate = false;
	options.teacher = teacher.get();
	fillDefaultLearningRuleOptions(&options);

	return new BackpropagationLearningRule(options);
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


std::string BackpropagationXorExample::getName()
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
	return BackpropagationLearningRule::getName();
}
