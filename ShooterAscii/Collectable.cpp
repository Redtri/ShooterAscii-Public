#include "Collectable.h"



Collectable::Collectable(SHORT x, SHORT y, char skin, short regen) :
	Entity(x, y, skin, 0x000B), m_regen(regen)
{
}

short Collectable::getRegen() const
{
	return m_regen;
}

bool Collectable::isWalkable() const
{
	return true;
}

bool Collectable::isCollectable() const
{
	return true;
}

