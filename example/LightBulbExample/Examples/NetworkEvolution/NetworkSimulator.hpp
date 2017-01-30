#pragma once

#ifndef _NETWORKSIMULATOR_H_
#define _NETWORKSIMULATOR_H_

// Library Includes
#include <vector>
#include <map>

// Include
#include "LightBulb/Learning/Evolution/AbstractSimpleEvolutionEnvironment.hpp"

// Forward declarations

class NetworkSimulator : public LightBulb::AbstractSimpleEvolutionEnvironment
{
	template <class Archive>
	friend void save(Archive& archive, NetworkSimulator const& environment);
	template <class Archive>
	friend void load(Archive& archive, NetworkSimulator& environment);
	friend struct cereal::LoadAndConstruct<NetworkSimulator>;
protected:
	LightBulb::Vector<> consumers;
	LightBulb::AbstractIndividual* createNewIndividual() override;
	//sf::RenderWindow window;
	//std::unique_ptr<NetworkDrawer> drawer;
	double distanceBetweenPositions(float pos1X, float pos1Y, float pos2X, float pos2Y) const;
public:
	NetworkSimulator(const LightBulb::Vector<> consumers_);
	NetworkSimulator() = default;
	void doSimulationStep() override;
	void getFitness(const LightBulb::AbstractIndividual& individual, LightBulb::Scalar<>& fitness) const override;
	LightBulb::Vector<>& getConsumers();
};

#include "IO/NetworkSimulatorIO.hpp"


#endif
