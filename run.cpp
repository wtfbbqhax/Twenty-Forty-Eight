/*
 * run.cpp
 *
 *  Created on: May 16, 2014
 *      Author: kmccormick
 */
#include <iostream>
#include <stdio.h>
#include "GameBoard.h"

using namespace std;


int main(int argc, char *argv[])
{
    Game::Board * game = new Game::Board();
	int move = 1;
	bool made = false;

	while ( (move != 0) && game->gameOver() )
	{
		game->printBoard();

		cout << "Enter a move: ";
		cin >> move;

		while ( (move % 2) || move > 9 )
		{
			cout << "[!] Nope" << endl;
			cin >> move;
		}

		if ( move )
			made = game->move(move);

		if ( !made )
			cout << "[!] Invalid" << endl;
	}

	cout << "[!] Final Score!" << endl;
	game->printBoard();

	return 1;
}



