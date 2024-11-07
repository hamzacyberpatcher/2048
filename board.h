#ifndef H_2048
#define H_2048

class Board
{
	int m_size = 0;
	int ** m_board = nullptr;

	void m_addTile(void);
	void m_stack(void);
	void m_combine(void);
	void m_reverse(void);
	void m_transpose(void);

	public:

	int ** prevBoard;
	int prevScore = 0;
	int score = 0;

	Board();
	~Board();
	void setBoard(int size);

	int* operator[](int row);
	const int* operator[](int row) const;


	void up(void);
	void down(void);
	void left(void);
	void right(void);


	bool zeroTileCheck(void) const;
	bool horizontalMoveCheck(void) const;
	bool verticalMoveCheck(void) const;

	void backup(void);
	bool change(void);

	void reset(void);

	bool check2048(void);

	void undo(void);

};

#endif
