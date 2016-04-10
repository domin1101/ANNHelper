#include "App.hpp"
#include "Windows/TrainingController.hpp"
#include "Windows/LoggerController.hpp"
#include "Windows/SimulatorController.hpp"
#include "Windows/LearningStateController.hpp"
#include "Repositories/NeuralNetworkRepository.hpp"
#include "Repositories/TrainingPlanRepository.hpp"
#include "Windows/LearningStateFactory.hpp"
#include "Windows/SimulatorFactory.hpp"
#include "Windows/LoggerFactory.hpp"

bool App::OnInit()
{
	NeuralNetworkRepository* neuralNetworkRepository = new NeuralNetworkRepository();
	TrainingPlanRepository* trainingPlanRepository = new TrainingPlanRepository();

	TrainingController* trainingController = new TrainingController(neuralNetworkRepository, trainingPlanRepository);
	LoggerFactory* loggerFactory = new LoggerFactory();
	SimulatorFactory* simulatorFactory = new SimulatorFactory(neuralNetworkRepository);
	LearningStateFactory* learningStateFactory = new LearningStateFactory(trainingPlanRepository);

	trainingController->addSubAppFactory(loggerFactory);
	trainingController->addSubAppFactory(simulatorFactory);
	trainingController->addSubAppFactory(learningStateFactory);

	trainingController->show();
	return true;
}
