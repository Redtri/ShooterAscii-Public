#include "Player.h"

#include "Map.h"

Player::Player() :
	LivingEntity(POS_INIT_PLAYER, MAX_HP_PLAYER, PLAYER_SKIN, PLAYER_COLOR) // WIN_WIDTH/2, WIN_HEIGHT/2
{
}

void Player::reset()
{
	setHP(MAX_HP_PLAYER);
	setPosition({ POS_INIT_PLAYER });
}

void Player::changeHp(short points) {
    LivingEntity::changeHp(points);
    if (points < 0) {
        setColor(0x000E);
    }
	if (getHP() > MAX_HP_PLAYER) {
		setHP(MAX_HP_PLAYER);
	}
}

void Player::update() {
    if ( getColor() != PLAYER_COLOR ) {
        setColor(PLAYER_COLOR);
    }
}