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
using namespace Game;
int main(){

	GameBoard* game=new GameBoard();
	int move=1;
	bool made=false;
	while((move!=0)&&(game->gameOver()))
	{
		game->printBoard();
		cout<<"Enter a move";
		cin>>move;
		while(((move%2)!=0)||(move>9))
		{
			cout<<"Invalid Move! Try agian\n";
			cin>>move;
		}

		if (move!=0)
		{
			made=game->move(move);
		}
		if(made==false)
		{
			cout<<"Invalid";
		}
	}
	cout<<"Final Score!\n";
	game->printBoard();
	return 1;


}



