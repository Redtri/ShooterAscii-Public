#include "EntityManager.h"

#include "GameManager.h"
#include "Map.h"
#include "Collectable.h"
#include "Apple.h"

EntityManager::EntityManager(float elapsedTime) :
	m_kills(0)
{
	m_entities.push_back(new Player());
	m_blockedTiles.insert( new COORD( m_entities.front()->getPosition() ) );
    std::srand((unsigned int)std::time(nullptr)); // use current time as seed for random generator
}

EntityManager::~EntityManager()
{
	for (auto pEntity : m_entities) {
		delete pEntity;
	}
	for (auto pBlockedTile : m_blockedTiles) {
		delete pBlockedTile;
	}
}

std::list<Entity*> const & EntityManager::getEntities() const
{
    return m_entities;
}

short EntityManager::getKills()
{
	short kills(m_kills);
	m_kills = 0;
	return kills;
}

void EntityManager::setNewMap(Map const & rMap, int const score)
{
    for (auto blockedTile : m_blockedTiles) {
        delete blockedTile;
    }
    m_blockedTiles.clear();

    for (auto wall : rMap.getWalls()) {
        m_blockedTiles.insert(new COORD(*wall));
    }

    generateEntities(score);
}

void EntityManager::movePlayer(Direction dir)
{
    LivingEntity* player((LivingEntity*)(m_entities.front()));
    move(player, dir);

	// If there is a collectable at the new position, collect it.
    for (auto iEntity = ++(m_entities.begin()); iEntity != m_entities.end(); ++iEntity) {
        if ( ((*iEntity)->getPosition() == player->getPosition()) ) {
            player->changeHp(((Collectable*)(*iEntity))->getRegen());
			Entity* pEntity(*iEntity);
            m_entities.erase(iEntity);
			delete pEntity;
            break;
        }
    }
}

bool EntityManager::update(float elapsedTime, float deltaTime)
{
    std::set<Entity*> entitiesToRemove;

	for (auto pEntity : m_entities) {

		pEntity->update();
		//Update monsters
		if (pEntity->isMonster()) {
			Monster *pMonster((Monster*)pEntity);

			if (pMonster->isPathCalculated()) {
				if ((pMonster->getLastPathRefresh() + MONSTER_PATH_REFRESH) < elapsedTime) {
					pMonster->setLastPathRefresh(elapsedTime);
					pMonster->remakePath(m_entities.front()->getPosition(), convertEntities());
				}
			}
			else {
				pMonster->calculatePath(convertEntities());
			}

			if ((pMonster->getLastMoveRefresh() + MONSTER_MOVE_REFRESH) < elapsedTime) {
				pMonster->setLastMoveRefresh(elapsedTime);
				pMonster->nextMovement();
				move(pMonster, pMonster->getDirection());
			}

			if (!random(80)) {
				shoot((LivingEntity*)pEntity, &entitiesToRemove);
			}
		}
	}
	for (auto pEntity : m_entities) {
		// Update projectiles
		if (pEntity->isProjectile()) {
			MovingEntity *pProj((MovingEntity*)pEntity);

			COORD posProj(pProj->getPosition());
			// If a monster moved on a projectile
			if (m_blockedTiles.find(&posProj) != m_blockedTiles.end()) {
				collide(findEntity(pProj->getPosition()), &entitiesToRemove);

				// Destroy the projectile
				entitiesToRemove.insert(pEntity);
			}
			// Tries to move the projectile
			// If the projectile collided with something
			else if (!move(pProj, pProj->getDirection())) {
					collide(findEntity((pProj)->nextPosition(pProj->getDirection())), &entitiesToRemove);

					// Destroy the projectile
					entitiesToRemove.insert(pEntity);
			}
			else {
				// If the projectile goes out of bounds
				COORD const& rPosProj(pProj->getPosition());
				if ((rPosProj.X == 0) || (rPosProj.X == WIN_WIDTH - 4) || (rPosProj.Y == 0) || (rPosProj.Y == WIN_HEIGHT - 2)) {
					// Destroy the projectile
					entitiesToRemove.insert(pEntity);
				}
			}
		}
	}

    // Destroy projectiles that need to ( Removes entities in pEntitiesToRemove from m_entities )
    m_entities.remove_if([entitiesToRemove](Entity* pEntity) -> bool { return entitiesToRemove.find(pEntity) != entitiesToRemove.end(); });
    for (auto pEntity : entitiesToRemove) {
        delete pEntity;
    }

    return ((LivingEntity *) m_entities.front())->isDead();
}

void EntityManager::shoot(LivingEntity *pEntity, std::set<Entity*>* pEntitiesToRemove)
{
    if (pEntity == nullptr) {
        pEntity = (LivingEntity*)(*m_entities.begin());
    }
    COORD projectileStartPosition = pEntity->nextPosition();

	if (m_blockedTiles.find(&projectileStartPosition) == m_blockedTiles.end()) {
		m_entities.push_back(new Projectile(projectileStartPosition.X, projectileStartPosition.Y, pEntity->getColor(), pEntity->getDirection()));
	} else {
		collide(findEntity(projectileStartPosition), pEntitiesToRemove);
	}
}

void EntityManager::reset()
{
	((Player *)m_entities.front())->reset();
}

SHORT EntityManager::absShort(SHORT const x) const {
    return (x < 0 ? -x : x);
}

std::list<Entity *>::const_iterator EntityManager::findEntity(COORD const &pCoord) const {
	auto ipEntity = m_entities.begin();
	while ( ipEntity != m_entities.end() && !((*ipEntity)->getPosition().X == pCoord.X && (*ipEntity)->getPosition().Y == pCoord.Y)) {
		++ipEntity;
    }
    return ipEntity;
}

std::set<COORD> EntityManager::convertEntities() const
{
	std::set<COORD> tmp;

	for (auto& i : m_entities) {
		if (!i->isWalkable()) {
			tmp.insert(i->getPosition());
		}
	}
	return tmp;
}

int EntityManager::random(int maxValue) {
    return std::rand() % maxValue;
}

void EntityManager::generateEntities(int const score)
{
	// Clears m_entities
	Entity* pPlayer = m_entities.front();

	for (auto itpEntity = ++(m_entities.begin()); itpEntity != m_entities.end(); ++itpEntity)
	{
		delete *itpEntity;
	}
	m_entities.clear();

	// Sets the player to the opposite position on the map.
	if (pPlayer->getPosition().X == 0 || pPlayer->getPosition().X == WIN_WIDTH - 4) {
		pPlayer->setPosition({absShort((WIN_WIDTH - 5) - pPlayer->getPosition().X ), pPlayer->getPosition().Y });
	} else if (pPlayer->getPosition().Y == 0 || pPlayer->getPosition().Y == WIN_HEIGHT - 2) {
		pPlayer->setPosition({pPlayer->getPosition().X, absShort((WIN_HEIGHT - 3) - (pPlayer->getPosition().Y) ) });
	}

	// Puts the player back in m_entities
	m_entities.push_back(pPlayer);

	m_blockedTiles.insert(new COORD(pPlayer->getPosition()));

	// Generate monsters
	int numMonsters = (score / 5) + 3;
	if (numMonsters > 100) { numMonsters = 100; }

	for (short i = 0; i < numMonsters; ++i) {
		COORD tmp = { (SHORT)(random(WIN_WIDTH - 10)+1), (SHORT)(random(WIN_HEIGHT - 10)+1) };
		while ( findEntity(tmp) != m_entities.end() ) {
			tmp.X = random(WIN_WIDTH - 3)+1;
			tmp.Y = random(WIN_HEIGHT - 3)+1;
		}
		Monster *m = new Monster(tmp.X, tmp.Y, (*m_entities.begin())->getPosition());
		m->setDirection((Direction)(random(4)+1));
		m_entities.push_back(m);
		m_blockedTiles.insert(new COORD(m->getPosition()));
	}
	// Generating monsters A* paths
	for (auto i : m_entities) {
		if (i->isMonster()) {
			Monster *m = (Monster*)i;
			m->remakePath(m_entities.front()->getPosition(), convertEntities());
		}
	}

	// Generate colectables
	
	short rand = (random(MAX_COLLECTABLE));
	for (short i = 0; i < rand ; ++i) {
		COORD tmp = { (SHORT)(random(WIN_WIDTH - 3) + 1), (SHORT)(random(WIN_HEIGHT - 3) + 1) };
		while (findEntity(tmp) != m_entities.end()) {
			tmp.X = random(WIN_WIDTH - 3) + 1;
			tmp.Y = random(WIN_HEIGHT - 3) + 1;
		}
		Apple *pApple = new Apple(tmp.X, tmp.Y);
		m_entities.push_back(pApple);
	}
}

bool EntityManager::move(MovingEntity *pLEntity, Direction dir)
{
    COORD nextPos( pLEntity->nextPosition(dir) );

    pLEntity->setDirection(dir);
    if (m_blockedTiles.find(&nextPos) == m_blockedTiles.end() ) {
		COORD curPos(pLEntity->getPosition());

		// If the entity blocks a tile (ie it is a Living entity), moves the blocked tile.
		auto iTile = m_blockedTiles.find(&curPos);
		if ( iTile != m_blockedTiles.end() ) {
			COORD* pTile(*iTile);
			m_blockedTiles.erase(iTile);
			delete pTile;
			m_blockedTiles.insert(new COORD(nextPos));
		}
        
		pLEntity->setPosition(nextPos);
        return true;
    }
    else {
        return false;
    }
}

void EntityManager::collide(std::list<Entity *>::const_iterator iCollidedEntity, std::set<Entity*> *pEntitiesToRemove)
{
	// If the projectile didn't collide with a wall, but with a LivingEntity, removes 1HP
	if (iCollidedEntity != m_entities.end()) {
		if ((*iCollidedEntity)->isLiving()) {
			LivingEntity* pLCollidedEntity((LivingEntity*)*iCollidedEntity);
    		pLCollidedEntity->changeHp(-1);

			// If the entity (not the player) dies, removes it.
			if (pLCollidedEntity != m_entities.front() && pLCollidedEntity->isDead()) {
   				COORD pos = pLCollidedEntity->getPosition();
				COORD* pTile = *m_blockedTiles.find(&pos);
				m_blockedTiles.erase(pTile);
				delete pTile;
				// If collide is called by the EntityManager.update
				if (pEntitiesToRemove != nullptr) {
					(*pEntitiesToRemove).insert(pLCollidedEntity);
				}
				// Else it means collide is called because a player projectile collided when it was just created 
				else {
					m_entities.remove(pLCollidedEntity);
					delete pLCollidedEntity;
				}
				++m_kills;
			}
		}
	}
}
