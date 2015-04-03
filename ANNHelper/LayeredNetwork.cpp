#include "LayeredNetwork.hpp"
#include "InputNeuron.hpp"
#include "StandardNeuron.hpp"

LayeredNetwork::~LayeredNetwork()
{

}

LayeredNetwork::LayeredNetwork(const LayeredNetworkOptions_t &options_)
{
	options = options_;

	// Add all neurons
	for (int l = 0; l < options.neuronsPerLayerCount.size(); l++)
	{
		// Add a new list for every layer
		neurons.push_back(std::list<Neuron*>());

		// Add the neurons to the current layer
		for (int i = 0; i < options.neuronsPerLayerCount[l]; i++)
		{
			// If its the first layer, add InputNeurons, else StandardNeurons
			if (l == 0)
				neurons.back().push_front(new InputNeuron());
			else
			{
				StandardNeuron* newNeuron = new StandardNeuron(options.inputFunction, options.activationFunction, options.outputFunction);
				neurons.back().push_front(newNeuron);

				// If its not the first layer connect the neuron of the current layer to all neurons of the previous layer

				// Get the last layer
				std::list<std::list<Neuron*>>::iterator lastLayer = neurons.end();
				lastLayer--;lastLayer--;

				// Add an edge to every neuron of the last layer
				for (std::list<Neuron*>::iterator prevNeuron = (*lastLayer).begin(); prevNeuron != (*lastLayer).end(); prevNeuron++)
				{
					newNeuron->addPrevNeuron(*prevNeuron, 0);
				}
			}
		}		
	}

		
}

std::list<Neuron*>* LayeredNetwork::getInputNeurons()
{
	// Return the first layer
	return &neurons.front();
}

std::list<Neuron*>* LayeredNetwork::getOutputNeurons()
{
	// Return the last layer
	return &neurons.back();
}

std::list<Neuron*>* LayeredNetwork::getNeuronsInLayer(int layerNr)
{	
	// Get the first layer
	std::list<std::list<Neuron*>>::iterator layer = neurons.begin();
	// Go to the layer with nr
	std::advance(layer, layerNr);

	return &(*layer);
}

std::list<std::list<Neuron*>>* LayeredNetwork::getNeurons()
{
	return &neurons;
}