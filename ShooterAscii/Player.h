#ifndef PLAYER_H
#define PLAYER_H

#include "LivingEntity.h"

#define PLAYER_COLOR 0x0002
#define PLAYER_SKIN 'O'
#define POS_INIT_PLAYER 12,5
#define MAX_HP_PLAYER 10

/*
===============================================================================
	Manages the player
===============================================================================
*/
class Player :
	public LivingEntity
{
public:
	/**
	 * Creates an instance of a Player.
	 * -> skin: (char) character representing the player.
	 * -> color: (char) hex number for the player character color
	 */
	Player();

	/**
	 * Reset the player's HP and position
	 */
	void reset();
	/**
	 * Modifiy the HP with points, with a minimum HP of 0, and a maximum of MAX_HP_PLAYER.
	 * If points is negative, set the color of the player to yellow (need to set back with update in the next frame).
	 * -> points: (short) the amount of health points to add or remove.
	 */
    void changeHp(short points) final;
	/**
	 * Update the living entity (set the color back to the default)
	 */
    void update() final;
};

#endif // !PLAYER_H

