#ifndef LIVING_ENTITY_H
#define LIVING_ENTITY_H

#include "MovingEntity.h"

/*
===============================================================================
	A moving entity possessing health points
===============================================================================
*/
class LivingEntity :
	public MovingEntity
{
public:
	/*
	* Constructor with parameters
	* -> x: (SHORT) y: (SHORT) coords value
	* -> hp: (short) entity health points
	* -> skin: (char) character to be rendered as the representation of the entity
	* -> color: (char) char code of the entity's color
	*/
	LivingEntity(SHORT x, SHORT y, short hp, char skin, char color);

	/**
	 * Return the HP
	 */
	short getHP() const;
	/**
	 * Return if HP == 0
	 */
	bool isDead();

	/**
	 * Return true (for entity classes that don't extends from LivingEntity, isLiving returns false)
	 */
	bool isLiving() const;

	/**
	 * Set the HP, without verification.
	 * -> hp: (short) the new value for HP
	 */
	virtual void setHP(short hp);
	/**
	 * Modifiy the HP with points, with a minimum HP of 0
	 * -> points: (short) the amount of health points to add or remove.
	 */
	virtual void changeHp(short points);
	/**
	 * Update the living entity (does nothing, redefine in children)
	 */
	virtual void update();

private:
	/**
	 * HP, health points
	 */
	short m_hp;
};

#endif // !LIVING_ENTITY_H