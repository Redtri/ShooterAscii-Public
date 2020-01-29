#ifndef OUPUT_HANDLER_H
#define OUPUT_HANDLER_H

#include <windows.h>
#include <cstdio>
#include <fstream>
#include <stdio.h>
#include <string>

#include "Map.h"
#include "EntityManager.h"

//Display offset inside the game window
#define W_OFFSET 2
#define H_OFFSET 1

#define TRUE_HEIGHT WIN_HEIGHT+H_OFFSET
#define TRUE_WIDTH WIN_WIDTH+W_OFFSET

#define WALL_COLOR 0x0080
#define SCORE_BUFFER_SIZE 10

/*
===============================================================================
	Manages all outputs to the console.
===============================================================================
*/
class OutputHandler
{
public:
	/**
	 * Retrieve the console handle, and configure it.
	 */
	OutputHandler();

	//MUTATORS
	/**
	 * Set a member variable pointer.
	 * -> pEntityManager: (EntityManager*) A pointer to the EntityManager.
	 */
	void setEntityManager(EntityManager *pEntityManager);
	/**
	 * Set the map that will be drawn each frame.
	 * -> rMap: (Map const &) the new map that will be drawn each frame.
	 */
	void setNewMap(Map const &rMap);
	/**
	 * Loads data to display on screen from text file.
	 * -> filePath: (char const *filePath) the file path to be loaded.
	 * -> tag: (char const tag[10]) the string ID that will be searched inside the text file.
	 */
	void loadTxtScreen(char const *filePath, char const tag[10]);

	//RENDERER
	/**
	 * Draw the map, all entities, and the HUD (score).
	 * -> score: (int) Current score to draw.
	 */
	void draw(int score) const;
	/**
	 * Draws the main menu.
	 */
	void drawMenu() const;
	/**
	 * Draws the Game Over screen.
	 * -> score: (int) Player score to be displayed.
	 */
	void drawDeathScreen(int score) const;

private:
	//Standard handle
	HANDLE m_hOutput;
	//Console Window handle
	HWND m_winOutput;
	EntityManager *m_pEntityManager;
	CHAR_INFO m_buffer[WIN_HEIGHT+H_OFFSET][WIN_WIDTH+W_OFFSET];
};

#endif // OUPUT_HANDLER_H