#include "MovingEntity.h"

MovingEntity::MovingEntity(SHORT x, SHORT y, char skin, char color, Direction direction) :
	Entity(x, y, skin, color), m_direction(direction)
{
}

void MovingEntity::setDirection(Direction direction)
{
	m_direction = direction;
}

Direction MovingEntity::getDirection() const
{
	return m_direction;
}

COORD MovingEntity::nextPosition(Direction direction) const
{
	COORD position = getPosition();

	if (direction == NONE) {
		direction = m_direction;
	}

	switch (direction) {
	case UP:
		--position.Y;
		break;
	case RIGHT:
		++position.X;
		break;
	case DOWN:
		++position.Y;
		break;
	case LEFT:
		--position.X;
		break;
	}

	return position;
}

void MovingEntity::update()
{
}
