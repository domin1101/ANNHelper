#pragma once

#ifndef _ABSRACTACTIVATIONORDER_H_
#define _ABSTRACTACTIVATIONORDER_H_

// Library includes
#include <map>
#include <memory>

// Forward declarations
class AbstractNetworkTopology;
class Edge;

/**
 * \brief Describes the order in which neurons should get activated
 */
class AbstractActivationOrder
{
private:
public:
	virtual ~AbstractActivationOrder() {}
	/**
	 * \brief Activates all Neurons in the given topology in a special order
	 * \param networkTopology The network to activate
	 */
	virtual void executeActivation(AbstractNetworkTopology &networkTopology) = 0;
	/**
	 * \brief Returns a copy of this activation order
	 * \return The copy
	 */
	virtual AbstractActivationOrder* getCopy() = 0; 
	/**
	 * \brief Returns a map which holds for every edge, if its two neurons are refreshed in the same timestep (In a feed forward with topological acitvation order for example are all edges in the same timestep) TODO: Refactor!
	 * \param networkTopology 
	 * \return 
	 */
	virtual std::unique_ptr<std::map<Edge*, bool>> getSameTimestepEdges(AbstractNetworkTopology &networkTopology) = 0;
};

#endif
