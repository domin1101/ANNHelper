#pragma once

#ifndef _CONSTANTMUTATIONCOMMAND_H_
#define _CONSTANTMUTATIONCOMMAND_H_

// Library Includes

// Includes
#include "Learning\AbstractMutationCommand.hpp"

// Forward declarations


class ConstantMutationCommand : public AbstractMutationCommand
{
private:
	int objectCount;
public:
	ConstantMutationCommand(int objectCount_);
	void execute(std::vector<std::pair<double, EvolutionObjectInterface*>>* highscore, std::vector<EvolutionObjectInterface*>* newObjectVector);
};

#endif