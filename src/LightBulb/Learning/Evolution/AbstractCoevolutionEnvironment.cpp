// Includes
#include "Learning/Evolution/AbstractCoevolutionEnvironment.hpp"
#include "Learning/Evolution/AbstractCombiningStrategy.hpp"
#include "Learning/Evolution/AbstractCoevolutionFitnessFunction.hpp"
//Library includes
#include <iostream>

namespace LightBulb
{
	AbstractCoevolutionEnvironment::AbstractCoevolutionEnvironment(bool isParasiteEnvironment_, AbstractCombiningStrategy* combiningStrategy_, AbstractCoevolutionFitnessFunction* fitnessFunction_, AbstractHallOfFameAlgorithm* hallOfFameToAddAlgorithm_, AbstractHallOfFameAlgorithm* hallOfFameToChallengeAlgorithm_)
	{
		parasiteEnvironment = isParasiteEnvironment_;
		combiningStrategy.reset(combiningStrategy_);
		fitnessFunction.reset(fitnessFunction_);
		hallOfFameToAddAlgorithm.reset(hallOfFameToAddAlgorithm_);
		hallOfFameToChallengeAlgorithm.reset(hallOfFameToChallengeAlgorithm_);
		comparisons = 0;
	}

	bool AbstractCoevolutionEnvironment::doSimulationStep()
	{
		CombiningStrategyResults& results = combiningStrategy->execute(*this);

		if (!learningState->disabledDatasets[std::string(parasiteEnvironment ? DATASET_PARASITE_PREFIX : "") + DATASET_AVG_WINS] && results.size() > 0)
			learningState->addData(std::string(parasiteEnvironment ? DATASET_PARASITE_PREFIX : "") + DATASET_AVG_WINS, static_cast<double>(combiningStrategy->getFirstPlayerWins()) / combiningStrategy->getTotalMatches(*this));

		if (hallOfFameToChallengeAlgorithm)
			hallOfFameToChallengeAlgorithm->execute(*this, results);

		fitnessValues.reset(fitnessFunction->execute(results));

		AbstractIndividual* bestAI = getHighscoreList().front().second;
		rateIndividual(*bestAI);

		if (hallOfFameToAddAlgorithm)
			hallOfFameToAddAlgorithm->addMember(bestAI);

		if (!learningState->disabledDatasets[DATASET_COMP])
			learningState->addData(DATASET_COMP, comparisons / 1000000.0);

		return false;
	}

	double AbstractCoevolutionEnvironment::getFitness(const AbstractIndividual& individual) const
	{
		return (*fitnessValues)[&individual];
	}

	int AbstractCoevolutionEnvironment::compareIndividuals(AbstractIndividual& individual1, AbstractIndividual& individual2, int round)
	{
		comparisons++;
		return doCompare(individual1, individual2, round);
	}

	AbstractCombiningStrategy& AbstractCoevolutionEnvironment::getCombiningStrategy() const
	{
		return *combiningStrategy.get();
	}

	void AbstractCoevolutionEnvironment::setLogger(AbstractLogger& logger_)
	{
		AbstractLoggable::setLogger(logger_);
		if (hallOfFameToChallengeAlgorithm)
			hallOfFameToChallengeAlgorithm->setLogger(logger_);
		if (hallOfFameToAddAlgorithm)
			hallOfFameToAddAlgorithm->setLogger(logger_);
	}

	std::vector<std::string> AbstractCoevolutionEnvironment::getDataSetLabels() const
	{
		auto labels = AbstractEvolutionEnvironment::getDataSetLabels();
		labels.push_back(std::string(parasiteEnvironment ? DATASET_PARASITE_PREFIX : "") + DATASET_AVG_WINS);
		if (!parasiteEnvironment)
			labels.push_back(DATASET_COMP);
		return labels;
	}

	bool AbstractCoevolutionEnvironment::isParasiteEnvironment() const
	{
		return parasiteEnvironment;
	}

	int AbstractCoevolutionEnvironment::getRoundCount() const
	{
		return 1;
	}

	void AbstractCoevolutionEnvironment::setRandomGenerator(AbstractRandomGenerator& randomGenerator_)
	{
		AbstractRandomGeneratorUser::setRandomGenerator(randomGenerator_);
		combiningStrategy->setRandomGenerator(randomGenerator_);
	}
}