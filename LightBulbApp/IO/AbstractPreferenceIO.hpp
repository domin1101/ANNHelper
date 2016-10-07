#pragma once

#ifndef _ABSTRACTPREFERENCEIO_H_
#define _ABSTRACTPREFERENCEIO_H_

// Includes
#include "TrainingPlans/Preferences/AbstractPreference.hpp"
// Libraray includes
#include <cereal/cereal.hpp>

namespace LightBulb
{
	template <class Archive>
	void save(Archive& archive, AbstractPreference const& preference)
	{
		archive(cereal::make_nvp("name", preference.name));
	}

	template <class Archive>
	void load(Archive& archive, AbstractPreference& preference)
	{
		archive(cereal::make_nvp("name", preference.name));
	}
}

#endif