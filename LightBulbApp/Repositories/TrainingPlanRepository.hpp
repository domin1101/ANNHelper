#pragma once

#ifndef _TRAININGPLANREPOSITORY_H_
#define _TRAININGPLANREPOSITORY_H_

// Library includes
#include <vector>
#include <Event/Observable.hpp>
#include <memory>
#include <TrainingPlans/AbstractTrainingPlan.hpp>

// Includes

// Forward declarations

enum TrainingPlanRepositoryEvents
{
	EVT_TP_CHANGED
};

class TrainingPlanRepository : public Observable<TrainingPlanRepositoryEvents, TrainingPlanRepository>
{
	template <class Archive>
	friend void load(Archive& archive, TrainingPlanRepository& trainingPlanRepository);
	template <class Archive>
	friend void save(Archive& archive, TrainingPlanRepository const& trainingPlanRepository);
private:
	std::vector<std::unique_ptr<AbstractTrainingPlan>> trainingPlans;
public:
	std::vector<std::unique_ptr<AbstractTrainingPlan>>* getTrainingPlans();
	int getIndexOfTrainingPlan(AbstractTrainingPlan* trainingPlan);
	void Add(AbstractTrainingPlan* trainingPlan);
	void save(std::string path, int trainingPlanIndex);
	AbstractTrainingPlan* load(std::string path, AbstractLogger* logger);
};

#endif
