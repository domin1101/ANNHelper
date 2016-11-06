#pragma once

#ifndef _NETWORKSIMULATOR_H_
#define _NETWORKSIMULATOR_H_

// Library Includes
#include <vector>
#include <map>

// Include
#include "Learning/Evolution/AbstractSimpleEvolutionWorld.hpp"

// Forward declarations

class NetworkSimulator : public LightBulb::AbstractSimpleEvolutionWorld
{
	template <class Archive>
	friend void save(Archive& archive, NetworkSimulator const& world);
	template <class Archive>
	friend void load(Archive& archive, NetworkSimulator& world);
	friend struct cereal::LoadAndConstruct<NetworkSimulator>;
protected:
	std::vector<std::vector<float>> consumers;
	LightBulb::AbstractEvolutionObject* createNewObject() override;
	//sf::RenderWindow window;
	//std::unique_ptr<NetworkDrawer> drawer;
	double distanceBetweenPositions(const std::vector<float>& pos1, const std::vector<float>& pos2) const;
public:
	NetworkSimulator(std::vector<std::vector<float>> consumers_);
	NetworkSimulator() = default;
	bool doSimulationStep() override;
	double getScore(const LightBulb::AbstractEvolutionObject& object) const override;
	std::vector<std::vector<float>>& getConsumers();
};

#include "IO/NetworkSimulatorIO.hpp"


#endif