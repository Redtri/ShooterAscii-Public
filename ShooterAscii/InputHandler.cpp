#include "InputHandler.h"

InputHandler::InputHandler() :
	m_handle(GetStdHandle(STD_INPUT_HANDLE)), m_pEntityManager(nullptr)
{
}

void InputHandler::setFlowHandler(FlowHandler * pFH)
{
	m_pFlowHandler = pFH;
}

void InputHandler::setEntityManager(EntityManager * pEM)
{
	m_pEntityManager = pEM;
}

void InputHandler::handleInput()
{
	ValidatedInputs validatedInputs;

	const short bufSize = 50;
	INPUT_RECORD buffer[bufSize]; // buffer to read input
	DWORD numEvent, numInRead, i;

	GetNumberOfConsoleInputEvents(m_handle, &numEvent);
	if (numEvent) {
		ReadConsoleInput(m_handle, buffer, bufSize, &numInRead);
		for (i = 0; i < numInRead; i++) {
			switch (buffer[i].EventType) {
			case KEY_EVENT:
				saveKeyEvent(buffer[i].Event.KeyEvent, validatedInputs);
				break;

			default:
				break;
			}
		}
		computeKeyEvent(validatedInputs);
	}
}

void InputHandler::saveKeyEvent(KEY_EVENT_RECORD const ker, ValidatedInputs& validatedInputs) const
{
	if (ker.bKeyDown) {
		switch (ker.wVirtualKeyCode)
		{
		case VK_UP:
			validatedInputs.up = true;
			break;
		case VK_DOWN:
			validatedInputs.down = true;
			break;
		case VK_LEFT:
			validatedInputs.left = true;
			break;
		case VK_RIGHT:
			validatedInputs.right = true;
			break;
		case VK_SPACE:
			validatedInputs.shoot = true;
			break;
        case VK_ESCAPE:
            validatedInputs.end = true;
            break;
		default:
			break;
		}
	}
}

void InputHandler::computeKeyEvent(ValidatedInputs& validatedInputs)
{
	GamePhase tmpPhase = m_pFlowHandler->getGamePhase();

	switch ( m_pFlowHandler->getGameState() ) {
	case RUN:
		
		//Testing ESC key that will leave the game during next frame (GameManager update)

		if(validatedInputs.end){
			m_pFlowHandler->setGameState(CLOSE);
			break;
		}else{ 
			if (tmpPhase == PLAY) {
				//Handling control keys for PLAY phase
				if (validatedInputs.up) {
					m_pEntityManager->movePlayer(UP);
				}
				if (validatedInputs.down) {
					m_pEntityManager->movePlayer(DOWN);
				}
				if (validatedInputs.left) {
					m_pEntityManager->movePlayer(LEFT);
				}
				if (validatedInputs.right) {
					m_pEntityManager->movePlayer(RIGHT);
				}
				if (validatedInputs.shoot) {
					m_pEntityManager->shoot();
				}
			} else if( validatedInputs.shoot ) {
				//Handling shoot key outside of the PLAY phase (eg : MENU, DEATH) : MENU leading to PLAY and DEATH leading to MENU
				m_pFlowHandler->setGamePhase( (tmpPhase == MENU) ? PLAY : MENU );
			}
		}
		break;
	case PAUSE:
		//Resuming pause mode
		if ( validatedInputs.shoot ) {
			m_pFlowHandler->setGameState(RUN);
		}
		break;
	}
}
