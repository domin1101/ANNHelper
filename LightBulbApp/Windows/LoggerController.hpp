#pragma once

#ifndef _LOGGERCONTROLLER_H_
#define _LOGGERCONTROLLER_H_

// Library includes
#include <vector>
#include <NeuralNetwork/AbstractNeuralNetwork.hpp>

// Includes
#include "TrainingPlans/AbstractTrainingPlan.hpp"
#include <Logging/AbstractLogger.hpp>
#include "LoggerWindow.hpp"

// Forward declarations
class TrainingWindow;

class LoggerController : public AbstractLogger
{
private:
	std::unique_ptr<LoggerWindow> window;
protected:
	void outputMessage(std::string message);
public:
	LoggerController(AbstractWindow* parent = NULL);
	void show();
	LoggerWindow* getWindow();
};

#endif
