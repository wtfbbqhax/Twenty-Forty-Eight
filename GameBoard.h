/*
 * GameBoard.h
 *
 *  Created on: May 15, 2014
 *      Author: kmccormick
 */

#pragma once

namespace Game {
class Board {
  public:

	Board();
	virtual ~Board();

	bool move(int direction);
	void printBoard();
	//int* getBoard();
	bool gameOver();

  private:

	bool setRandCell();
	bool peek(int direction);
	unsigned int board[16];
	bool over;
};
} // namespace Game
