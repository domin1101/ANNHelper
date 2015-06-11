#include "AbstractTeachingLesson.hpp"
#include "NeuralNetwork.hpp"
#include "AbstractNetworkTopology.hpp"
#include "StandardNeuron.hpp"
#include "NeuralNetworkIO.hpp"

std::unique_ptr<NeuralNetworkIO<float>> AbstractTeachingLesson::tryLesson(NeuralNetwork &neuralNetwork, AbstractActivationOrder &activationOrder, int startTime, int timeStepCount, std::vector<std::map<AbstractNeuron*, float>>* outputValuesInTime, std::vector<std::map<AbstractNeuron*, float>>* netInputValuesInTime)
{
	// Let the network calculate
	return neuralNetwork.calculate(*getTeachingPattern(), activationOrder, startTime, timeStepCount, outputValuesInTime, netInputValuesInTime);
}

std::unique_ptr<ErrorMap_t> AbstractTeachingLesson::getErrormap(NeuralNetwork &neuralNetwork, AbstractActivationOrder &activationOrder, int startTime, int timeStepCount, std::vector<std::map<AbstractNeuron*, float>>* outputValuesInTime, std::vector<std::map<AbstractNeuron*, float>>* netInputValuesInTime)
{
	// Get the teachingInput
	NeuralNetworkIO<float>* teachingInput = getTeachingInput(dynamic_cast<StandardNeuron*>((*neuralNetwork.getNetworkTopology()->getOutputNeurons())[0])->getActivationFunction());

	// Create the errorVector with the right size
	std::unique_ptr<ErrorMap_t> errorMap(new ErrorMap_t());

	// Try the lesson and extract the output
	std::unique_ptr<NeuralNetworkIO<float>> outputVector = tryLesson(neuralNetwork, activationOrder, startTime, timeStepCount, outputValuesInTime, netInputValuesInTime);

	// Calculate the error values (expected value - real value)
	for (NeuralNetworkIO<float>::iterator teachingInputValue = teachingInput->begin(); teachingInputValue != teachingInput->end(); teachingInputValue++)
	{
		if ((*outputVector).count(teachingInputValue->first) != 0)
		{
			for (int i = 0; i < teachingInputValue->second.size(); i++)
			{
				(*errorMap)[teachingInputValue->first][static_cast<StandardNeuron*>((*neuralNetwork.getNetworkTopology()->getOutputNeurons())[i])] = teachingInputValue->second[i] - outputVector->at(teachingInputValue->first)[i];
			}
		}
	}

	return errorMap;
}

float AbstractTeachingLesson::getEuclidienDistance(NeuralNetwork &neuralNetwork, AbstractActivationOrder &activationOrder)
{
	return 0;
}

float AbstractTeachingLesson::getRMS(NeuralNetwork &neuralNetwork, AbstractActivationOrder &activationOrder)
{
	return 0;
}

float AbstractTeachingLesson::getSpecificError(NeuralNetwork &neuralNetwork, AbstractActivationOrder &activationOrder)
{
	// Calculate the errorVector
	std::unique_ptr<ErrorMap_t> errorMap = getErrormap(neuralNetwork, activationOrder, 0, getMaxTimeStep() + 1);

	float specificError = 0;

	for (ErrorMap_t::iterator errorValues = errorMap->begin(); errorValues != errorMap->end(); errorValues++)
	{
		// Add the square of every errorValue in the errorVector
		for (std::map<StandardNeuron*, float>::iterator errorValue = errorValues->second.begin(); errorValue != errorValues->second.end(); errorValue++)
		{
			specificError += pow(errorValue->second, 2);
		}

		// Divide the specific error by two
		specificError /= 2;
	}
	
	return specificError;
}
