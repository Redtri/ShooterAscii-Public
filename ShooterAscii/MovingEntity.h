#pragma once
#include "Entity.h"

/*
===============================================================================
	An entity that is able to move (got a direction)
===============================================================================
*/
class MovingEntity :
	public Entity
{
public:
	/*
	* Constructor with parameters
	* -> x: (SHORT) y: (SHORT) coords value
	* -> hp: (short) entity health points
	* -> skin: (char) character to be rendered as the representation of the entity
	* -> color: (char) char code of the entity's color
	* -> direction: (Direction) starting direction
	*/
	MovingEntity(SHORT x, SHORT y, char skin, char color, Direction direction = UP);

	/**
	 * Set the direction of the moving entity.
	 * -> direction: (Direction) the new direction
	 */
	void setDirection(Direction direction);
	/**
	 * Return the direction of the moving entity
	 */
	Direction getDirection() const;
	/**
	 * Return the position of the moving entity if it moves 1 tile in the given direction.
	 * If the direction given is NONE, use the direction of the moving entity.
	 * -> direction: (Direction) the direction to move to
	 */
	COORD nextPosition(Direction direction = NONE) const;

	/**
	 * Update the living entity (does nothing, redefine in children)
	 */
	virtual void update();

private:
	Direction m_direction;
};

