#include "Astar.h"
#include <iostream>

Astar::Astar() :
	m_pStartNode(nullptr), m_pDestNode(nullptr)
{
}

Astar::~Astar()
{
	for (auto i : m_nodes) {
		delete i.second;
	}
	if (m_pStartNode) {
		delete m_pStartNode;
	}
	if (m_pDestNode) {
		delete m_pDestNode;
	}
}

void Astar::makePath(std::set<COORD> const &blockedTiles)
{
	std::set<COORD*, ComparePointers<COORD>> blocked;
	/*
	TODO
	for (auto& i : blockedTiles) {
		COORD pCoord = i;
		blocked.insert(&pCoord);
	}*/

	if (m_openSet.empty()) {
		m_openSet.insert(m_pStartNode);
	}
	//Path calculation has a maximum of iterations steps, if they are passed, the process will be continued to the next frame (avoiding performance issues)
	for (int i = 0; (i < PATH_ITERATIONS) && !m_calculated; ++i) {
		//Picking the less F costing node in openSet
		AstarNode *node = (*m_openSet.begin());

		//Adding it to the closedSet
		m_openSet.erase(node);
		m_closedSet.insert(node);

		//Break if destination is reached
		if (*node == *m_pDestNode) {
			retracePath(node);
			m_calculated = true;
			break;
		}

		//Iterating over each neighbour
		std::set<AstarNode*> neighbours = getNeighbours(node, blocked);
		for (auto neighbour : neighbours) {
			//If this neighbour has already been treated, skip to the next one
			if (m_closedSet.find(neighbour) != m_closedSet.end()) {
				continue;
			}
			short costToNeighbour = node->getGCost() + node->getDistanceFrom(*neighbour);
			//If this neighbour is not already being treated or has already been but is less costing than another path leading to the same node
			if (costToNeighbour < neighbour->getGCost() || m_openSet.find(neighbour) == m_openSet.end()) {
				//Its data are updated considering this new node
				neighbour->setGCost(costToNeighbour);
				neighbour->setHCost(neighbour->getDistanceFrom(*m_pDestNode));
				neighbour->setParent(node);

				if (m_openSet.find(neighbour) == m_openSet.end()) {
					m_openSet.insert(neighbour);
				}
			}
		}
	}
}

void Astar::retracePath(AstarNode *dest) {
	AstarNode *currentNode = dest;

	while (*currentNode != *m_pStartNode) { 
		m_path.push_back(currentNode);
		currentNode = currentNode->getParent();
	}
	m_currentNodeIndex = m_path.size();
}

COORD Astar::nextNode()
{
	if (m_path.empty()) {
		return m_pDestNode->getPos();
	}
	return (m_currentNodeIndex == 0) ? m_path.at(m_currentNodeIndex)->getPos() : m_path.at(--m_currentNodeIndex)->getPos();
}

std::set<AstarNode*> Astar::getNeighbours(AstarNode *node, std::set<COORD*, ComparePointers<COORD>> const &blockedTiles)
{
	//Trying to collect the node neighbours
	std::set<AstarNode*> tmp = node->getNeighbours();

	//If there's no neighbour for this node, we try to generate them
	if (tmp.empty()) {
		for (short i = -1; i <= 1; ++i) {
			for (short j = -1; j <= 1; ++j) {
				//For each of the 4 coordinates adjacent to this node
				if (!((i == 0 && j == 0) || (abs(i) == 1 && abs(j) == 1)) &&
					((node->getPos().Y + i) >= 0 && (node->getPos().Y + i) < 40) &&
					((node->getPos().X + j) >= 0 && (node->getPos().X + j) < 100)
					)
				{
					COORD coord = { node->getPos().X + j, node->getPos().Y + i };
					//Checking the walkable property of the coordinates
					bool test = false;

					for (auto tCoord : blockedTiles) {
						if (*tCoord == coord) {
							test = true;
							break;
						}
					}
					//If these coordinates are walkable (not contained inside blockedTiles)
					if (!test) {
						AstarNode *pNewNode = nullptr;
						auto i = m_nodes.find(coord);
						//Either a new neighbour node is being generated and added to both the allocated nodes map and the current node neighbours set
						if (i == m_nodes.end()) {
							pNewNode = new AstarNode(node, coord);
							node->addNeighbour(pNewNode);
							m_nodes.insert(std::make_pair(coord, pNewNode));
						}//Or it's simply added as a neighbour node if it already exists and is not parent of the current node
						else if (i->second != node->getParent()) {
							node->addNeighbour(i->second);
						}
					}
				}
			}
		}
		//Now that the node has neighbours, it's being stored
		tmp = node->getNeighbours();
	}
	return tmp;
}

void Astar::setStart(AstarNode * pStart)
{
	m_pStartNode = pStart;
}

void Astar::setStart(AstarNode start)
{
	*m_pStartNode = start;
}

void Astar::setDestination(AstarNode * pDestination)
{
	m_pDestNode = pDestination;
}

void Astar::setDestination(AstarNode destination)
{
	*m_pDestNode = destination;
}

AstarNode * Astar::getStart() const
{
	return m_pStartNode;
}

AstarNode * Astar::getDest() const
{
	return m_pDestNode;
}

bool const Astar::isCalculated() const
{
	return m_calculated;
}

void Astar::reset()
{
	m_calculated = false;
	for (auto i : m_nodes) {
		if (i.second != m_pStartNode && i.second != m_pDestNode) {
			delete i.second;
		}
	}
	m_nodes.clear();
	m_path.clear();
	m_openSet.clear();
	m_closedSet.clear();
}
