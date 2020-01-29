#include "Monster.h"

Monster::Monster(SHORT x, SHORT y, COORD playerPos) :
	LivingEntity(x, y, MAX_HP_MONSTER, MONSTER_SKIN, MONSTER_COLOR),
	m_lastRefresh(0)
{
	m_pathfinding.setStart(new AstarNode(nullptr, COORD({ x,y })));
	m_pathfinding.setDestination(new AstarNode(nullptr, playerPos));
}

Monster::~Monster()
{
}

void Monster::setLastPathRefresh(float const refreshTime)
{
	m_lastRefresh = refreshTime;
}

void Monster::setLastMoveRefresh(float const refreshTime)
{
	m_moveRefresh = refreshTime;
}

bool const Monster::isPathCalculated() const
{
	return m_pathfinding.isCalculated();
}

float const Monster::getLastPathRefresh() const
{
	return m_lastRefresh;
}

float const Monster::getLastMoveRefresh() const
{
	return m_moveRefresh;
}

void Monster::changeHp(short points) {
    LivingEntity::changeHp(points);
    if (points < 0) {
        setColor(0x000E);
    }
	if (getHP() > MAX_HP_MONSTER) {
		setHP(MAX_HP_MONSTER);
	}
}

Direction Monster::nextMovement()
{
	COORD tmp = m_pathfinding.nextNode();
	Direction dir = UP;
	if (tmp == 0) {
		dir = LEFT;
	}else {
		if (m_position.X < tmp.X) {
			dir = RIGHT;
		}
		else if (m_position.X > tmp.X) {
			dir = LEFT;
		}

		if (m_position.Y < tmp.Y) {
			dir = DOWN;
		}
		else if (m_position.Y > tmp.Y) {
			dir = UP;
		}
	}
	setDirection(dir);
	
	return dir;
}

void Monster::calculatePath(std::set<COORD> const & blockedTiles)
{
	m_pathfinding.makePath(blockedTiles);
}

void Monster::remakePath(COORD playerPos, std::set<COORD> const &blockedTiles)
{
	m_pathfinding.reset();
	m_pathfinding.setStart(AstarNode(nullptr, COORD({ m_position.X, m_position.Y })));
	m_pathfinding.setDestination(AstarNode(nullptr, playerPos));
	m_pathfinding.makePath(blockedTiles);
}

void Monster::update()
{
    if ( getColor() != MONSTER_COLOR ) {
        setColor(MONSTER_COLOR);
    }
}

COORD Monster::updateMovement()
{
	return m_pathfinding.nextNode();
}

bool Monster::isMonster() const
{
	return true;
}
