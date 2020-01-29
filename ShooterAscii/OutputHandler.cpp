#include "OutputHandler.h"

OutputHandler::OutputHandler() :
	m_hOutput((HANDLE)GetStdHandle(STD_OUTPUT_HANDLE)), m_pEntityManager(nullptr)
{
	m_winOutput = GetConsoleWindow();
	
	//Setting new window style, disabling some utilities (eg : Resizing)
	LONG_PTR style_ptr = SetWindowLongPtr(m_winOutput, GWL_STYLE, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);

	SetWindowPos(m_winOutput, 0, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_DRAWFRAME);
	ShowWindow(m_winOutput, SW_SHOW);

	//Setting window size
	SMALL_RECT windowRect = { 0, 0, WIN_WIDTH, WIN_HEIGHT};
	SetConsoleWindowInfo(m_hOutput, true, &windowRect);

	//Setting console output Code Page (allowing some Unicode characters)
	SetConsoleOutputCP(12000);

	//Initializing m_buffer values
    for ( int i = 0; i < WIN_HEIGHT; ++i )
    {
        for ( int j = 0; j < WIN_WIDTH; ++j )
        {
			m_buffer[i][j] = CHAR_INFO({' ', 0});
        }
    }
}

//MUTATORS

void OutputHandler::setEntityManager(EntityManager *pEntityManager)
{
	m_pEntityManager = pEntityManager;
}

void OutputHandler::setNewMap(Map const &rMap)
{
	//Buffer cleaning
	for (int i = 0; i < WIN_HEIGHT; ++i)
	{
		for (int j = 0; j < WIN_WIDTH; ++j)
		{
			m_buffer[i][j] = CHAR_INFO({ (WCHAR)' ', 0x0001 });
		}
	}
	//Walls rendering
	for (auto& wall : rMap.getWalls()) {
		m_buffer[wall->Y][wall->X].Attributes = WALL_COLOR;
	}
}

void OutputHandler::loadTxtScreen(char const *filePath, char const tag[10])
{
	//Buffer cleaning
	for (int i = 0; i < WIN_HEIGHT; ++i) {
		for (int j = 0; j < WIN_WIDTH; ++j) {
			m_buffer[i][j] = CHAR_INFO({ (WCHAR)' ', PLAYER_COLOR });
		}
	}

	//Used to read char by char data inside the stream
	char c;
	//Represent coordinates considering line breaks and nb chars in the stream
	short x = 0, y = 0;
	//Buffer to store getLine() function result 
	char str[10] = { 0 };
	//Boolean testing whether the text data is currently being loaded into buffer or not
	bool storing = false;
	std::ifstream fs;

	fs.open(filePath);

	while (!fs.eof()) {
		int i = 0;

		//Testing next stream char
		c = fs.get();
		//If a balisa is being opened
		if (c == '#') {
			//Unget the last char 
			fs.unget();
			//Get the full line into str buffer
			fs.getline(str, 10);
			//If the string is the same as the tag searched
			if (strcmp(str, tag) == 0) {
				//If the buffer is not already storing stream data, storing becomes true for next operations, else loop is broken
				if (!storing)
					storing = true;
				else
					break;
			}
		}
		if (storing) {
			switch (c) {
			case '\n':
				//Incrementing Y axis for each line break
				++y;
				x = 0;
				break;
			default:
				//Incrementing X axis for each character and storing it into buffer
				m_buffer[y][x].Char.UnicodeChar = c;
				++x;
			}
		}
	}
	fs.close();
}

//RENDERER

void OutputHandler::draw(int score) const
{
	COORD dwBufferSize = { TRUE_WIDTH, TRUE_HEIGHT };
	COORD dwBufferCoord = { 0, 0};
	SMALL_RECT rcRegion = { 0, 0, WIN_WIDTH - 1, WIN_HEIGHT - 1};
	CHAR_INFO buffer[TRUE_HEIGHT][TRUE_WIDTH];

	//Loading data contained within m_buffer (with OFFSET)
	for (int i = 0; i < TRUE_HEIGHT; ++i) {
		for (int j = 0; j < TRUE_WIDTH; ++j) {
			if (i < H_OFFSET || j < W_OFFSET) {
				buffer[i][j] = CHAR_INFO({ ' ', 0 });
			} else {
				buffer[i][j] = m_buffer[i - H_OFFSET][j - W_OFFSET];
			}
		}
	}

	//Aiming entities rendering
	std::set<Entity*> priors;
	for ( auto& entity : m_pEntityManager->getEntities() ) {
		if ( entity->isLiving() )
		{
			Direction dir = ((LivingEntity*)entity)->getDirection();
			switch (dir)
			{
			case UP:
				buffer[entity->getPosition().Y-1 + H_OFFSET][entity->getPosition().X + W_OFFSET].Char.UnicodeChar = AIM_UP;
				buffer[entity->getPosition().Y-1 + H_OFFSET][entity->getPosition().X + W_OFFSET].Attributes += entity->getColor();
				break;
			case DOWN:
				buffer[entity->getPosition().Y+1 + H_OFFSET][entity->getPosition().X + W_OFFSET].Char.UnicodeChar = AIM_DOWN;
				buffer[entity->getPosition().Y+1 + H_OFFSET][entity->getPosition().X + W_OFFSET].Attributes += entity->getColor();
				break;
			case LEFT:
				buffer[entity->getPosition().Y + H_OFFSET][entity->getPosition().X-1 + W_OFFSET].Char.UnicodeChar = AIM_LEFT;
				buffer[entity->getPosition().Y + H_OFFSET][entity->getPosition().X-1 + W_OFFSET].Attributes += entity->getColor();
				break;
			case RIGHT:
				buffer[entity->getPosition().Y + H_OFFSET][entity->getPosition().X+1 + W_OFFSET].Char.UnicodeChar = AIM_RIGHT;
				buffer[entity->getPosition().Y + H_OFFSET][entity->getPosition().X+1 + W_OFFSET].Attributes += entity->getColor();
				break;
            default:
                break;
			}
		}
		priors.insert(entity);
	}

	//Entities (priority) rendering above aiming drawing
	for (auto& i : priors) {
		buffer[i->getPosition().Y + H_OFFSET][i->getPosition().X + W_OFFSET] = CHAR_INFO({ (WCHAR)i->getSkin(), (WORD)i->getColor() });
	}

	//Score displaying
	char str[SCORE_BUFFER_SIZE] = { 0 };

	sprintf_s(str, SCORE_BUFFER_SIZE,"%d", score);

	for (short i = 0; str[i] != '\0'; ++i )
	{
		buffer[0][(SHORT)((TRUE_WIDTH)*0.3) + i] = CHAR_INFO({ (WCHAR)str[i], 0x0008 });
	}

	//Health displaying
	short hp = ((LivingEntity*)m_pEntityManager->getEntities().front())->getHP();
	short i = 0;

	//Displaying HP bar's starting border
	buffer[0][(SHORT)((TRUE_WIDTH)*0.7)-1] = CHAR_INFO({ '|', PLAYER_COLOR });
	//Displaying HPs
	for ( i = 0; i < MAX_HP_PLAYER; ++i )
	{
		if(i > hp)
			buffer[0][(SHORT)((TRUE_WIDTH)*0.7) + i] = CHAR_INFO({ ' ', PLAYER_COLOR });
		else
			buffer[0][(SHORT)((TRUE_WIDTH)*0.7) + i] = CHAR_INFO( { 0x058D, PLAYER_COLOR } );
	}
	//Displaying HP bar's ending border
	buffer[0][(SHORT)((TRUE_WIDTH)*0.7)+i] = CHAR_INFO({ '|', PLAYER_COLOR });
	
	//Rendering buffer
	WriteConsoleOutput(m_hOutput, (CHAR_INFO *)buffer, dwBufferSize, dwBufferCoord, &rcRegion);
	ShowScrollBar(m_winOutput, SB_BOTH, FALSE);
}

void OutputHandler::drawMenu() const
{
	COORD dwBufferSize = { TRUE_WIDTH, TRUE_HEIGHT };
	COORD dwBufferCoord = { 0, 0 };
	SMALL_RECT rcRegion = { 0, 0, WIN_WIDTH - 1, WIN_HEIGHT - 1 };
	CHAR_INFO buffer[TRUE_HEIGHT][TRUE_WIDTH];

	//Récupération des infos du buffer
	for (int i = 0; i < TRUE_HEIGHT; ++i) {
		for (int j = 0; j < TRUE_WIDTH; ++j) {
			if (i < H_OFFSET || j < W_OFFSET) {
				buffer[i][j] = CHAR_INFO({ ' ', 0 });
			}
			else {
				buffer[i][j] = m_buffer[i - H_OFFSET][j - W_OFFSET];
			}
		}
	}

	//Rendering buffer
	WriteConsoleOutput(m_hOutput, (CHAR_INFO *)buffer, dwBufferSize, dwBufferCoord, &rcRegion);
	ShowScrollBar(m_winOutput, SB_BOTH, FALSE);
}

void OutputHandler::drawDeathScreen(int score) const
{
	COORD dwBufferSize = { TRUE_WIDTH, TRUE_HEIGHT };
	COORD dwBufferCoord = { 0, 0 };
	SMALL_RECT rcRegion = { 0, 0, WIN_WIDTH - 1, WIN_HEIGHT - 1 };
	CHAR_INFO buffer[TRUE_HEIGHT][TRUE_WIDTH];

	//Récupération des infos du buffer
	for (int i = 0; i < TRUE_HEIGHT; ++i) {
		for (int j = 0; j < TRUE_WIDTH; ++j) {
			if (i < H_OFFSET || j < W_OFFSET) {
				buffer[i][j] = CHAR_INFO({ ' ', PLAYER_COLOR });
			}
			else {
				buffer[i][j] = m_buffer[i - H_OFFSET][j - W_OFFSET];
			}
		}
	}
	
	char strScore[SCORE_BUFFER_SIZE] = { 0 };

	sprintf_s(strScore, SCORE_BUFFER_SIZE, "%d", score);

	for (short i = 0; strScore[i] != '\0'; ++i)
	{
		if (strScore[i] != '\0') {
			buffer[(SHORT)((TRUE_HEIGHT)*0.5)][(SHORT)((TRUE_WIDTH)*0.5) + i].Char.UnicodeChar = strScore[i];
		}
	}

	//Rendering buffer
	WriteConsoleOutput(m_hOutput, (CHAR_INFO *)buffer, dwBufferSize, dwBufferCoord, &rcRegion);
	ShowScrollBar(m_winOutput, SB_BOTH, FALSE);
}