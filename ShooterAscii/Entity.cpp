#include "Entity.h"

Entity::Entity(SHORT x, SHORT y, char skin, char color) :
	m_skin(skin), m_color(color)
{
	m_position = { x, y };
}


COORD const & Entity::getPosition() const
{
	return m_position;
}

void Entity::setPosition(COORD const pos)
{
    m_position.X = pos.X;
    m_position.Y = pos.Y;
}

char const & Entity::getSkin() const
{
	return m_skin;
}

char const & Entity::getColor() const
{
	return m_color;
}

void Entity::setColor(char color) {
    m_color = color;
}

bool Entity::isWalkable() const
{
	return false;
}

bool Entity::isLiving() const
{
	return false;
}

bool Entity::isProjectile() const
{
	return false;
}

bool Entity::isCollectable() const
{
    return false;
}

bool Entity::isMonster() const
{
	return false;
}

void Entity::update()
{
}
