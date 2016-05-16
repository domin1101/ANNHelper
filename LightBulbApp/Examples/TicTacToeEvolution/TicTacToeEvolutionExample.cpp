#include "TicTacToeEvolutionExample.hpp"
#include <Learning/Evolution/ConstantMutationCommand.hpp>
#include <Learning/Evolution/EvolutionStrategy/MutationAlgorithm.hpp>
#include <Learning/Evolution/BestSelectionCommand.hpp>
#include <Learning/Evolution/ConstantReuseCommand.hpp>
#include <Learning/Evolution/ConstantRecombinationCommand.hpp>
#include <Learning/Evolution/EvolutionStrategy/RecombinationAlgorithm.hpp>
#include <Learning/Evolution/ConstantCreationCommand.hpp>
#include <Learning/Evolution/EvolutionLearningRule.hpp>
#include <Learning/Evolution/RandomSelector.hpp>
#include <Learning/Evolution/SharedSamplingCombiningStrategy.hpp>
#include <Learning/Evolution/AbstractHallOfFameAlgorithm.hpp>
#include <Learning/Evolution/RandomHallOfFameAlgorithm.hpp>
#include <Examples/TicTacToeEvolution/TicTacToe.hpp>
#include <Learning/Evolution/SharedCoevolutionFitnessFunction.hpp>
#include <Learning/Evolution/PerfectObjectFoundCondition.hpp>
#include <Learning/Evolution/BipartiteEvolutionLearningRule.hpp>
#include <fstream>
#include <Learning/Evolution/BestReuseSelector.hpp>
#include "TicTacToeGameFactory.hpp"
#include <TrainingPlans/DoublePreference.hpp>
#include <TrainingPlans/IntegerPreference.hpp>
#include <TrainingPlans/BooleanPreference.hpp>
#include <NeuronFactory/SameNeuronDescriptionFactory.hpp>
#include <Function/WeightedSumFunction.hpp>
#include <Function/BinaryFunction.hpp>
#include <Neuron/NeuronDescription.hpp>
#include <NetworkTopology/LayeredNetwork.hpp>

#define PREFERENCE_POPULATION_SIZE "Population size"
#define PREFERENCE_MUTATION_PERCENTAGE "Mutation percentage"
#define PREFERENCE_RECOMBINATION_PERCENTAGE "Recombination percentage"
#define PREFERENCE_COMPETITIONS_SIZE "Competitions per object"
#define PREFERENCE_HALLOFFAME_COMPETITIONS_SIZE "Hall of fame competitions"
#define PREFERENCE_SHORTCUT_ENABLE "Enable shortcut connections"
#define PREFERENCE_NEURON_COUNT_FIRST_LAYER "Neuron count in 1. layer"
#define PREFERENCE_SECOND_LAYER_ENABLE "Enable 2. layer"
#define PREFERENCE_NEURON_COUNT_SECOND_LAYER "Neuron count in 2. layer"


AbstractLearningRule* TicTacToeEvolutionExample::createLearningRate()
{

	EvolutionLearningRuleOptions options;

	options.creationCommands.push_back(new ConstantCreationCommand(getIntegerPreference(PREFERENCE_POPULATION_SIZE)));
	options.exitConditions.push_back(new PerfectObjectFoundCondition(200));
	options.reuseCommands.push_back(new ConstantReuseCommand(new BestReuseSelector(), 16));
	options.selectionCommands.push_back(new BestSelectionCommand(getIntegerPreference(PREFERENCE_POPULATION_SIZE)));
	options.mutationsCommands.push_back(new ConstantMutationCommand(new MutationAlgorithm(1.6), new RandomSelector(new RankBasedRandomFunction()), getDoublePreference(PREFERENCE_MUTATION_PERCENTAGE)));
	options.recombinationCommands.push_back(new ConstantRecombinationCommand(new RecombinationAlgorithm(), new RandomSelector(new RankBasedRandomFunction()), getDoublePreference(PREFERENCE_RECOMBINATION_PERCENTAGE)));
	//options.fitnessFunctions.push_back(new FitnessSharingFitnessFunction(150));
	fillDefaultEvolutionLearningRule1Options(&options);

	//options.recombinationCommands.push_back(new ConstantRecombinationCommand(7));

	EvolutionLearningRule* learningRule1 = new EvolutionLearningRule(options);
	fillDefaultEvolutionLearningRule2Options(&options);
	EvolutionLearningRule* learningRule2 = new EvolutionLearningRule(options);

	BipartiteEvolutionLearningRuleOptions bipartiteOptions;
	bipartiteOptions.learningRule1 = learningRule1;
	bipartiteOptions.learningRule2 = learningRule2;
	fillDefaultLearningRuleOptions(&bipartiteOptions);

	return new BipartiteEvolutionLearningRule(bipartiteOptions);
}


LayeredNetworkOptions TicTacToeEvolutionExample::getNetworkOptions()
{
	LayeredNetworkOptions options;
	options.enableShortcuts = getBooleanPreference(PREFERENCE_SHORTCUT_ENABLE);

	options.neuronsPerLayerCount.push_back(18);
	options.neuronsPerLayerCount.push_back(getIntegerPreference(PREFERENCE_NEURON_COUNT_FIRST_LAYER));
	if (getBooleanPreference(PREFERENCE_SECOND_LAYER_ENABLE))
		options.neuronsPerLayerCount.push_back(getIntegerPreference(PREFERENCE_NEURON_COUNT_SECOND_LAYER));
	options.neuronsPerLayerCount.push_back(9);

	options.descriptionFactory = new SameNeuronDescriptionFactory(new NeuronDescription(new WeightedSumFunction(), new BinaryFunction()));
	return options;
}

AbstractEvolutionWorld* TicTacToeEvolutionExample::createWorld()
{
	cs1 = new SharedSamplingCombiningStrategy(getIntegerPreference(PREFERENCE_COMPETITIONS_SIZE));

	hof1 = new RandomHallOfFameAlgorithm(getIntegerPreference(PREFERENCE_HALLOFFAME_COMPETITIONS_SIZE));
	hof2 = new RandomHallOfFameAlgorithm(getIntegerPreference(PREFERENCE_HALLOFFAME_COMPETITIONS_SIZE));

	LayeredNetworkOptions options = getNetworkOptions();
	return new TicTacToe(options, false, cs1, new SharedCoevolutionFitnessFunction(), hof1, hof2);
}

AbstractEvolutionWorld* TicTacToeEvolutionExample::createParasiteWorld()
{
	cs2 = new SharedSamplingCombiningStrategy(getIntegerPreference(PREFERENCE_COMPETITIONS_SIZE));

	LayeredNetworkOptions options = getNetworkOptions();
	TicTacToe* ticTacToe2 = new TicTacToe(options, true, cs2, new SharedCoevolutionFitnessFunction(), hof2, hof1);
	cs1->setSecondWorld(ticTacToe2);
	cs2->setSecondWorld(static_cast<TicTacToe*>(world.get()));
	return ticTacToe2;
}

TicTacToeEvolutionExample::TicTacToeEvolutionExample()
{
	addCustomSubApp(new TicTacToeGameFactory());
	addPreference(new DoublePreference(PREFERENCE_MUTATION_PERCENTAGE, 1.8, 0, 3));
	addPreference(new DoublePreference(PREFERENCE_RECOMBINATION_PERCENTAGE, 0.3, 0, 3));
	addPreference(new IntegerPreference(PREFERENCE_POPULATION_SIZE, 500, 1, 1000));
	addPreference(new IntegerPreference(PREFERENCE_COMPETITIONS_SIZE, 100, 1, 1000));
	addPreference(new IntegerPreference(PREFERENCE_HALLOFFAME_COMPETITIONS_SIZE, 50, 1, 1000));
	addPreference(new BooleanPreference(PREFERENCE_SHORTCUT_ENABLE, true));
	addPreference(new IntegerPreference(PREFERENCE_NEURON_COUNT_FIRST_LAYER, 10, 1, 30));
	addPreference(new BooleanPreference(PREFERENCE_SECOND_LAYER_ENABLE, true));
	addPreference(new IntegerPreference(PREFERENCE_NEURON_COUNT_SECOND_LAYER, 10, 1, 30));
}

std::string TicTacToeEvolutionExample::getDefaultName()
{
	return "TicTacToe evolution example";
}

std::string TicTacToeEvolutionExample::getDescription()
{
	return "Evolution of a perfect TicTacToe KI with the bipartite evolution learning rule.";
}

AbstractTrainingPlan* TicTacToeEvolutionExample::getCopy()
{
	return new TicTacToeEvolutionExample();
}

std::string TicTacToeEvolutionExample::getLearningRuleName()
{
	return BipartiteEvolutionLearningRule::getName();
}

