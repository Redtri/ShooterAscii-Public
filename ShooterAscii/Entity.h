#ifndef ENTITY_H
#define ENTITY_H

#include <windows.h>

enum Direction { NONE, UP, DOWN, LEFT, RIGHT };

class Entity
{
public:
	/*
	* Constructor with parameters
	* -> x: (SHORT) y: (SHORT) coords value
	* -> skin: (char) character to be rendered as the representation of the entity
	* -> color: (char) char code of the entity's color
	*/
	Entity(SHORT x, SHORT y, char skin, char color);

	/**
	 * Return the current position.
	 */
	COORD const & getPosition() const;
	/**
	 * Set a new position.
	 * -> pos: (COORD) the new position.
	 */
    void setPosition(COORD pos);
	/**
	 * Return the skin.
	 */
	char const & getSkin() const;
	/**
	 * Return the color.
	 */
	char const & getColor() const;
	/**
	 * Set a new color.
	 * -> color: (char) the new color.
	 */
	void setColor(char color);

	/**
	 * Return true if the player can walk on the entity (return false, redefine in children)
	 */
	virtual bool isWalkable() const;
	/**
	 * Return true if the entity is a LivingEntity (return false, redefine in children)
	 */
	virtual bool isLiving() const;
	/**
	 * Return true if the entity is a Projectile (return false, redefine in children)
	 */
	virtual bool isProjectile() const;
	/**
	 * Return true if the entity is a Collectable (return false, redefine in children)
	 */
	virtual bool isCollectable() const;
	virtual bool isMonster() const;

	/**
	 * Update the living entity (does nothing, redefine in children)
	 */
	virtual void update();
protected:
	COORD m_position;
private:
	char m_skin;
	char m_color;
};

#endif // !ENTITY_H

