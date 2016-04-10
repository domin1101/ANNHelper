#pragma once

#ifndef _LEARNINGSTATEIO_H_
#define _LEARNINGSTATEIO_H_

// Libary includes
#include <cereal/types/map.hpp>
#include <cereal/cereal.hpp>
#include <cereal/access.hpp>
#include <cereal/types/polymorphic.hpp>
#include <Learning/LearningState.hpp>

template <class Archive>
void serialize(Archive& archive, LearningState& learningState)
{
	archive(cereal::make_nvp("dataSaveInterval", learningState.dataSaveInterval));
	archive(cereal::make_nvp("dataSets", learningState.dataSets));
	archive(cereal::make_nvp("iterationsNeeded", learningState.iterationsNeeded));
	archive(cereal::make_nvp("quality", learningState.quality));
	archive(cereal::make_nvp("successful", learningState.successful));
}

namespace cereal
{
	template <> struct LoadAndConstruct<LearningState>
	{
		template <class Archive>
		static void load_and_construct(Archive & ar, cereal::construct<LearningState>& construct)
		{
			int dataSaveInterval;
			ar(cereal::make_nvp("dataSaveInterval", dataSaveInterval));
			construct(dataSaveInterval);

			ar(cereal::make_nvp("dataSets", construct->dataSets));
			ar(cereal::make_nvp("iterationsNeeded", construct->iterationsNeeded));
			ar(cereal::make_nvp("quality", construct->quality));
			ar(cereal::make_nvp("successful", construct->successful));
		}
	};
}

#endif