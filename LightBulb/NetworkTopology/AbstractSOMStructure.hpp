#pragma once

#ifndef _ABSTRACTSOMSTRUCTURE_H_
#define _ABSTRACTSOMSTRUCTURE_H_

// Libary includes
#include <map>
#include <vector>
#include <list>

// Includes

// Forward declarations
class StandardNeuron;
class SOMNetwork;

class AbstractSOMStructure
{
protected:	
	std::map<StandardNeuron*, std::list<StandardNeuron*>> neighborhoodNeurons;
	std::map<StandardNeuron*, std::map<StandardNeuron*, double>> neighborhoodDistances;
	std::map<StandardNeuron*, std::vector<float>> neuronPositionsInStructure;
public:
	virtual void initialize(SOMNetwork &somNetwork) = 0;
	std::map<StandardNeuron*, std::map<StandardNeuron*, double>>* getNeighborhoodDistances();
	std::map<StandardNeuron*, std::vector<float>>* getNeuronPositionsInStructure();
	std::map<StandardNeuron*, std::list<StandardNeuron*>>* getNeighborhoodNeurons();

};

#endif