#pragma once

#ifndef _ABSTRACTSELECTIONCOMMAND_H_
#define _ABSTRACTSELECTIONCOMMAND_H_

// Library Includes
#include <vector>

// Forward declarations
class EvolutionObjectInterface;
class EvolutionWorldInterface;

class AbstractSelectionCommand
{
public:
	virtual void execute(std::vector<std::pair<double, EvolutionObjectInterface*>>* highscore, std::vector<EvolutionObjectInterface*>* newObjectVector) = 0;
};

#endif