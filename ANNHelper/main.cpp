#include <iostream>
#include <exception>

#include "WeightedSumFunction.hpp"
#include "Edge.hpp"
#include "NeuralNetwork.hpp"
#include "LayeredNetwork.hpp"
#include "WeightedSumFunction.hpp"
#include "IdentityFunction.hpp"
#include "TopologicalOrder.hpp"

int main()
{
	LayeredNetworkOptions layeredNetworkOptions;
	layeredNetworkOptions.inputFunction = new WeightedSumFunction();
	layeredNetworkOptions.activationFunction = new IdentityFunction();
	layeredNetworkOptions.outputFunction = new IdentityFunction();
	layeredNetworkOptions.neuronsPerLayerCount = std::vector<int>(4);
	layeredNetworkOptions.neuronsPerLayerCount[0]=3;
	layeredNetworkOptions.neuronsPerLayerCount[1]=1;
	layeredNetworkOptions.neuronsPerLayerCount[2]=4;
	layeredNetworkOptions.neuronsPerLayerCount[3]=2;

	LayeredNetwork layeredNetwork(layeredNetworkOptions);

	NeuralNetwork neuralNetwork(&layeredNetwork);

	TopologicalOrder topologicalOrder;
	neuralNetwork.refreshAllNeurons(topologicalOrder);
	std::cout << "ANNHelper!";
	system("pause");
	return 0;
}