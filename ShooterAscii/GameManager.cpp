#include "GameManager.h"

GameManager::GameManager() :
	m_score(0), m_time(0), m_pMap(new Map()), m_entityManager(m_time)
{
	m_outputHandler.setEntityManager(&m_entityManager);
	m_inputHandler.setFlowHandler(&m_flowHandler);
	m_inputHandler.setEntityManager(&m_entityManager);
	m_outputHandler.setNewMap(*m_pMap);
	m_entityManager.setNewMap(*m_pMap, m_score);
}

GameManager::~GameManager()
{
	delete m_pMap;
}

int GameManager::getScore() const
{
	return m_score;
}

float GameManager::getTime() const
{
	return m_time;
}

void GameManager::increaseScore(int const value)
{
	m_score += value;
}

void GameManager::newMap()
{
	delete m_pMap;
	m_pMap = new Map();
	m_outputHandler.setNewMap(*m_pMap);
	m_entityManager.setNewMap(*m_pMap, m_score);
}

bool GameManager::update(float deltaTime)
{
	
	// If the player go out of the map, generates a new map and changes the player position accordingly.
	COORD const & playerPos(m_entityManager.getEntities().front()->getPosition());

	switch ( m_flowHandler.getGameState() ) {
	case RUN:
		m_time += deltaTime;
		switch (m_flowHandler.getGamePhase()) {
		case MENU:
			m_inputHandler.handleInput();
			m_outputHandler.drawMenu();
			break;
		case PLAY:
			if (m_entityManager.update(m_time, deltaTime)) {
				m_flowHandler.setGamePhase(DEATH);
				newMap();
				m_entityManager.reset();
			}
			m_inputHandler.handleInput();

			if ((playerPos.X == 0) || (playerPos.X == WIN_WIDTH - 4) || (playerPos.Y == 0) || (playerPos.Y == WIN_HEIGHT - 2)) {
				newMap();
			}
			m_score += m_entityManager.getKills();
			m_outputHandler.draw(m_score);
			break;
		case DEATH:
			m_inputHandler.handleInput();
			m_outputHandler.drawDeathScreen(m_score);
			break;
		}
		return false;
	case LOAD:
		switch (m_flowHandler.getGamePhase()) {
		case MENU:
			m_outputHandler.loadTxtScreen("Files/texts.txt", "#menu#");
			m_score = 0;
			break;
		case PLAY:
			m_outputHandler.setNewMap(*m_pMap);
			m_entityManager.setNewMap(*m_pMap, m_score);
			break;
		case DEATH:
			m_outputHandler.loadTxtScreen("Files/texts.txt", "#death#");
			break;
		}
		m_flowHandler.setGameState(RUN);
		return false;
	case PAUSE:
		m_inputHandler.handleInput();
		return false;
	case CLOSE:
		return true;
	default:
		return false;
	}
}
