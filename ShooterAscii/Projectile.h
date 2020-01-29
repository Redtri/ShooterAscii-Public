#pragma once
#include "MovingEntity.h"

/*
===============================================================================
	Shot by a living entity, give damage by touching a living entity.
===============================================================================
*/
class Projectile :
	public MovingEntity
{
public:
	Projectile(short x, short y, char color, Direction direction = UP);

	/**
	 * Return true (other entity classes isProjectile returns false)
	 */
	bool isProjectile() const final;
};

