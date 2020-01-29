#include "FlowHandler.h"

FlowHandler::FlowHandler()
{
	m_gameState = LOAD;
	m_previousGS = RUN;

	m_gamePhase = MENU;
	m_previousGP = MENU;
}

GamePhase const FlowHandler::getGamePhase() const
{
	return m_gamePhase;
}

GameState const FlowHandler::getGameState() const
{
	return m_gameState;
}

void FlowHandler::setGamePhase(GamePhase gp)
{
	m_previousGP = m_gamePhase;
	m_gamePhase = gp;
	m_gameState = LOAD;
}

void FlowHandler::setGameState(GameState gs)
{
	m_previousGS = m_gameState;
	m_gameState = gs;
}

bool FlowHandler::changedGameState() const
{
	return m_previousGS == m_gameState;
}

bool FlowHandler::changedGamePhase() const
{
	return m_previousGP == m_gamePhase;
}
