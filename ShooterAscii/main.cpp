#include "GameManager.h"
#include "NY_TIMER.h"

#define TICK_TIME 32*0.001

int main()
{
	NYTimer timer;
	bool end = false;
	float deltaTime;

	timer.start();
	while ( !end )
	{
		if ((deltaTime = timer.getElapsedSeconds()) > TICK_TIME) {
			timer.start();
			end = GameManager::getInstance().update(deltaTime);
		}
	}

	return EXIT_SUCCESS;
}