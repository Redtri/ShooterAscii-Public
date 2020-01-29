#ifndef ASTAR_H
#define ASTAR_H

#include <set>
#include <map>
#include <vector>
#include <math.h>

#include "AstarNode.h"
#include "Utils.h"

#define PATH_ITERATIONS 50
/*
=============================================================================================================
	Manages pathfinding operations over several iterations considering obstacles and A* algorithm
=============================================================================================================
*/
class Astar
{
public:
	Astar();
	~Astar();

	/*
	* Mutators for starting node.
	*
	* Address : (Will only be called with the Monster class constructor associated).
	* -> pStart : (AstarNode*) new node address where the path starts from.
	*
	* Value : (Will be called each time the start node is being updated, avoiding deleting pointers too often).
	* -> start : (AstarNode) new node where the path starts from.
	*/
	void setStart(AstarNode *pStart);
	void setStart(AstarNode start);
	/*
	* Mutators for destination node.
	*
	* Address : (Will only be called with the Monster class constructor associated).
	* -> pDestination : (AstarNode*) new node address where the path ends to.
	*
	* Value : (Will be called each time the destination node is being updated, avoiding deleting pointers too often).
	* -> destination : (AstarNode) new node where the path ends to.
	*/
	void setDestination(AstarNode *pDestination);
	void setDestination(AstarNode destination);
	/*
	* Returns starting node's address.
	*/
	AstarNode* getStart() const;
	/*
	* Returns destination node's address.
	*/
	AstarNode* getDest() const;
	/*
	* Returns whether the path calculating process is over or not.
	*/
	bool const isCalculated() const;
	/*
	* Resets path data (such as allocated nodes, path vector...).
	*/
	void reset();
	/*
	* Calculates the nodes composing the path to be followed.
	* -> blockedTiles : (std::set<COORD>) set of obstacles to be taken into account for the path.
	*/
	void makePath(std::set<COORD> const &blockedTiles);
	/*
	* Retraces all the nodes composing the path from destination to start to populate m_path.
	* -> dest : (AstarNode*) last node iterated to be tested as the destination node .
	*/
	void retracePath(AstarNode *dest);
	/*
	* Iterating over the next node to be followed, decrementing the current node index.
	*/
	COORD nextNode();
private:
	/*
	* Returning an AstarNode* set of an AstarNode neighbours.
	* -> node : (AstarNode*) node pointer.
	* -> blockedTiles : (std::set<COORD*, ComparePointers<COORD>>) the coordinates that are blocked and cannot be designated as nodes.
	*/
	std::set<AstarNode*> getNeighbours(AstarNode *node, std::set<COORD*, ComparePointers<COORD>> const &blockedTiles);

	AstarNode *m_pStartNode;	
	AstarNode *m_pDestNode;
	int m_currentNodeIndex;
	bool m_calculated;
	std::vector<AstarNode*> m_path;
	//Nodes set containing nodes that need to be treated.
	std::set<AstarNode*, ComparePointers<AstarNode>> m_openSet;
	//Nodes set containing nodes that are already treated.
	std::set<AstarNode*> m_closedSet;
	//Nodes map containing each allocated node by the class.
	std::map<COORD, AstarNode*> m_nodes;
};
#endif // !1