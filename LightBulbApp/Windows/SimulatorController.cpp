// Includes
#include "Windows/SimulatorController.hpp"
#include <Repositories/NeuralNetworkRepository.hpp>
#include <NetworkTopology/AbstractNetworkTopology.hpp>
#include <ActivationOrder/TopologicalOrder.hpp>

SimulatorController::SimulatorController(NeuralNetworkRepository* neuralNetworkRepository_, AbstractWindow* parent)
{
	neuralNetworkRepository = neuralNetworkRepository_;
	neuralNetworkRepository->registerObserver(EVT_NN_CHANGED, &SimulatorController::neuralNetworksChanged, this);
	window.reset(new SimulatorWindow(this, parent));
}

SimulatorWindow* SimulatorController::getWindow()
{
	return window.get();
}

std::vector<std::unique_ptr<AbstractNeuralNetwork>>* SimulatorController::getNeuralNetworks()
{
	return neuralNetworkRepository->getNeuralNetworks();
}

void SimulatorController::neuralNetworksChanged(NeuralNetworkRepository* neuralNetworkRepository)
{
	window->refreshNeuralNetworks();
}

std::vector<double> SimulatorController::calculate(int neuralNetworkIndex, std::vector<double> input)
{
	AbstractNeuralNetwork* network = (*neuralNetworkRepository->getNeuralNetworks())[neuralNetworkIndex].get();
	std::vector<double> output(network->getNetworkTopology()->getOutputSize());
	TopologicalOrder activationOrder;
	network->calculate(input, output, activationOrder);
	return output;
}

std::string SimulatorController::getLabel()
{
	return "Simulator";
}
