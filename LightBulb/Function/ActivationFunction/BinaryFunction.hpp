#pragma once

#ifndef _BINARYFUNCTION_H_
#define _BINARYFUNCTION_H_

// Includes
#include "Function/ActivationFunction/AbstractActivationFunction.hpp"
#include <IO/UseParentSerialization.hpp>

namespace LightBulb
{
	/**
	 * \brief A binary function can decide between two values
	 * \details The function returns maxValue if the input > 0, otherwise it will return minValue.
	 */
	class BinaryFunction : public AbstractActivationFunction
	{
	private:
		/**
		 * \brief The minimum value
		 */
		double minValue;
		/**
		 * \brief The maximum value
		 */
		double maxValue;
	public:
		BinaryFunction(double minValue_ = 0, double maxValue_ = 1);
		// Inherited:
		double execute(double input) override;
		double executeDerivation(double input) override;
		AbstractActivationFunction* getActivationFunctionCopy() override;
		double getMaximum() override;
		double getMinimum() override;
		bool hasAMaxAndMinimum() override;
	};
}

EMPTY_CHILD_SERIALIZATION(LightBulb::BinaryFunction, LightBulb);

#endif