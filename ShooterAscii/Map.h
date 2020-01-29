#ifndef MAP_H
#define MAP_H

#include <list>
#include <windows.h>
#include <fstream>

#include "Utils.h"

#define FILE_PATH "Files/maps.txt"

#define WIN_WIDTH 100
#define WIN_HEIGHT 40


/*
===============================================================================
	Contains a list of walls
===============================================================================
*/
class Map
{
public:
	Map();
	~Map();

	//ACCESSEURS
	/**
	 * Return a list of walls making up the map.
	 */
	std::list<COORD*> const &getWalls() const;

private:
	std::list<COORD*> m_walls;
};

#endif // !MAP_H