// Includes
#include "Examples/FunctionEvolution/FunctionSimulator.hpp"
#include "Examples/FunctionEvolution/Position.hpp"
#include "Learning/Evolution/AbstractIndividual.hpp"
//Library includes
#include <iostream>

using namespace LightBulb;

AbstractIndividual* FunctionSimulator::createNewIndividual()
{
	return new Position(*this);
}

FunctionSimulator::FunctionSimulator(FunctionSimulatorOptions &options_, Function function_)
{
	function = function_;

	options.reset(new FunctionSimulatorOptions(options_));

	//if (options->enableGraphics)
	//{
	//	window.create(sf::VideoMode(800, 700), "LightBulb!");
	//	FunctionDrawerOptions options;
	//	options.functionSimulator = this;
	//	options.startX = -2.5;
	//	options.endX = 2.5;
	//	options.startY = -1.5;
	//	options.endY = 1.5;

	////	options.startX = -0.5;
	////	options.endX = 0.5;
	////	options.startY = 0.25;
	////	options.endY = 1.25;
	//	options.markedPositions.push_back(sf::Vector2f(0.0898f, -0.7126f));
	//	options.markedPositions.push_back(sf::Vector2f(-0.0898f, 0.7126f));
	//	//options.markedPositions.push_back(sf::Vector2f(0, 0));
	//	drawer.reset(new FunctionDrawer(options));
	//}
}

void FunctionSimulator::doSimulationStep()
{
	for (auto position = individuals.begin(); position != individuals.end(); position++)
	{
		(*position)->doNNCalculation();
	}

	/*if (options->enableGraphics)
	{
		window.clear();
		drawer->recalculateAllValues();
		drawer->draw(window);
		window.display();
	}*/
}

double FunctionSimulator::getFitness(const AbstractIndividual& individual) const
{
	std::vector<float> pos = static_cast<const Position&>(individual).getPosition();
	return function(pos);
}

