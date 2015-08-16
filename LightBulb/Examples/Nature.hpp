#pragma once

#ifndef _NATURE_H_
#define _NATURE_H_

// Library Includes
#include <vector>
#include <SFML\Graphics.hpp>

// Include
#include "Learning\EvolutionWorldInterface.hpp"
#include "Examples\NatureDrawer.hpp"

// Forward declarations
class EvolutionLearningRule;
class EvolutionObjectInterface;
class Animal;

class Nature : public EvolutionWorldInterface
{
	std::vector<Animal*> animals;
	std::vector<std::vector<bool>> plants;
	int width;
	int height;
	bool getViewValueOfPos(int posX, int posY);
	sf::RenderWindow window;
	std::unique_ptr<NatureDrawer> drawer;
public:
	Nature();
	EvolutionObjectInterface* addNewObject();
	void doSimulationStep(EvolutionLearningRule& learningRule);
	std::vector<bool> getSight(int posX, int posY, int dirX, int dirY);
	void tryToEat(int posX, int posY);
	bool isTileFree(int posX, int posY);
	int getWidth();
	int getHeight();
	std::vector<std::vector<bool>>* getPlants();
};

#endif