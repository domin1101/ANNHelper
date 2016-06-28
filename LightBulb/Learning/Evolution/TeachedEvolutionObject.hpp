#pragma once

#ifndef _TEACHEDEVOLUTIONOBJECT_H_
#define _TEACHEDEVOLUTIONOBJECT_H_

// Library Includes

// Includes
#include "Learning/Evolution/AbstractEvolutionObject.hpp"
#include "NeuralNetwork/NeuralNetwork.hpp"

// Forward declarations
class EvolutionLearningRule;
class TeachingEvolutionWorld;
class LayeredNetworkOptions;

// A evolution object used in the TeachingEvolutionWorld
class TeachedEvolutionObject : public AbstractEvolutionObject
{
	template <class Archive>
	friend void save(Archive& archive, TeachedEvolutionObject const& object);
	template <class Archive>
	friend void load(Archive& archive, TeachedEvolutionObject& object);
protected:
	// The corresponding world
	TeachingEvolutionWorld* teachingEvolutionWorld;
	// The used network
	std::unique_ptr<NeuralNetwork> neuralNetwork;
	// The currentTotalError (used as score)
	double currentTotalError;
	double currentTeachingError;
	double currentWeightDecayError;
public:	
	// Create a new evolution object in the given world and with a NN built after the given network options
	TeachedEvolutionObject(TeachingEvolutionWorld* teachingEvolutionWorld_, LayeredNetworkOptions& options);
	TeachedEvolutionObject() = default;
	double getCurrentTotalError();
	double getCurrentTeachingError();
	double getCurrentWeightDecayError();
	// Inherited:
	void doNNCalculation();
	void resetNN();
	AbstractNeuralNetwork* getNeuralNetwork();
	AbstractEvolutionObject* clone(bool addToWorld = true);
};

#include "IO/TeachedEvolutionObjectIO.hpp"

#endif
