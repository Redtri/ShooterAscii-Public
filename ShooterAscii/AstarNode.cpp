#include "AstarNode.h"

AstarNode::AstarNode(AstarNode *pParent, COORD position) :
	m_pParent(pParent), m_position(position)
{
}
AstarNode::AstarNode() :
	m_pParent(nullptr), m_position(COORD({ 0,0 }))
{
}

void AstarNode::setPos(COORD pos)
{
	m_position = pos;
}

void AstarNode::setGCost(short cost)
{
	m_gCost = cost;
}

void AstarNode::setHCost(short cost)
{
	m_hCost = cost;
}

void AstarNode::setParent(AstarNode * pParent)
{
	m_pParent = pParent;
}

void AstarNode::addNeighbour(AstarNode* neighbour)
{
	m_pNeighbours.insert(neighbour);
}

COORD AstarNode::getPos() const
{
	return m_position;
}

AstarNode * AstarNode::getParent() const
{
	return (m_pParent) ? m_pParent : nullptr;
}

std::set<AstarNode*> AstarNode::getNeighbours()
{
	return m_pNeighbours;
}

int AstarNode::getFCost() const
{
	return m_gCost + m_hCost;
}

int AstarNode::getHCost() const
{
	return m_hCost;
}

int AstarNode::getGCost() const
{
	return m_gCost;
}

short AstarNode::getDistanceFrom(AstarNode const & other) const
{
	SHORT x = abs(other.getPos().X - m_position.X);
	SHORT y = abs(other.getPos().Y - m_position.Y);

	COORD tmp = COORD({ x,y });

	if (tmp.X > tmp.Y)
		return tmp.Y + (tmp.X - tmp.Y);
	return tmp.X + (tmp.Y-tmp.X);
}

bool AstarNode::operator==(AstarNode const & a) const
{
	return a.getPos().X == getPos().X && a.getPos().Y == getPos().Y;
}

bool AstarNode::operator!=(AstarNode const & a) const
{
	return !(*this == a);
}

bool AstarNode::operator<(AstarNode const & a) const
{
	if (getFCost() == a.getFCost()) {
		if (m_hCost == a.getHCost()) {
			return (m_position < a.getPos());
		} else {
			return (m_hCost < a.getHCost());
		}
	} else {
		return getFCost() < a.getFCost();
	}
}
