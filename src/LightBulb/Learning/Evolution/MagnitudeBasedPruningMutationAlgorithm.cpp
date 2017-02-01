// Includes
#include "LightBulb/LightBulbPrec.hpp"
#include "LightBulb/Learning/Evolution/MagnitudeBasedPruningMutationAlgorithm.hpp"
#include "LightBulb/Learning/Evolution/AbstractIndividual.hpp"
#include "LightBulb/NeuralNetwork/AbstractNeuralNetwork.hpp"
#include "LightBulb/NetworkTopology/AbstractNetworkTopology.hpp"
#include "LightBulb/LinearAlgebra/Matrix.hpp"

namespace LightBulb
{
	MagnitudeBasedPruningMutationAlgorithm::MagnitudeBasedPruningMutationAlgorithm(int removeNeuronsPerIteration_, int removeWeightsPerIteration_, bool useRandomFunction_, bool ignoreInputLayer_, bool removeNeuronsByTheirTotalWeight_)
	{
		removeNeuronsPerIteration = removeNeuronsPerIteration_;
		removeWeightsPerIteration = removeWeightsPerIteration_;
		removeNeuronsByTheirTotalWeight = removeNeuronsByTheirTotalWeight_;
		useRandomFunction = useRandomFunction_;
		ignoreInputLayer = ignoreInputLayer_;
	}

	void MagnitudeBasedPruningMutationAlgorithm::execute(AbstractIndividual& individual1)
	{
		AbstractNetworkTopology& networkTopology = individual1.getNeuralNetwork().getNetworkTopology();
		for (int n = 0; n < removeNeuronsPerIteration; n++)
		{
			bool usesBiasNeuron = networkTopology.usesBiasNeuron();
			std::vector<std::tuple<int, int, int>> neuronRanking;


			if (removeNeuronsByTheirTotalWeight)
			{
				auto weights = networkTopology.getAllWeights();
				int layerIndex = 0;
				for (auto layer = weights.begin(); layer != weights.end(); layer++, layerIndex++)
				{
					if (layerIndex > 0 || !ignoreInputLayer)
					{
						if (networkTopology.getNeuronCountsPerLayer()[layerIndex] > 1)
						{
							auto weightSums = layer->getEigenValue().cwiseAbs().colwise().sum();
							for (int i = usesBiasNeuron; i < weightSums.cols(); i++)
							{
								neuronRanking.push_back(std::make_tuple(weightSums[i], layerIndex, i));
							}
						}
					}
				}
			}
			else
			{
				auto weights = networkTopology.getAllWeights();
				int layerIndex = 0;
				for (auto layer = weights.begin(); layer != weights.end(); layer++, layerIndex++)
				{
					if (layerIndex > 0 || !ignoreInputLayer)
					{
						if (networkTopology.getNeuronCountsPerLayer()[layerIndex] > 1)
						{
							for (int i = usesBiasNeuron; i < layer->getEigenValue().cols(); i++)
							{
								int weightCount = 0;
								for (int j = 0; j < layer->getEigenValue().rows(); j++)
								{
									if (networkTopology.existsAfferentWeight(layerIndex, i, j))
										weightCount++;
								}
								neuronRanking.push_back(std::make_tuple(weightCount, layerIndex, i));
							}
						}
					}
				}
			}

			if (neuronRanking.size() > 0) {

				sort(neuronRanking.begin(), neuronRanking.end(), std::less<std::tuple<int, int, int>>());

				int selectedIndex = 0;
				if (useRandomFunction)
					selectedIndex = randomFunction.execute(neuronRanking.size());

				individual1.removeNeuron(std::get<1>(neuronRanking[selectedIndex]), std::get<2>(neuronRanking[selectedIndex]));
			}
		}

		for (int w = 0; w < removeWeightsPerIteration; w++)
		{
			double minimalWeight = -1;
			int minimalWeightIndex = -1;
			int minimalWeightNeuronIndex = -1;
			int minimalWeightLayerIndex = -1;

			auto weights = networkTopology.getAllWeights();
			int layerIndex = 0;
			for (auto layer = weights.begin(); layer != weights.end(); layer++, layerIndex++)
			{
				for (int i = 0; i < layer->getEigenValue().rows(); i++)
				{
					for (int j = 0; j < layer->getEigenValue().cols(); j++)
					{
						if (networkTopology.existsAfferentWeight(layerIndex, j, i) && (abs((*layer).getEigenValue()(i, j)) < minimalWeight || minimalWeight == -1))
						{
							minimalWeight = abs((*layer).getEigenValue()(i, j));
							minimalWeightLayerIndex = layerIndex;
							minimalWeightIndex = i;
							minimalWeightNeuronIndex = j;
						}
					}
				}
			}

			if (minimalWeightLayerIndex != -1)
				networkTopology.removeAfferentWeight(minimalWeightLayerIndex, minimalWeightNeuronIndex, minimalWeightIndex);
		}


	}

	void MagnitudeBasedPruningMutationAlgorithm::setRandomGenerator(AbstractRandomGenerator& randomGenerator_)
	{
		AbstractRandomGeneratorUser::setRandomGenerator(randomGenerator_);
		randomFunction.setRandomGenerator(randomGenerator_);
	}

	AbstractCloneable* MagnitudeBasedPruningMutationAlgorithm::clone() const
	{
		return new MagnitudeBasedPruningMutationAlgorithm(*this);
	}
}
