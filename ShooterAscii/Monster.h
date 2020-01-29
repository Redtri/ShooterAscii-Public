#ifndef MONSTER_H
#define MONSTER_H

#include <list>

#include "LivingEntity.h"
#include "Astar.h"

#define MONSTER_PATH_REFRESH 2
#define MONSTER_MOVE_REFRESH 0.5
#define MAX_HP_MONSTER 5
#define MONSTER_SKIN 'X'
#define MONSTER_COLOR 0x000C

/*
===============================================================================
	Basic monster class that will hunt the player
===============================================================================
*/
class Monster :
	public LivingEntity
{
public:
	/*
	* Constructor with parameters.
	* -> x : (SHORT) x axis position.
	* -> y : (SHORT) y axis position.
	* -> player : (COORD) player position for the pathfinding.
	*/
	Monster(SHORT x, SHORT y, COORD player);
	~Monster();
	/*
	* Mutator for the last A* refresh time.
	* -> refreshTime : (float) new refresh time.
	*/
	void setLastPathRefresh(float const refreshTime);
	/*
	* Mutator for the last movement refresh time.
	* -> refreshTime : (float) new refresh time.
	*/
	void setLastMoveRefresh(float const refreshTime);
	/*
	* Returns whether the A* path has already been calculated or not.
	*/
	bool const isPathCalculated() const;
	/*
	* Returns the last path refresh time.
	*/
	float const getLastPathRefresh() const;
	/*
	* Returns the last movement refresh time.
	*/
	float const getLastMoveRefresh() const;

	/*
	* Returns and sets the correct direction of the entity considering the next node of the pathfiding algorithm.
	*/
	Direction nextMovement();
	/*
	* Calls the Astar class method of pathfinding calculation.
	* -> blockedTiles : (std::set<COORD>) unwalkable coordinates to be considered when calculating.
	*/
	void calculatePath(std::set<COORD> const &blockedTiles);
	/*
	* Restarts the path calculation process of the A* algorithm calculation process.
	* -> playerPos : (COORD) new player position (that is the destination).
	* -> blockedTiles : (std::set<COORD>) unwalkable coordinates to be considered when calculating.
	*/
	void remakePath(COORD playerPos, std::set<COORD> const &blockedTiles);
	/**
	 * Modifiy the HP with points, with a minimum HP of 0, and a maximum of MAX_HP_MONSTER.
	 * If points is negative, set the color of the monster to yellow (need to set back with update in the next frame).
	 * -> points: (short) the amount of health points to add or remove.
	 */
    void changeHp(short points);
	/**
	 * Update the living entity (set the color back to the default).
	 */
	void update();
	COORD updateMovement();

	bool isMonster() const;
private:
	AstarNode *m_pCurrentNode;
	Astar m_pathfinding;
	float m_lastRefresh;
	float m_moveRefresh;
};

#endif // !1