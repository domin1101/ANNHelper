// Includes
#include "LightBulbExamplePrec.hpp"
#include "Examples/FunctionEvolution/Position.hpp"
#include "Examples/FunctionEvolution/FunctionSimulator.hpp"

using namespace LightBulb;

Position::Position(FunctionSimulator& functionSimulator_)
	: AbstractDefaultIndividual(functionSimulator_)
{
	functionSimulator = &functionSimulator_;

	FeedForwardNetworkTopologyOptions options;
	options.useBiasNeuron = false;
	options.neuronsPerLayerCount.push_back(1);
	options.neuronsPerLayerCount.push_back(2);
	options.descriptionFactory = new SameNeuronDescriptionFactory(new NeuronDescription(new WeightedSumFunction(), new IdentityFunction()));
	buildNeuralNetwork(options);
}

void Position::getNNInput(LightBulb::Vector<>& input)
{
	if (input.getEigenValue().size() != 1) {
		input.getEigenValueForEditing().resize(1);
		input.getEigenValueForEditing()[0] = 1;
	}
}

void Position::interpretNNOutput(const LightBulb::Vector<>& output)
{
	position = output;
}

const LightBulb::Vector<>& Position::getPosition() const
{
	return position;
}

Position::~Position()
{
}

