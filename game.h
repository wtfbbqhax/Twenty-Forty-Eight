/* GameBoard.h */
#pragma once

namespace Game {

enum MoveDir {
    MoveUp, MoveDown, MoveLeft, MoveRight
};

class Board {
  private:
	unsigned board[4][4];
	bool over;

	bool Update(void);
	bool Peek( MoveDir );

  public:
	Board();
	virtual ~Board();
	bool Move( MoveDir );
	void Print(void);
	bool GameOver(void);
};

} // namespace Game
