/* GameBoard.h */

#pragma once

namespace Game {
class Board {
  public:

	Board();
	virtual ~Board();

	bool Move(int direction);
	void Print();
	bool GameOver();

  private:

	bool SetRandomTile();
	bool Peek(int direction);
	unsigned board[16];
	bool over;
};
} // namespace Game
