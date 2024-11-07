#include "game.h"
#include "termcolor.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdio>


#include <termios.h>
#include <unistd.h>

void enableRawMode() {
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void disableRawMode() {
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= ICANON | ECHO; // Enable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}


char getche(bool display = true) {
    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);  // Save old settings
    newt = oldt;
    
    if (!display) {
        newt.c_lflag &= ~(ICANON | ECHO);  // Disable buffering and echo
    } else {
        newt.c_lflag &= ~ICANON;            // Disable buffering only
    }
    
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);  // Apply new settings

    char ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);  // Restore old settings
    return ch;
}

#ifdef _WIN32
	#include <windows.h>
#else
	#include <cstdlib>
#endif

Game::Game() = default;

Game::Game(int size) : m_size(size)
{
	m_b.setBoard(m_size);
}

Game::~Game() = default;

void Game::m_colorPicker(int n) const
{
	switch(n)
	{
		case 0:
			std::cout << termcolor::reset;
			break;
		case 2:
			std::cout << termcolor::color<230,16,34>;
			break;
		case 4:
			std::cout << termcolor::color<16,230,41>;
			break;
		case 8:
			std::cout << termcolor::color<25,138,230>;
			break;
		case 16:
			std::cout << termcolor::color<230,21,177>;
			break;
		case 32:
			std::cout << termcolor::color<219,230,21>;
			break;
		case 64:
			std::cout << termcolor::color<230,59,11>;
			break;
		case 128:
			std::cout << termcolor::color<230,99,11>;
			break;
		case 256:
			std::cout << termcolor::color<226,230,11>;
			break;
		case 512:
			std::cout << termcolor::color<175,230,11>;
			break;
		case 1024:
			std::cout << termcolor::color<126,219,252>;
			break;
		case 2048:
			std::cout << termcolor::color<183,126,252>;
			break;
		case 4096:
			std::cout << termcolor::color<252,126,173>;
			break;
		case 8192:
			std::cout << termcolor::color<126,126,252>;
			break;
		case 16384:
			std::cout << termcolor::color<128,23,26>;
			break;
		case 32768:
			std::cout << termcolor::color<23,128,73>;
			break;
		case 65536:
			std::cout << termcolor::color<232,167,81>;
			break;
	}
}

void Game::m_displayBoard() const
{
	for(int i = 0; i < m_size; i++)
	{
		for(int j = 0; j < m_size; j++)
		{
			m_colorPicker(m_b[i][j]);
			std::cout << std::setw(8);


			if (m_b[i][j] == 0)
				std::cout << ".";
			else
				std::cout << m_b[i][j];
		}

		std::cout << std::endl;
	}

	m_colorPicker(0);

	std::cout << std::endl << std::setw(20) << std::right 
		<< "Score: " << m_b.score << std::endl << std::endl;
}

void Game::m_displayHeader(void) const
{
	if (m_passed)
		std::cout << termcolor::green;
	else
		std::cout << termcolor::red;

	std::cout << "\n2048 by hamzacyberpatcher - meow meow meow\n\n" << termcolor::reset;
}

void Game::m_displayFooter(void) const
{
	std::cout << std::endl;

	std::cout << termcolor::cyan << "R-RESET  ";
	std::cout << termcolor::red << "Q-QUIT  ";
	std::cout << termcolor::magenta << "U-UNDO  ";

	std::ifstream in("./save.dat");

	if (in.is_open())
		std::cout << termcolor::green;
	else
		std::cout << termcolor::red << termcolor::crossed;

	std::cout << "L-LOAD";

	std::cout << termcolor::reset;

	std::cout << termcolor::blue << "  S-SAVE  ";
	std::cout << termcolor::bright_red << "D-DELETE" << std::endl << termcolor::reset;

	in.close();

}

void Game::m_display(void) const
{
#ifdef _WIN32
    system("cls"); // Windows system clears the whole screen
#else
    std::cout << "\033[1;1H" << std::endl; // Move cursor to the top
    std::cout << "\033[J"; // Clear from the cursor position downwards
#endif

	m_displayHeader();
    m_displayBoard();
	m_displayFooter();
}

void Game::m_upKey(void)
{
	m_b.up();
}

void Game::m_downKey(void)
{
	m_b.down();
}

void Game::m_leftKey(void)
{
	m_b.left();
}

void Game::m_rightKey(void)
{
	m_b.right();
}

void Game::m_handleInput(void)
{
	m_display();
	char ch;
		while (true) 
		{

			ch = getchar();
			if (ch == '\033')
			{ // Escape character
				getchar(); // Skip the '[' character
				switch (getchar()) 
				{ // Get the final character
					case 'A':
						m_upKey();
						break;
					case 'B':
						m_downKey();
						break;
					case 'C':
						m_rightKey();
						break;
					case 'D':
						m_leftKey();
						break;
				}

			} 
			else if (ch == 'q') 
			{ // 'q' to quit
                break;
			}
			else if (ch == 'u' || ch == 'U')
			{
				m_b.undo();
				m_display();
			}
			else if (ch == 'r' || ch == 'R')
			{
				char choice;
				disableRawMode();
				std::cout << "Do u really want to restart the game????? (y/n): ";
				std::cin >> choice;

				if (choice == 'y' || choice == 'Y') m_reset();

				enableRawMode();
			}
			else if (ch == 's' || ch == 'S')
			{
				char choice;
				disableRawMode();
				std::cout << "Wanna save the game??? (y/n): ";
				std::cin >> choice;

				if(choice == 'y' || choice == 'Y') m_save();

				enableRawMode();
			}
			else if (ch == 'l' || ch == 'Y')
			{
				char choice;
				disableRawMode();
				std::cout << "Wanna load a game??? (y/n): ";
				std::cin >> choice;

				if(choice == 'y' || choice == 'Y') m_load();

				enableRawMode();
			}
			else if (ch == 'd' || ch == 'D')
			{
				char choice;
				disableRawMode();
				std::cout << "Wanna delete the old game??? (y/n): ";
				std::cin >> choice;

				if (choice == 'y' || choice == 'Y') m_deleteSave();

				enableRawMode();
			}

			if (m_b.change())
				m_display();


			if (!m_b.verticalMoveCheck() && !m_b.horizontalMoveCheck() && !m_b.zeroTileCheck())
			{
				disableRawMode();
				char choice;
				std::cout << "game over....\n";
				std::cout << "Reset the board???? (y/n): ";
				std::cin >> choice;
				enableRawMode();
				if (choice == 'y' || ch == 'Y')
					m_reset();
				else
					break;
			}

			if (!m_passed)
			{
				if (m_b.check2048())
				{
					int choice;

					disableRawMode();

					std::cout << "You WON!!!!!!!!!!!!!!" << std::endl;
					std::cout << "1 for continue, 2 for reset: ";
					std::cin >> choice;

					enableRawMode();

					if (choice == 1) m_passed = true;
					else
					{
						m_reset();
						m_passed = false;
					}
				}
			}
		}
}

void Game::run(void)
{
	enableRawMode();
	m_handleInput();
	disableRawMode();
	std::cout << std::endl;
}

void Game::m_save(void)
{
	std::ofstream out;

	out.open("./save.dat");

	for(int i = 0; i < m_size; i++)
	{
		for(int j = 0; j < m_size; j++)
		{
			out << m_b[i][j] << std::endl;
		}
	}

	out << m_b.score << std::endl;

	for(int i = 0; i < m_size; i++)
	{
		for(int j = 0; j < m_size; j++)
		{
			out << m_b.prevBoard[i][j] << std::endl;
		}
	}

	out << m_b.prevScore << std::endl;
	out << m_passed << std::endl;

	out.close();
}

void Game::m_load(void)
{
	std::ifstream in;

	in.open("./save.dat");

	if (!in.is_open())
	{
		std::cout << "\nLoading error!!!!" << std::endl;
		disableRawMode();
		getche();
		enableRawMode();
		return;
	}

	for(int i = 0; i < m_size; i++)
	{
		for(int j = 0; j < m_size; j++)
		{
			in >> m_b[i][j];
		}
	}

	in >> m_b.score;

	for(int i = 0; i < m_size; i++)
	{
		for(int j = 0; j < m_size; j++)
		{
			in >> m_b.prevBoard[i][j];
		}
	}

	in >> m_b.prevScore;

	in >> m_passed;


	in.close();
}

void Game::m_deleteSave(void)
{
	if(remove("./save.dat") == 0)
	{
		std::cout << "\nDeleted successfully!!!" << std::endl;
	}
	else
	{
		std::cout << "\nCouldn't delete sorry!!!" << std::endl;
	}

	disableRawMode();

	getche(false);

	enableRawMode();
}

void Game::m_reset(void)
{
	m_b.reset();
	m_passed = false;
}
