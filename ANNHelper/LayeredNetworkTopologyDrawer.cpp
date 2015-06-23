#include "LayeredNetworkTopologyDrawer.hpp"
#include "AbstractNeuron.hpp"
#include "StandardNeuron.hpp"
#include "AbstractNetworkTopology.hpp"
#include "Edge.hpp"
#include <exception>


LayeredNetworkTopologyDrawer::LayeredNetworkTopologyDrawer( AbstractNetworkTopologyDrawerOptions& options_)
	: AbstractNetworkTopologyDrawer(options_)
{

}

void LayeredNetworkTopologyDrawer::refresh()
{
	// Check if all given options are correct
	// Check if the given topology is valud
	if (options->networkTopology == NULL)
		throw std::invalid_argument("The given networkTopology is not valid!");

	// Calculate the offset between two layers (The +2 is needed for input neurons and a border)
	int layerOffset = options->width / (options->networkTopology->getNeurons()->size() + 2);	
	// Go through all layers in the given networkTopology
	int layerIndex = 0;

	{
		// Calculate the offset between two neurons (The +1 is needed for a border)
		int neuronOffset = options->height / (options->networkTopology->getInputNeurons()->size() + 1);
		// Go through all neurons in this layer
		int neuronIndex = 0;
		for (std::vector<AbstractNeuron*>::iterator neuron = options->networkTopology->getInputNeurons()->begin(); neuron != options->networkTopology->getInputNeurons()->end(); neuron++, neuronIndex++)
		{
			addShapeFromNeuron(*neuron, sf::Vector2f((layerIndex + 1) * layerOffset, (neuronIndex + 1) * neuronOffset));
		}
	}

	layerIndex++;

	for (std::vector<std::vector<StandardNeuron*>>::iterator layer = options->networkTopology->getNeurons()->begin(); layer != options->networkTopology->getNeurons()->end(); layer++, layerIndex++)
	{
		// Calculate the offset between two neurons (The +1 is needed for a border)
		int neuronOffset = options->height / (layer->size() + 1);
		// Go through all neurons in this layer
		int neuronIndex = 0;
		for (std::vector<StandardNeuron*>::iterator neuron = layer->begin(); neuron != layer->end(); neuron++, neuronIndex++)
		{
			addShapeFromNeuron(*neuron,	sf::Vector2f((layerIndex + 1) * layerOffset, (neuronIndex + 1) * neuronOffset));
		}
	}

	addEdgesToAllShapes();
}
