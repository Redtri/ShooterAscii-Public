#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <cstdlib>
#include <ctime>
#include <list>
#include <set>

#include "Utils.h"
#include "Map.h"
#include "Player.h"
#include "Monster.h"
#include "Projectile.h"

#define AIM_UP '|'
#define AIM_RIGHT '-'
#define AIM_DOWN '|'
#define AIM_LEFT '-'

#define MAX_COLLECTABLE 3

/*
===============================================================================
	Manages entities (generation, collisions, movement, ...)
===============================================================================
*/
class EntityManager
{
public:
	EntityManager(float elapsedTime);
	~EntityManager();

	/**
	 * Return a list of all entities.
	 */
	std::list<Entity*> const &getEntities() const;
	/**
	 * Return the number of monster killed during this frame
	 */
	short getKills();

	/**
	 * Cleanly delete the the blocked tiles, create new ones with the map, and regenerate all non player entities.
	 * -> map: (Map const &) The new map.
	 * -> score: (int) The current score to decide the number of monsters.
	 */
	void setNewMap(Map const & map, int const score);
	/**
	 * Move the player in the specified direction. If there is a collectable at the new position, collect it.
	 * -> dir: (Map const &) The new map.
	 */
	void movePlayer(Direction dir);
	/**
	 * Update all entities and  move the projectiles.
	 * -> elapsedTime : (float) total elapsed time since the beginning of the application.
	 * -> deltaTime : (float) elapsed time since the last update call.
	 */
	bool update(float elapsedTime, float deltaTime);
	/**
	 * Shoot a projectile, from the player or a monster.
	 * -> pEntity: (LivingEntity *) The entity shooting. If no entity is given, shoot from the player.
	 * -> pEntitiesToRemove: (std::set<Entity*>*) A pointer to a set where to add entities to remove.
	 */
	void shoot(LivingEntity *pEntity = nullptr, std::set<Entity*>* pEntitiesToRemove = nullptr);
	/**
	 * Reset the player.
	 */
	void reset();

private:
	/**
	 * Return the absolute value of x.
	 * -> x: (SHORT) A number we want the absolute value.
	 */
    SHORT absShort(SHORT x) const;
	/**
	 * Return a random number.
	 * -> maxValue: (int) The maximum value for the number.
	 */
	int random(int maxValue);

	/**
	 * Return an iterator on m_entities pointing to the entity at the position pCoord.
	 * -> pCoord: (COORD const&) The coordinate where is the entity you look for.
	 */
	std::list<Entity*>::const_iterator findEntity(COORD const& pCoord) const;
	/**
	* Creates a COORD set from every unwalkable entity in the game (Performance gain since we do not include the walls).
	*/
	std::set<COORD> convertEntities() const;

	/**
	 * Cleanly delete all non player entities, the generate monters and collectables.
	 * -> score: (int) The current score to decide the number of monsters.
	 */
	void generateEntities(int const score);
	/**
	 * Move a moving entity, a tile in the given direction.
	 * -> pLEntity: (MovingEntity *) The moving entity to move.
	 * -> dir: (Direction) The direction in which to move.
	 */
	bool move(MovingEntity *pLEntity, Direction dir);
	/**
	 * Collide a projectile with a wall or an entity.
	 * -> iCollidedEntity: (std::list<Entity*>::const_iterator) The collided entity, the return value of findEntity.
	 * -> pEntitiesToRemove: (std::set<Entity*> *) A pointer to a set where to add entities to remove.
	 */
	void collide(std::list<Entity*>::const_iterator iCollidedEntity, std::set<Entity*> *pEntitiesToRemove);

	std::list<Entity*> m_entities;
	std::set<COORD*, ComparePointers<COORD>> m_blockedTiles;
	short m_kills;
};

#endif // !ENTITY_MANAGER_H