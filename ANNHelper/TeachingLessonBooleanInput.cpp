#include "TeachingLessonBooleanInput.hpp"
#include "NeuralNetwork.hpp"
#include "AbstractActivationFunction.hpp"
#include <exception>

TeachingLessonBooleanInput::TeachingLessonBooleanInput(std::vector<float>* teachingPattern_, std::vector<bool>* teachingInput_)
{
	teachingInput = std::unique_ptr<std::vector<bool>>(teachingInput_);	
	teachingPattern = std::unique_ptr<std::vector<float>>(teachingPattern_);
	teachingInputLinear = std::unique_ptr<std::vector<float>>(new std::vector<float>(teachingInput->size()));
}

std::vector<float>* TeachingLessonBooleanInput::getTeachingInput(AbstractActivationFunction* activationFunction)
{
	// Check if the neuralNetwork has a boolean acitvationFunction in all outputNeurons
	if (activationFunction->isLinear())
		throw std::invalid_argument("The activationFunction of the outputNeurons is linear, but your teaching input is boolean.");

	// Go through all  teaching input values
	for (int i = 0; i < teachingInputLinear->size(); i++)
	{
		// If the boolean value is true, set the maximum of the activationFunction, else the minimum
		if ((*teachingInput)[i])
			(*teachingInputLinear)[i] = activationFunction->getMaximum();
		else
			(*teachingInputLinear)[i] = activationFunction->getMinimum();
	}

	// Return the vector with float values
	return teachingInputLinear.get();
}

std::vector<float>* TeachingLessonBooleanInput::getTeachingPattern()
{
	return teachingPattern.get();
}