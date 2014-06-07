/* main.cpp */ 

#include <stdio.h>
#include <unistd.h>
#include <ctype.h>

#include <iostream>

#include <termios.h>

#include "game.h"

using namespace std;

int main( int argc, char *argv[] )
{

    Game game;

    struct termios saved, term;
    tcgetattr(STDIN_FILENO, &saved);
    term = saved;

    term.c_lflag &= ~(ECHO|ICANON);
    //cfmakeraw(&term);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);

    int c = 'r';
    do 
    {
        switch (c) {
            case 'r': game.Reset(); break;
            case 'k': game.Move(Game::UP); break;
            case 'h': game.Move(Game::LEFT); break;
            case 'j': game.Move(Game::DOWN); break;
            case 'l': game.Move(Game::RIGHT); break;
            case 'q': goto quit;
        }
	    game.Print();
        //cout << "  k:UP  j:DOWN  h:LEFT  l:RIGHT"
        //     << "  r:RESET  q:QUIT\n";
//        if ( game.GameOver() )
//            break;
        usleep(5000);
	} while ( (c = tolower(getchar())) );

quit:
    tcsetattr(STDIN_FILENO, TCSANOW, &saved);

	//cout << "[!] Final Score!" << endl;
	//board.Print();

	return 0;
}
