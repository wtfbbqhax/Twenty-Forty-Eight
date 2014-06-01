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
    Game::Board board;
	int move = 1;
	bool made = false;

	while ( (move != 0) && board.GameOver() )
	{
		board.Print();

		cout << "Enter a move: ";
		cin >> move;

		while ( (move % 2) || move > 9 )
		{
			cout << "[!] Nope" << endl;
			cin >> move;
		}

		if ( move )
			made = board.Move(move);

		if ( !made )
			cout << "[!] Invalid" << endl;
	}

	cout << "[!] Final Score!" << endl;
	board.Print();

	return 1;
}



