#include "gtest/gtest.h"
#include <Mocks/MockTestableEvolutionWorld.hpp>
#include <Mocks/MockEvolutionObject.hpp>
#include <Mocks/MockTestableCoevolutionWorld.hpp>
#include <Mocks/MockCombiningStrategy.hpp>
#include <Mocks/MockHallOfFameAlgorithm.hpp>
#include <Mocks/MockCoevolutionFitnessFunction.hpp>

using namespace LightBulb;

class AbstractCoevolutionWorldTest : public testing::Test {
public:
	MockTestableCoevolutionWorld* coevolutionWorld;
	MockCombiningStrategy* combiningStrategy;
	MockCoevolutionFitnessFunction* coevolutionFitnessFunction;
	MockHallOfFameAlgorithm* hallOfFameToAddAlgorithm;
	MockHallOfFameAlgorithm* hallOfFameToChallengeAlgorithm;
	
	void SetUp() {
		combiningStrategy = new MockCombiningStrategy();
		coevolutionFitnessFunction = new MockCoevolutionFitnessFunction();
		hallOfFameToAddAlgorithm = new MockHallOfFameAlgorithm();
		hallOfFameToChallengeAlgorithm = new MockHallOfFameAlgorithm();
		coevolutionWorld = new MockTestableCoevolutionWorld(true, combiningStrategy, coevolutionFitnessFunction, hallOfFameToAddAlgorithm, hallOfFameToChallengeAlgorithm);
	}

	virtual ~AbstractCoevolutionWorldTest()
	{
		delete coevolutionWorld;
	}
};

TEST_F(AbstractCoevolutionWorldTest, doSimulationStep)
{
	CombiningStrategyResults result;
	std::map<const AbstractEvolutionObject*, double>* fitnessValues = new std::map<const AbstractEvolutionObject*, double>();
	MockEvolutionObject bestObject;
	std::vector<std::pair<double, AbstractEvolutionObject*>> highscore;
	std::map<std::string, bool> disabledDataSets;
	LearningState learningState(disabledDataSets, 5);
	coevolutionWorld->setLearningState(learningState);
	highscore.push_back(std::make_pair(42, &bestObject));
	{
		testing::InSequence s;
		EXPECT_CALL(*combiningStrategy, execute(testing::Ref(*coevolutionWorld))).WillOnce(testing::ReturnRef(result));
		EXPECT_CALL(*hallOfFameToChallengeAlgorithm, execute(testing::Ref(*coevolutionWorld), testing::Ref(result))).Times(1);
		EXPECT_CALL(*coevolutionFitnessFunction, execute(testing::Ref(result))).WillOnce(testing::Return(fitnessValues));
		EXPECT_CALL(*coevolutionWorld, getHighscoreList()).WillOnce(testing::ReturnRef(highscore));
		EXPECT_CALL(*coevolutionWorld, rateKI(testing::Ref(bestObject))).Times(1);
		EXPECT_CALL(*hallOfFameToAddAlgorithm, addMember(&bestObject)).Times(1);
	}

	coevolutionWorld->doSimulationStep();
}