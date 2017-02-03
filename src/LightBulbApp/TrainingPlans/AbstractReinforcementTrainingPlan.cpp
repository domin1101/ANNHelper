﻿// Includes
#include "LightBulbApp/LightBulbAppPrec.hpp"
#include "LightBulbApp/TrainingPlans/AbstractReinforcementTrainingPlan.hpp"
#include "LightBulb/Learning/Reinforcement/AbstractReinforcementLearningRule.hpp"

namespace LightBulb
{
	void AbstractReinforcementTrainingPlan::fillDefaultLearningRuleOptions(AbstractReinforcementLearningRuleOptions& options) const
	{
		AbstractLearningRuleTrainingPlan::fillDefaultLearningRuleOptions(options);
		options.environment = environment.get();
	}

	AbstractReinforcementTrainingPlan::AbstractReinforcementTrainingPlan()
	{
		environment = nullptr;
	}

	void AbstractReinforcementTrainingPlan::initializeStart()
	{
		environment.reset(createEnvironment());
	}

	AbstractReinforcementEnvironment& AbstractReinforcementTrainingPlan::getEnvironment()
	{
		return *environment.get();
	}

}
