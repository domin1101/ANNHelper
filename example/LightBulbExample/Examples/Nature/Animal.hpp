#pragma once

#ifndef _ANIMAL_H_
#define _ANIMAL_H_

// Library Includes
#include <memory>

// Includes
#include "Learning/Evolution/AbstractDefaultIndividual.hpp"

// Forward declarations
class Nature;

class Animal : public LightBulb::AbstractDefaultIndividual
{
protected:
	Nature* nature;
	int posX;
	int posY;
	int dirX;
	int dirY;
	int health;
	bool dead;
	int stepsSurvived;
	void rotate(int dir);
	void getNNInput(std::vector<double>& input) override;
	void interpretNNOutput(std::vector<double>& output) override;
public:	
	Animal(Nature& nature_, int posX_, int posY_, int dirX_, int dirY_);
	~Animal();
	int getPosX() const;
	int getPosY() const;
	bool isDead() const;
	int getStepsSurvived() const;
	void reset(int posX_, int posY_, int dirX_, int dirY_);
};
#endif
