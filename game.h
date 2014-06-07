/* GameBoard.h */
#pragma once

#define COLS (4)
#define ROWS (4)

class Game {
  private:
	unsigned board[COLS][ROWS];
	bool Update(void);
	bool Peek(void);

  public:
    enum Direction {
        UP, DOWN, LEFT, RIGHT 
    };

	//Game();
    //virtual ~Game();

    void Reset(void);
	bool Move( Direction );
	void Print(void);
	bool GameOver(void);
};
