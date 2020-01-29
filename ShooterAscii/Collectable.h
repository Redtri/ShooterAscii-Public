#pragma once
#include "Entity.h"

/*
===============================================================================
	Objet collectable (currently only to regen HP)
===============================================================================
*/
class Collectable :
	public Entity
{
public:
	Collectable(SHORT x, SHORT y, char skin, short regen);

	/**
	 * Return the number of HP this entity regenerate by collecting it.
	 */
	short getRegen() const;
	/**
	 * Return true.
	 */
	bool isWalkable() const;
	/**
	 * Return true (for entity classes that don't extends from Collectable, isCollectable returns false)
	 */
	bool isCollectable() const;

private:
	short m_regen;
};

