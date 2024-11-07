#include "board.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

Board::Board() {}

Board::~Board()
{
	for(int i = 0; i < m_size; i++)
	{
		delete [] m_board[i];
		delete [] prevBoard[i];
		prevBoard[i] = nullptr;
		m_board[i] = nullptr;
	}
	delete [] m_board;
	delete [] prevBoard;
	m_board = nullptr;
}

void Board::setBoard(int size)
{
	if (m_board != nullptr)
	{
		for(int i = 0; i < m_size; i++)
		{
			delete [] prevBoard[i];
			delete [] m_board[i];
		}
		delete [] m_board;
		delete [] prevBoard;
	}

	m_size = size;

	m_board = new int*[m_size];
	prevBoard = new int*[m_size];
	for(int i = 0; i < m_size; i++)
	{
		prevBoard[i] = new int[m_size];
		m_board[i] = new int[m_size];
	}

	m_addTile();
	m_addTile();
}

void Board::m_addTile(void)
{
	srand(time(0));

	int i, j;

	do
	{
		i = rand() % m_size;
		j = rand() % m_size;
	} while(m_board[i][j] != 0);

	int chance = rand() % 10;

	if (chance == 9)
		m_board[i][j] = 4;
	else
		m_board[i][j] = 2;

}

int* Board::operator[](int row)
{
	return m_board[row];
}

const int* Board::operator[](int row) const
{
	return m_board[row];
}

void Board::backup(void)
{
	for(int i = 0; i < m_size; i++)
		for(int j = 0; j < m_size; j++)
			prevBoard[i][j] = m_board[i][j];

	prevScore = score;
}

bool Board::change(void)
{
	int flag = false;

	for(int i = 0; i < m_size && !flag; i++)
		for(int j = 0; j < m_size && !flag; j++)
			if (m_board[i][j] != prevBoard[i][j])
				flag = true;

	return flag;
}

void Board::m_stack(void)
{
	int fill = 0;

	for(int i = 0; i < m_size; i++)
	{
		int fill = 0;

		for(int j = 0; j < m_size; j++)
		{
			if (m_board[i][j] != 0)
			{
				std::swap(m_board[i][fill], m_board[i][j]);
				fill++;
			}
		}
	}
}

void Board::m_combine(void)
{
	for(int i = 0; i < m_size; i++)
	{
		for(int j = 0; j < m_size - 1; j++)
		{
			if (m_board[i][j] == m_board[i][j+1])
			{
				m_board[i][j] *= 2;
				score += m_board[i][j];
				m_board[i][j + 1] = 0;
			}
		}
	}
}

void Board::m_reverse(void)
{
	for(int i = 0; i < m_size; i++)
	{
		for(int j = 0; j < m_size / 2; j++)
		{
			std::swap(m_board[i][j], m_board[i][m_size - j - 1]);
		}
	}
}


void Board::m_transpose(void)
{
	for(int i = 0; i < m_size; i++)
	{
		for(int j = i + 1; j < m_size; j++)
		{
			std::swap(m_board[i][j], m_board[j][i]);
		}
	}
}

void Board::up(void)
{
	backup();

	m_transpose();
	m_stack();
	m_combine();
	m_stack();
	m_transpose();
	
	if (zeroTileCheck() && change())
		m_addTile();

}

void Board::down(void)
{
	backup();

	m_transpose();
	m_reverse();
	m_stack();
	m_combine();
	m_stack();
	m_reverse();
	m_transpose();
	
	if (zeroTileCheck() && change())
		m_addTile();
}

void Board::left(void)
{
	backup();

	m_stack();
	m_combine();
	m_stack();

	if (zeroTileCheck() && change())
		m_addTile();

}

void Board::right(void)
{
	backup();

	m_reverse();
	m_stack();
	m_combine();
	m_stack();
	m_reverse();

	if (zeroTileCheck() && change())
		m_addTile();

}

bool Board::zeroTileCheck(void) const
{
	bool found = false;

	for(int i = 0; i < m_size && !found; i++)
	{
		for(int j = 0; j < m_size && !found; j++)
		{
			if (m_board[i][j] == 0)
			{
				found = true;
			}
		}
	}
	



	return found;
}

bool Board::horizontalMoveCheck(void) const
{

	bool found = false;

	for(int i = 0; i < m_size && !found; i++)
	{
		for(int j = 0; j < m_size - 1 && !found; j++)
		{
			if (m_board[i][j] == m_board[i][j+1])
			{
				found = true;
			}
		}
	}




	return found;
}

bool Board::verticalMoveCheck(void) const
{

	bool found = false;

	for(int i = 0; i < m_size - 1 && !found; i++)
	{
		for(int j = 0; j < m_size && !found; j++)
		{
			if (m_board[i][j] == m_board[i+1][j])
			{
				found = true;
			}
		}
	}


	return found;
}

void Board::reset(void)
{
	for(int i = 0; i < m_size; i++)
		for(int j = 0; j < m_size; j++)
			m_board[i][j] = prevBoard[i][j] = 0;

	score = 0;

	m_addTile();
	m_addTile();
}

bool Board::check2048(void)
{
	int found = false;
	for(int i = 0; i < m_size && !found; i++)
		for(int j = 0; j < m_size && !found; j++)
			if (m_board[i][j] == 2048)
				found = true;

	return found;
}

void Board::undo(void)
{
	for(int i = 0; i < m_size; i++)
		for(int j = 0; j < m_size; j++)
			m_board[i][j] = prevBoard[i][j];

	score = prevScore;

}
