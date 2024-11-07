#ifndef H_GAME
#define H_GAME

#include "board.h"

class Game
{
	Board m_b;
	int m_size = 4;
	

	void m_upKey(void);
	void m_downKey(void);
	void m_leftKey(void);
	void m_rightKey(void);

	void m_handleInput(void);

	void m_save(void);
	void m_load(void);

	void m_deleteSave(void);

	void m_colorPicker(int n) const;

	void m_reset(void);

	bool m_passed = false;
	void m_displayBoard(void) const;
	void m_displayHeader(void) const;
	void m_displayFooter(void) const;

	void m_display(void) const;

public:
	Game();
	Game(int size);
	~Game();

	
	void run(void);
};

#endif
