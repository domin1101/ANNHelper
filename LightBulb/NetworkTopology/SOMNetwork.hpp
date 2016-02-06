#pragma once

#ifndef _SOMNETWORK_H_
#define _SOMNETWORK_H_

// Libary includes
#include <map>

// Includes
#include "NetworkTopology/LayeredNetwork.hpp"

// Forward declarations
class AbstractTeachingLessoni;
class Teacher;
class AbstractSOMStructure;

// A SOMNetwork is layered network which has only one input and one output layer.
class SOMNetwork : public LayeredNetwork
{
private:	
	AbstractSOMStructure* structure;
public:
	// Create a new SOMNetwork with the given counts
	SOMNetwork(unsigned int inputNeuronCount, unsigned int codebookVectorCount, AbstractSOMStructure* somStructure);
	// Returns the count of all codebook vectors
	int getCodebookVectorCount();
	// Returns the structure
	AbstractSOMStructure* getStructure();
};

#endif