#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <iostream>
#include <windows.h>

#include "FlowHandler.h"
#include "EntityManager.h"

/*
===============================================================================
	Manages inputs
===============================================================================
*/
class InputHandler
{
public:
	InputHandler();

	/**
	 * Set the floaw handler.
	 * -> pFH: (FlowHandler *) A pointer to the flow handler.
	 */
	void setFlowHandler(FlowHandler *pFH);
	/**
	 * Set the entity manager.
	 * -> pEM: (EntityManager*) A pointer to the entity manager.
	 */
	void setEntityManager(EntityManager *pEM);

	/**
	 * Get the inputs, call saveKeyEvent on each one, and then call computeKeyEvent.
	 */
	void handleInput();

private:
	/**
	 * Used to take into account the inputs only once per frame.
	 * -> up: (bool) key arrow up
	 * -> down: (bool)  key arrow down
	 * -> left: (bool) key arrow left
	 * -> right: (bool) key arrow right
	 * -> shoot: (bool) key space
	 * -> end: (bool) key escape
	 */
	struct ValidatedInputs {
		bool up = false;
		bool down = false;
		bool left = false;
		bool right = false;
		bool shoot = false;
		bool end = false;
	};

	/**
	 * Turn the validatedInputs entry corresponding to the ker to true (if it is a valid input).
	 * -> ker: (KEY_EVENT_RECORD const) the key enent to compute.
	 * -> validatedInputs: (ValidatedInputs&) the structure to save the key event.
	 */
	void saveKeyEvent(KEY_EVENT_RECORD const ker, ValidatedInputs& validatedInputs) const;
	/**
	 * Compute the validated key events.
	 * -> validatedInputs: (ValidatedInputs&) the structure whith the validated key events.
	 */
	void computeKeyEvent(ValidatedInputs& validatedInputs);

	HANDLE m_handle;
	FlowHandler *m_pFlowHandler;
	EntityManager *m_pEntityManager;
};

#endif // !INPUT_HANDLER_H