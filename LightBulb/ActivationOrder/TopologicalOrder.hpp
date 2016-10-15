#pragma once

#ifndef _TOPOLOGICALORDER_H_
#define _TOPOLOGICALORDER_H_

// Includes
#include "ActivationOrder/AbstractActivationOrder.hpp"

namespace LightBulb
{
	/**
	 * \brief Activates neurons layer by layer. This class is only compatible with a layered network!
	 */
	class TopologicalOrder : public AbstractActivationOrder
	{
	private:
	public:
		/**
		 * \brief Activates all neurons in the given layered(!) topology in a topological order.
		 * \param networkTopology The network to activate
		 */
		void executeActivation(AbstractNetworkTopology &networkTopology) const override;
		// Inherited:
		AbstractActivationOrder* getCopy() const override;
		std::unique_ptr<std::map<Edge*, bool>> getSameTimestepEdges(const AbstractNetworkTopology &networkTopology) const override;
	};
}

#endif