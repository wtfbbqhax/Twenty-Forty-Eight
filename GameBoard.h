/*
 * GameBoard.h
 *
 *  Created on: May 15, 2014
 *      Author: kmccormick
 */

#ifndef GAMEBOARD_H_
#define GAMEBOARD_H_

namespace Game {

class GameBoard {
public:
	GameBoard();
	virtual ~GameBoard();
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

} /* namespace Game */
#endif /* GAMEBOARD_H_ */
