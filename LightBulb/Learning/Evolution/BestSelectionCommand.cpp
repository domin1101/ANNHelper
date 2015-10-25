// Includes
#include "Learning/Evolution/BestSelectionCommand.hpp"
#include "Learning/Evolution/AbstractEvolutionWorld.hpp"
#include "Learning/Evolution/AbstractEvolutionObject.hpp"
#include "Learning/Evolution/EvolutionLearningRule.hpp"
//Library includes
#include <iostream>
#include <iomanip>
#include <algorithm>

BestSelectionCommand::BestSelectionCommand(int objectCount_, bool enableDebugOutput_)
	: AbstractSelectionCommand(enableDebugOutput_)
{
	objectCount = objectCount_;
	selectionPercentage = 0;
}

BestSelectionCommand::BestSelectionCommand(double selectionPercentage_, bool enableDebugOutput_)
	: AbstractSelectionCommand(enableDebugOutput_)
{
	objectCount = 0;
	selectionPercentage = selectionPercentage_;
}

void BestSelectionCommand::execute(std::vector<std::pair<double, AbstractEvolutionObject*>>* highscore)
{
	int objectCount = this->objectCount;
	// Calculate a temporary static object count if the percentage value is used
	if (selectionPercentage)
		objectCount = (int)(highscore->size() * selectionPercentage);

	if (enableDebugOutput)
		std::cout << "Selected " << objectCount << " best ones:";
	
	if (highscore->size() > objectCount)
	{
		// Go through all not selected objects
		for (auto entry = highscore->begin() + objectCount; entry != highscore->end(); entry++)
		{
			// Delete them
			delete(entry->second);
		}

		// Resize the vector
		highscore->resize(objectCount);
	}

	if (enableDebugOutput)
	{
		for (auto entry = highscore->begin(); entry != highscore->end(); entry++)
		{
			std::cout << std::fixed << std::setprecision(5) << entry->first << ", ";
		}
	}

	if (enableDebugOutput)
		std::cout << std::endl;
}

void BestSelectionCommand::setObjectCount(int newObjectCount)
{
	objectCount = newObjectCount;
}
