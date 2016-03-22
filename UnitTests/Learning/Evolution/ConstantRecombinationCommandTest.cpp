#include "gtest/gtest.h"
#include "Learning/Evolution/ConstantRecombinationCommand.hpp"
#include "Function/FermiFunction.hpp"
#include <Mocks/MockRecombinationAlgorithm.hpp>
#include <Mocks/MockRecombinationSelector.hpp>
#include <Mocks/MockEvolutionObject.hpp>

class ConstantRecombinationCommandTest : public testing::Test {
public:
	ConstantRecombinationCommand* constantRecombinationCommand;
	MockRecombinationAlgorithm* recombinationAlgorithm;
	MockRecombinationSelector* recombinationSelector;

	std::map<AbstractEvolutionObject*, int> counter;
	std::vector<AbstractEvolutionObject*> selectedObjects;
	std::vector<AbstractEvolutionObject*> newObjectVector;
	std::vector<AbstractEvolutionObject*> notUsedObjects;
	void SetUp() {
		recombinationAlgorithm = new MockRecombinationAlgorithm();
		recombinationSelector = new MockRecombinationSelector();
		constantRecombinationCommand = new ConstantRecombinationCommand(recombinationAlgorithm, recombinationSelector, 10);
	}

	void SetUpExecute()
	{
		MockEvolutionObject firstSelectedObject, secondSelectedObject;

		selectedObjects.resize(2);
		selectedObjects[0] = new MockEvolutionObject();
		selectedObjects[1] = new MockEvolutionObject();

		EXPECT_CALL(*recombinationSelector, getRecombinationSelection()).WillRepeatedly(testing::Return(&selectedObjects));
	}

	virtual ~ConstantRecombinationCommandTest()
	{
		delete constantRecombinationCommand;
		for (int i = 0; i < selectedObjects.size(); i++)
			delete(selectedObjects[i]);
	}
};

TEST_F(ConstantRecombinationCommandTest, selectStaticCount)
{
	std::vector<std::pair<double, AbstractEvolutionObject*>> highscore;
	std::map<AbstractEvolutionObject*, int> counter;
	constantRecombinationCommand->setRecombinationCount(10);

	EXPECT_CALL(*recombinationSelector, executeRecombinationSelection(10, &highscore, &counter)).Times(1);

	constantRecombinationCommand->select(&highscore, &counter);
}

TEST_F(ConstantRecombinationCommandTest, selectPercentage)
{
	std::vector<std::pair<double, AbstractEvolutionObject*>> highscore(10);
	std::map<AbstractEvolutionObject*, int> counter;
	constantRecombinationCommand->setRecombinationPercentage(0.5);

	EXPECT_CALL(*recombinationSelector, executeRecombinationSelection(5, &highscore, &counter)).Times(1);

	constantRecombinationCommand->select(&highscore, &counter);
}

TEST_F(ConstantRecombinationCommandTest, executeWithNoMultipleUsedObjects)
{
	SetUpExecute();
	counter[selectedObjects[0]] = 1;
	counter[selectedObjects[1]] = 1;

	EXPECT_CALL(*recombinationAlgorithm, execute(selectedObjects[0], selectedObjects[1])).Times(1);

	constantRecombinationCommand->execute(&newObjectVector, &counter, &notUsedObjects);

	EXPECT_EQ(1, newObjectVector.size());
	EXPECT_EQ(selectedObjects[0], newObjectVector[0]);
	EXPECT_EQ(0, counter[selectedObjects[0]]);
	EXPECT_EQ(0, counter[selectedObjects[1]]);
}

TEST_F(ConstantRecombinationCommandTest, executeWithOneMultipleUsedObject1)
{
	SetUpExecute();
	counter[selectedObjects[0]] = 2;
	counter[selectedObjects[1]] = 1;

	EXPECT_CALL(*recombinationAlgorithm, execute(selectedObjects[1], selectedObjects[0])).Times(1);

	constantRecombinationCommand->execute(&newObjectVector, &counter, &notUsedObjects);

	EXPECT_EQ(1, newObjectVector.size());
	EXPECT_EQ(selectedObjects[1], newObjectVector[0]);
	EXPECT_EQ(1, counter[selectedObjects[0]]);
	EXPECT_EQ(0, counter[selectedObjects[1]]);
}


TEST_F(ConstantRecombinationCommandTest, executeWithOneMultipleUsedObject2)
{
	SetUpExecute();
	counter[selectedObjects[0]] = 1;
	counter[selectedObjects[1]] = 2;

	EXPECT_CALL(*recombinationAlgorithm, execute(selectedObjects[0], selectedObjects[1])).Times(1);

	constantRecombinationCommand->execute(&newObjectVector, &counter, &notUsedObjects);

	EXPECT_EQ(1, newObjectVector.size());
	EXPECT_EQ(selectedObjects[0], newObjectVector[0]);
	EXPECT_EQ(0, counter[selectedObjects[0]]);
	EXPECT_EQ(1, counter[selectedObjects[1]]);
}


TEST_F(ConstantRecombinationCommandTest, executeWithTwoMultipleUsedObjects)
{
	SetUpExecute();
	counter[selectedObjects[0]] = 2;
	counter[selectedObjects[1]] = 2;
	MockEvolutionObject clonedObject;

	EXPECT_CALL(*(MockEvolutionObject*)selectedObjects[1], clone(true)).Times(1).WillOnce(testing::Return(&clonedObject));
	EXPECT_CALL(*recombinationAlgorithm, execute(&clonedObject, selectedObjects[0])).Times(1);

	constantRecombinationCommand->execute(&newObjectVector, &counter, &notUsedObjects);

	EXPECT_EQ(1, newObjectVector.size());
	EXPECT_EQ(&clonedObject, newObjectVector[0]);
	EXPECT_EQ(1, counter[selectedObjects[0]]);
	EXPECT_EQ(1, counter[selectedObjects[1]]);
}

TEST_F(ConstantRecombinationCommandTest, executeWithTwoMultipleUsedObjectsAndANotUsedObject)
{
	SetUpExecute();
	counter[selectedObjects[0]] = 2;
	counter[selectedObjects[1]] = 2;
	MockEvolutionObject unusedObject;
	notUsedObjects.push_back(&unusedObject);

	EXPECT_CALL(*recombinationAlgorithm, execute(&unusedObject, selectedObjects[0])).Times(1);

	constantRecombinationCommand->execute(&newObjectVector, &counter, &notUsedObjects);

	EXPECT_EQ(1, newObjectVector.size());
	EXPECT_EQ(&unusedObject, newObjectVector[0]);
	EXPECT_EQ(1, counter[selectedObjects[0]]);
	EXPECT_EQ(1, counter[selectedObjects[1]]);
}