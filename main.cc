/* main.cpp */ 

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <assert.h>

#include <termios.h>

#include "game.h"

using namespace std;

// Escape sequence '^['.
// Adds support for the arrow keys.
int UNESCAPE(int c)
{
    if (c!=0x1B)
        return c;

    if ((c=getchar())!=0x5B)
        return c;

    switch ((c=getchar())) {
        case 'A': return 'k';// up
        case 'B': return 'j';// down
        case 'C': return 'l';// right
        case 'D': return 'h';// left
    }
    return c;
}

int main( int argc, char *argv[] )
{
    Game game;
    Display display(&game);

    struct termios saved, term;
    tcgetattr(STDIN_FILENO, &saved);
    term = saved;

    term.c_lflag &= ~(ECHO|ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);

    int c = 'r';
    do {
        switch (UNESCAPE(c)) {
            case 'r': game.reset(); break;
            case 'k': game.move(Up); break;
            case 'j': game.move(Down); break;
            case 'l': game.move(Right); break;
            case 'h': game.move(Left); break;
            case 'q': goto quit;
        }

	} while ( (c = tolower(getchar())) );

quit:
    tcsetattr(STDIN_FILENO, TCSANOW, &saved);
	return 0;
}
