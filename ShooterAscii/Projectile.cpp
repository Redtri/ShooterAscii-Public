#include "Projectile.h"

Projectile::Projectile(short x, short y, char color, Direction direction) :
	MovingEntity(x, y, '*', color, direction)
{
}

bool Projectile::isProjectile() const
{
	return true;
}
