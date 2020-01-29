#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "EntityManager.h"
#include "InputHandler.h"
#include "OutputHandler.h"
#include "FlowHandler.h"
#include "NY_TIMER.h"

/*
===============================================================================
	Singleton. Updates all entities, calls the input et output handler, keeps track of the score.
===============================================================================
*/
class GameManager
{
public:
	/**
	 * Return the game manager, call the constructor if none where created before.
	 */
	static GameManager& getInstance()
	{
		static GameManager instance;

		return instance;
	}
	~GameManager();

	/**
	 * Return the current score.
	 */
	int getScore() const;
	float getTime() const;

	/**
	 * Increase the score by an amount of value.
	 * -> value: (int const) The amount to increase the score.
	 */
	void increaseScore(int const value);

	/**
	 * Cleanly delete the previous map, create a new one, and give it to the output handler and the entity manager.
	 */
	void newMap();
	/**
	 * Call the handlers and managers, depending on the game state and phase.
	 * -> deltaTime : (float) elapsed time since the last update function call.
	 */
	bool update(float deltaTime);
private:
	GameManager();

	int m_score;
	FlowHandler m_flowHandler;
	OutputHandler m_outputHandler;
	InputHandler m_inputHandler;
	EntityManager m_entityManager;
	Map *m_pMap;
	float m_time;
};

#endif

