// Includes
#include "Function/ActivationFunction/GaussianRBFFunction.hpp"
#include "NetworkTopology/AbstractSOMStructure.hpp"
// Library includes
#include <limits>
#include <stdexcept>
#include <EigenSrc/Dense>

namespace LightBulb
{
	GaussianRBFFunction::GaussianRBFFunction(Eigen::VectorXd* widths_)
	{
		widths = widths_;
	}

	void GaussianRBFFunction::execute(int layerNr, std::vector<Eigen::VectorBlock<Eigen::VectorXd>>& activations, const std::vector<Eigen::VectorXd>& netInputs) const
	{
		activations[layerNr] = (-1 * netInputs[layerNr].array().pow(2) / (2 * widths->array().pow(2))).exp();
	}

	double GaussianRBFFunction::execute(double input) const
	{
		throw std::exception("Currently not working!");
		// Return the value of the gaussian rbf: e^(-input^2/(2*width^2)
		//return exp(-pow(input, 2) / (2 * pow(rbfThreshold->getWidth(), 2)));
	}

	double GaussianRBFFunction::executeDerivation(double input) const
	{
		// TODO: Implement the derivation even if it is not used
		return 0;

	}

	AbstractActivationFunction* GaussianRBFFunction::getActivationFunctionCopy() const
	{
		return new GaussianRBFFunction(*this);
	}

	double GaussianRBFFunction::getMaximum() const
	{
		throw std::logic_error("A GaussianRBFFunction does not have a maximum");
	}

	double GaussianRBFFunction::getMinimum() const
	{
		throw std::logic_error("A GaussianRBFFunction does not have a minimum");
	}

	bool GaussianRBFFunction::hasAMaxAndMinimum() const
	{
		return true;
	}

	double GaussianRBFFunction::execute(StandardNeuron* neuron, StandardNeuron* activatedNeuron, AbstractSOMStructure* structure, NeuronCompareThreshold* threshold, double maxDistance) const
	{
		return exp(-pow((*structure->getNeighborhoodDistances())[neuron][activatedNeuron], 2) / (2 * pow(maxDistance, 2)));
	}

	AbstractNeighborhoodFunction* GaussianRBFFunction::getNeighborhoodFunctionCopy() const
	{
		return new GaussianRBFFunction(*this);
	}
}