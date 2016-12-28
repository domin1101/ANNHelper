// Includes
#include "LightBulb/Learning/Evolution/EvolutionStrategy/RecombinationAlgorithm.hpp"
#include "LightBulb/Learning/Evolution/AbstractIndividual.hpp"
#include "LightBulb/NeuralNetwork/NeuralNetwork.hpp"
#include "LightBulb/NetworkTopology/AbstractNetworkTopology.hpp"
#include "LightBulb/NetworkTopology/FeedForwardNetworkTopology.hpp"
#include "LightBulb/LinearAlgebra/Matrix.hpp"

namespace LightBulb
{
	RecombinationAlgorithm::RecombinationAlgorithm(bool useAverageForWeight_, bool useAverageForMutationStrength_)
	{
		useAverageForWeight = useAverageForWeight_;
		useAverageForMutationStrength = useAverageForMutationStrength_;
	}

	void RecombinationAlgorithm::execute(AbstractIndividual& individual1, AbstractIndividual& individual2)
	{
		std::vector<Matrix>& weights1 = static_cast<FeedForwardNetworkTopology&>(individual1.getNeuralNetwork().getNetworkTopology()).getAllWeights();
		std::vector<Matrix>& weights2 = static_cast<FeedForwardNetworkTopology&>(individual2.getNeuralNetwork().getNetworkTopology()).getAllWeights();

		auto layer1 = weights1.begin();
		auto layer2 = weights2.begin();
		for (; layer1 != weights1.end(); layer1++, layer2++)
		{
			for (int i = 0; i < layer1->getEigenValue().rows(); i++)
			{
				for (int j = 0; j < layer1->getEigenValue().cols(); j++)
				{
					if (i < layer2->getEigenValue().rows() && j < layer2->getEigenValue().cols())
					{
						if (useAverageForWeight)
						{
							// Calculate the weights average and store it inside the first individual
							(*layer1).getEigenValueForEditing()(i, j) = ((*layer1).getEigenValue()(i, j) + (*layer2).getEigenValue()(i, j)) / 2;
						}
						else
						{
							if (randomGenerator->randDouble() > 0.5)
								(*layer1).getEigenValueForEditing()(i, j) = (*layer2).getEigenValue()(i, j);
						}
					}
				}
			}
		}


		std::vector<double>& mutationStrength1 = individual1.getMutationStrength();
		std::vector<double>& mutationStrength2 = individual2.getMutationStrength();
		for (int i = 0; i < mutationStrength2.size() && i < mutationStrength1.size(); i++)
		{
			if (useAverageForMutationStrength)
			{
				mutationStrength1[i] = (mutationStrength2[i] + mutationStrength1[i]) / 2;
			}
			else
			{
				if (randomGenerator->randDouble() > 0.5)
					mutationStrength1[i] = mutationStrength2[i];
			}
		}
	}

	AbstractCloneable* RecombinationAlgorithm::clone() const
	{
		return new RecombinationAlgorithm(*this);
	}
}
