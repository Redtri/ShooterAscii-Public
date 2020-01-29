#ifndef ASTAR_NODE_H
#define ASTAR_NODE_H

#include <windows.h>
#include <set>
#include <math.h>

#include "Utils.h"

/*
==============================================
	Base class defining an A* node structure 
==============================================
*/
class AstarNode
{
public:
	AstarNode();
	/*
	* Constructor with parameters.
	* -> pParent : (AstarNode*) the parent node to be associated to.
	* -> position : (COORD) node position.
	*/
	AstarNode(AstarNode *pParent, COORD position);

	/*
	* Mutator for node position.
	* -> pos : (COORD) new position.
	*/
	void setPos(COORD pos);
	/*
	* Mutator for node GCost.
	* -> cost : (short) new G cost.
	*/
	void setGCost(short cost);
	/*
	* Mutator for node HCost.
	* -> cost : (short) new H cost.
	*/
	void setHCost(short cost);
	/*
	* Mutator for the parent node reference.
	* -> pParent : (AstarNode*) new parent node address.
	*/
	void setParent(AstarNode *pParent);
	/*
	* Adds a new neighbour to the AstarNode* set containing the node neighbours.
	* -> neighbour : (AstarNode*) new neighbour to be added.
	*/
	void addNeighbour(AstarNode* neighbour);
	/*
	* Returns the node position.
	*/
	COORD getPos() const;
	/*
	* Returns the node parent address.
	*/
	AstarNode* getParent() const;
	/*
	* Returns the neighbours set of the current node.
	*/
	std::set<AstarNode*> getNeighbours();
	/*
	* Returns F cost.
	*/
	int getFCost() const;
	/*
	* Returns H cost.
	*/
	int getHCost() const;
	/*
	* Returns G cost.
	*/
	int getGCost() const;
	/*
	* Calculates the absolute value of the distance to another node.
	* -> other : (AstarNode) the other node on which the distance has to be evaluated.
	*/
	short getDistanceFrom(AstarNode const &other) const;

	/*
	========================================================
		Operators overloading
	========================================================
	*/
	bool operator==(AstarNode const &a) const;
	bool operator!=(AstarNode const &a) const;
	bool operator<(AstarNode const &a) const;

private:

	// current position
	COORD m_position;
	// parent node
	AstarNode *m_pParent;
	std::set<AstarNode*> m_pNeighbours;
	int m_gCost;
	int m_hCost;
};
#endif // !