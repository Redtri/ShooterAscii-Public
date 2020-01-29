#include "Map.h"
#include <iostream>

Map::Map()
{
	COORD coordBuffer = {0,0};
	char c;
	std::ifstream fs(FILE_PATH, std::ifstream::in);

	while (fs.good()) {
		c = fs.get();
		switch (c) {
		case '*':
			//Ajout d'un mur
			m_walls.push_back(new COORD(coordBuffer));
			//Incrémentation de l'axe X pour chaque caractère
			coordBuffer.X++;
			break;
		case '\n':
			//Incrémentation de l'axe Y s'il y a un retour à la ligne
			coordBuffer.Y ++;
			coordBuffer.X = 0;
			break;
		default:
			//Incrémentation de l'axe X pour chaque caractère
			coordBuffer.X++;
		}
	}
	fs.close();
}

Map::~Map()
{
	for (auto pWall : m_walls) {
		delete pWall;
	}
	m_walls.clear();
}

std::list<COORD*> const &Map::getWalls() const
{
	return m_walls;
}
