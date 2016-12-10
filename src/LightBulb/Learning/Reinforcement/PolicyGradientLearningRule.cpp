// Includes
#include "LightBulb/Learning/Reinforcement/PolicyGradientLearningRule.hpp"
#include "LightBulb/NeuralNetwork/NeuralNetwork.hpp"
#include "LightBulb/NetworkTopology/AbstractNetworkTopology.hpp"
#include "LightBulb/NetworkTopology/FeedForwardNetworkTopology.hpp"
#include <iostream>
#include <algorithm>
#include <map>
#include <string>
#include "LightBulb/Learning/Evolution/EvolutionLearningResult.hpp"
#include "LightBulb/Learning/Evolution/EvolutionLearningRule.hpp"
#include "LightBulb/Learning/Reinforcement/AbstractReinforcementEnvironment.hpp"
#include "LightBulb/NeuronDescription/NeuronDescription.hpp"
#include "LightBulb/Learning/Supervised/GradientCalculation/Backpropagation.hpp"
#include "LightBulb/Learning/Supervised/GradientDescentAlgorithms/RMSPropLearningRate.hpp"
#include "LightBulb/NeuronDescription/DifferentNeuronDescriptionFactory.hpp"
#include "LightBulb/Function/InputFunction/WeightedSumFunction.hpp"
#include "LightBulb/Function/ActivationFunction/RectifierFunction.hpp"
#include "LightBulb/ActivationOrder/TopologicalOrder.hpp"

namespace LightBulb
{
	AbstractLearningResult* PolicyGradientLearningRule::getLearningResult()
	{
		EvolutionLearningResult* learningResult = new EvolutionLearningResult();

		return learningResult;
	}


	PolicyGradientLearningRule::PolicyGradientLearningRule(PolicyGradientLearningRuleOptions& options_)
		: AbstractReinforcementLearningRule(new PolicyGradientLearningRuleOptions(options_))
	{
		initialize(static_cast<PolicyGradientLearningRuleOptions&>(*options.get()));
	}

	PolicyGradientLearningRule::PolicyGradientLearningRule(PolicyGradientLearningRuleOptions* options_)
		: AbstractReinforcementLearningRule(options_)
	{
		initialize(*options_);
	}

	PolicyGradientLearningRule::PolicyGradientLearningRule()
		: AbstractReinforcementLearningRule(new PolicyGradientLearningRuleOptions())
	{
		initialize(static_cast<PolicyGradientLearningRuleOptions&>(*options.get()));
	}

	void PolicyGradientLearningRule::initialize(PolicyGradientLearningRuleOptions& options)
	{
		stepsSinceLastReward = 0;

		gradientCalculation.reset(new Backpropagation());
		gradientDescentAlgorithm.reset(new RMSPropLearningRate(options.rmsPropLearningRateOptions));

		valueFunctionGradientCalculation.reset(new Backpropagation());
		valueFunctionGradientDescentAlgorithm.reset(new RMSPropLearningRate(options.valueRmsPropLearningRateOptions));
	}

	void PolicyGradientLearningRule::initializeLearningAlgoritm()
	{
		gradientDescentAlgorithm->initialize(getOptions().environment->getNeuralNetwork().getNetworkTopology());
		stateRecord.resize(1000);
		gradientRecord.resize(1000);

		lastOutput.resize(getOptions().environment->getNeuralNetwork().getNetworkTopology().getOutputSize());
		if (getOptions().valueFunctionAsBase)
		{
			FeedForwardNetworkTopologyOptions options;
			options.enableShortcuts = false;
			options.useBiasNeuron = true;

			options.neuronsPerLayerCount.push_back(6);
			options.neuronsPerLayerCount.push_back(256);
			options.neuronsPerLayerCount.push_back(1);

			options.descriptionFactory = new DifferentNeuronDescriptionFactory(new NeuronDescription(new WeightedSumFunction(), new RectifierFunction()), new NeuronDescription(new WeightedSumFunction(), new IdentityFunction()));
			
			valueFunctionNetwork.reset(new NeuralNetwork(new FeedForwardNetworkTopology(options)));
			valueFunctionGradientDescentAlgorithm->initialize(valueFunctionNetwork->getNetworkTopology());
		}
	}

	void PolicyGradientLearningRule::recordStep(AbstractNetworkTopology& networkTopology)
	{
		Eigen::VectorXd errorVector;
		getErrorVector(networkTopology, errorVector);
		
		auto patternVector = networkTopology.getActivationsPerLayer(0);
		stateRecord[stepsSinceLastReward] = std::vector<double>(patternVector.data() + networkTopology.usesBiasNeuron(), patternVector.data() + patternVector.size());
		
		if (gradientRecord[stepsSinceLastReward].empty())
			gradientRecord[stepsSinceLastReward] = networkTopology.getAllWeights();

		for (int i = 0; i < gradientRecord[stepsSinceLastReward].size(); i++)
			gradientRecord[stepsSinceLastReward][i].setZero();
		
		gradientCalculation->calcGradient(networkTopology, errorVector, gradientRecord[stepsSinceLastReward]);
	}

	void PolicyGradientLearningRule::getErrorVector(AbstractNetworkTopology& networkTopology, Eigen::VectorXd& errorVector)
	{
		networkTopology.getOutput(lastOutput);

		errorVector.resize(lastOutput.size());
		for (int i = 0; i < lastOutput.size(); i++)
		{
			errorVector(i) = getOptions().environment->getLastBooleanOutput().at(i) - lastOutput[i];//-2 * std::signbit(getOptions()->environment->getLastBooleanOutput()[i] - lastOutput[i]) + 1;
		}
		errorSum += errorVector.cwiseAbs().sum();
		errorSteps++;
	}

	void PolicyGradientLearningRule::initializeTry()
	{
		//resilientLearningRateHelper->initialize(*getOptions()->environment->getNeuralNetwork());
		getOptions().environment->setLearningState(*learningState.get());
		getOptions().environment->initializeForLearning();

		stepsSinceLastReward = 0;

		if (getOptions().valueFunctionAsBase)
		{
			valueFunctionNetwork->getNetworkTopology().randomizeDependingOnLayerSize(*randomGenerator.get());
		}
	}

	
	std::string PolicyGradientLearningRule::getName()
	{
		return "PolicyGradientLearningRule";
	}

	std::vector<std::string> PolicyGradientLearningRule::getDataSetLabels() const
	{
		std::vector<std::string> labels = AbstractReinforcementLearningRule::getDataSetLabels();
		labels.push_back(DATA_SET_ERROR_AVG);
		if (getOptions().valueFunctionAsBase)
			labels.push_back(DATA_SET_VALUE_ERROR_AVG);
		return labels;
	}


	void PolicyGradientLearningRule::doIteration()
	{
		int rewardCounter = 0;
		double totalReward = 0;
		errorSum = 0;
		valueErrorSum = 0;
		errorSteps = 0;
		valueErrorSteps = 0;
		AbstractNetworkTopology& networkTopology = getOptions().environment->getNeuralNetwork().getNetworkTopology();
		gradient.clear();
		if (getOptions().valueFunctionAsBase)
			valueFunctionGradientCalculation->initGradient(valueFunctionNetwork->getNetworkTopology());

		while (rewardCounter < getOptions().episodeSize)
		{
			double reward = getOptions().environment->doSimulationStep();

			recordStep(networkTopology);

			stepsSinceLastReward++;

			if (getOptions().environment->isTerminalState())
			{
				totalReward += reward;

				computeGradients(stepsSinceLastReward, reward);
				stepsSinceLastReward = 0;
				rewardCounter++;
			}
		}

		learningState->addData(DATA_SET_REWARD, totalReward);

		learningState->addData(DATA_SET_ERROR_AVG, errorSum / errorSteps);
		if (getOptions().valueFunctionAsBase)
			learningState->addData(DATA_SET_VALUE_ERROR_AVG, valueErrorSum / valueErrorSteps);

		addGradients(networkTopology);

		// Continue with the next generation
		learningState->iterations++;

		if (learningState->iterations % getOptions().ratingInterval == 0)
		{
			getOptions().environment->setStochasticActionDecision(false);
			getOptions().environment->rate();
			getOptions().environment->setStochasticActionDecision(true);
		}
	}


	void PolicyGradientLearningRule::addGradients(AbstractNetworkTopology& networkTopology)
	{
		for (int l = gradient.size() - 1; l >= 0; l--)
		{
			Eigen::MatrixXd newWeights = networkTopology.getAfferentWeightsPerLayer(l + 1) + gradientDescentAlgorithm->calcDeltaWeight(networkTopology, l + 1, gradient[l]);
			networkTopology.setAfferentWeightsPerLayer(l + 1, newWeights);
		}

		if (getOptions().valueFunctionAsBase)
		{
			for (int l = valueFunctionGradientCalculation->getGradient().size() - 1; l >= 0; l--)
			{
				Eigen::MatrixXd newWeights = valueFunctionNetwork->getNetworkTopology().getAfferentWeightsPerLayer(l + 1) + valueFunctionGradientDescentAlgorithm->calcDeltaWeight(valueFunctionNetwork->getNetworkTopology(), l + 1, valueFunctionGradientCalculation->getGradient().at(l));
				valueFunctionNetwork->getNetworkTopology().setAfferentWeightsPerLayer(l + 1, newWeights);
			}
		}
	}

	const PolicyGradientLearningRuleOptions& PolicyGradientLearningRule::getOptions() const
	{
		return static_cast<PolicyGradientLearningRuleOptions&>(*options.get());
	}

	void PolicyGradientLearningRule::doCalculationAfterLearningProcess()
	{
	}

	void PolicyGradientLearningRule::computeGradients(int stepsSinceLastReward, double reward)
	{
		Eigen::VectorXd rewards(stepsSinceLastReward);
		rewards(stepsSinceLastReward - 1) = reward;
		for (int i = stepsSinceLastReward - 2; i >= 0; i--)
		{
			rewards(i) = rewards(i + 1) * 0.99;
		}

	/*	rewards = rewards.array() - rewards.mean();
		double stddev = std::sqrt(rewards.cwiseAbs2().sum() / stepsSinceLastReward);
		rewards = rewards.array() / stddev;*/

		for (int i = 0; i < stepsSinceLastReward; i++)
		{
			if (getOptions().valueFunctionAsBase)
			{
				std::vector<double> output(1);
				valueFunctionNetwork->calculate(stateRecord[i], output, TopologicalOrder());

				Eigen::VectorXd errorVector(1);
				errorVector(0) = rewards(i) - output[0];
				valueFunctionGradientCalculation->calcGradient(valueFunctionNetwork->getNetworkTopology(), errorVector);
				valueErrorSum += abs(errorVector(0));
				valueErrorSteps++;

				rewards(i) -= output[0];
			}
			
			if (gradient.empty())
			{
				for (int j = 0; j < gradientRecord[i].size(); j++)
					gradientRecord[i][j].noalias() = gradientRecord[i][j] * rewards(i);
				gradient = gradientRecord[i];
			}
			else
			{
				for (int j = 0; j < gradientRecord[i].size(); j++)
					gradient[j].noalias() = gradient[j] + gradientRecord[i][j] * rewards(i);
			}
		}

	}


}
