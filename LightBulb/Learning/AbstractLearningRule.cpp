// Includes
#include "Learning/AbstractLearningRule.hpp"
#include "NeuralNetwork/AbstractNeuralNetwork.hpp"
#include "NetworkTopology/LayeredNetwork.hpp"
#include "Teaching/Teacher.hpp"
#include "Teaching/AbstractTeachingLesson.hpp"
#include "ActivationOrder/AbstractActivationOrder.hpp"
// Library includes
#include <iomanip>
#include <iostream>
#include <vector>
#include <limits>
#include <list>
#include <vld.h>

AbstractLearningRule::AbstractLearningRule(AbstractLearningRuleOptions* options_)
{
	options.reset(options_);

	learningState.reset(new LearningState(options->dataSaveInterval));

	pauseRequest = false;
}

bool AbstractLearningRule::start()
{
	// Let the learning algorithm do stuff before starting
	initializeStartLearningAlgoritm();

	// Reset all counter
	learningState->tries = 0;
		
	return learn(false);
}

bool AbstractLearningRule::learn(bool resume)
{
	initializeLearningAlgoritm();

	// Start a new try
	do
	{
		if (!resume)
		{
			learningState->addTry();

			// Reset the iteration counter
			learningState->iterations = 0;

			// Initialize a new try
			initializeTry();

			// If debug is enabled, print every n-th iteration a short debug info
			log("Start Try: " + std::to_string(learningState->tries - 1), LL_HIGH);
			resume = false;
		}
		rateLearning();

		// Do while the totalError is not zero
		while (!hasLearningSucceeded() && learningState->iterations++ < options->maxIterationsPerTry)
		{
			bool successful = doIteration();
			if (!successful)
				break;

			if (pauseRequest)
			{
				pauseRequest = false;
				return false;
			}

			rateLearning();
		}
	} while (!hasLearningSucceeded() && learningState->tries < options->maxTries);

	if (hasLearningSucceeded())
		log("Try (No. " + std::to_string(learningState->tries - 1) + ", " + std::to_string(learningState->iterations) + " iterations needed) was successful", LL_HIGH);
	else
		log("All tries failed => stop learning", LL_HIGH);

	doCalculationAfterLearningProcess();

	return hasLearningSucceeded();
}

bool AbstractLearningRule::resume()
{
	initializeResumeLearningAlgoritm();

	return learn(true);
}

void AbstractLearningRule::sendPauseRequest()
{
	pauseRequest = true;
}

void AbstractLearningRule::setLogger(AbstractLogger* logger)
{
	options->logger = logger;
}

void AbstractLearningRule::log(std::string message, LogLevel level)
{
	if (options->logger)
		options->logger->log(message, level);
}


LearningState* AbstractLearningRule::getLearningState()
{
	return learningState.get();
}

std::vector<std::string> AbstractLearningRule::getDataSetLabels()
{
	std::vector<std::string> labels;
	return labels;
}
