#include "LivingEntity.h"

LivingEntity::LivingEntity(SHORT x, SHORT y, short hp, char skin, char color) :
	MovingEntity(x, y, skin, color), m_hp(hp)
{
}

short LivingEntity::getHP() const
{
	return m_hp;
}

bool LivingEntity::isDead() {
    return !m_hp;
}

bool LivingEntity::isLiving() const
{
	return true;
}

void LivingEntity::update()
{
}

void LivingEntity::setHP(short hp)
{
	m_hp = hp;
}

void LivingEntity::changeHp(short points)
{
	m_hp += points;
	if (m_hp < 0) {
		m_hp = 0;
	}
}
