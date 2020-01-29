#ifndef FLOW_MANAGER_H
#define FLOW_MANAGER_H

enum GameState { RUN, LOAD, PAUSE, CLOSE };
enum GamePhase { MENU, PLAY, DEATH };

/*
===============================================================================
	Keeps track of the game state (RUN, LOAD, PAUSE, CLOSE) and phase (MENU, PLAY, DEATH)
===============================================================================
*/
class FlowHandler
{
public:
	FlowHandler();

	GamePhase const getGamePhase() const;
	GameState const getGameState() const;
	void setGamePhase(GamePhase gp);
	void setGameState(GameState gs);
	bool changedGameState() const;
	bool changedGamePhase() const;
private :
	GameState m_gameState;
	GameState m_previousGS;

	GamePhase m_gamePhase;
	GamePhase m_previousGP;
};
#endif