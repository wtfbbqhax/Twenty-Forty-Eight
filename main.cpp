/* main.cpp */ 

#include <stdio.h>
#include <unistd.h>
#include <ctype.h>

#include <iostream>
#include "game.h"

using namespace std;

int main( int argc, char *argv[] )
{

    //cout << "\033[?25h\033[0m\033[H\033[2J\n\n";
    Game::Board board;
	board.Print();

    //cout << "\nPress '?' to display usage.\n";

    int c;
	while ( (c = tolower(getchar())) != 'q' )
    {
        getchar(); // consume newline

        bool ok;
        switch (c) {
        case 'k': ok=board.Move(Game::MoveUp); break;
        case 'j': ok=board.Move(Game::MoveDown); break;
        case 'h': ok=board.Move(Game::MoveLeft); break;
        case 'l': ok=board.Move(Game::MoveRight); break;
        case '?':
            cout << "\nUsage:\n";
            cout << "_input is case insensitive_\n\n";
            cout << "  k Up\n";
            cout << "  j Down\n";
            cout << "  h Left\n";
            cout << "  l Right\n";
            cout << "  ? Help, Q Quit\n\n";
            continue;
        }

        if ( !ok )
            cout << "\nNope\n\n";

	    board.Print();

        //cout << "print board" << endl;
        //if ( board.GameOver() )
        //    break;

        usleep(5000);
	}

	//cout << "[!] Final Score!" << endl;
	board.Print();

	return 0;
}
