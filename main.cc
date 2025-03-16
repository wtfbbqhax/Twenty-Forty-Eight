/* main.cpp */ 

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <assert.h>

#include <termios.h>

#include "cvar.h"
#include "game.h"

#include "display.h"
#include "debug.h"

using namespace std;

enum { 
    PROMPT = 256
};

cvar_t * g_debug;
cvar_t * g_cheats;
cvar_t * g_drawResetAnim;
cvar_t * g_console;
cvar_t * g_consoleSize;

// Escape sequence '^['.
// Adds support for the arrow keys.
int UNESCAPE(int c)
{
    if (c!=0x1B)
        return c;

    if ((c=getchar())!=0x5B)
        return c;

    switch ((c=getchar())) {
        case 'A': return 'k'; // arrow up
        case 'B': return 'j'; // arrow down
        case 'C': return 'l'; // arrow right
        case 'D': return 'h'; // arrow left
    }
    return c;
}

struct termios saved, term;

int main()
{
    Game game;
    Display display(&game);
    //DebugDisplay display(&game);
    Undo undo(&game);

//    struct termios saved, term;
    tcgetattr(STDIN_FILENO, &saved);
    term = saved;

    term.c_lflag &= ~(ECHO|ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);

    Cvar_PreInit();
    Cvar_Init();

    g_cheats        = Cvar_Get("g_cheats", "0", CVAR_LATCH);
    g_drawResetAnim = Cvar_Get("g_drawResetAnim", "1", CVAR_LATCH);
    g_console       = Cvar_Get("console", "0", 0);
    g_consoleSize   = Cvar_Get("consoleSize", "10", 0);

    int c = 'r';
    do {
        switch (UNESCAPE(c))
        {
            case '~': 
             if (Cvar_GetIntegerValue(g_console) > 0)
                 Cvar_Set("console", "0");
             else
                 Cvar_Set("console", "1");
             game.notify();
             break;
            
            case 'r': 
             // TODO: Reset CVAR_LATCH vars
             //Cvar_Set("g_cheats", "0");
             game.reset();
             break;

            // 
            // Undo move
            //
            case 'u': 
             if (Cvar_CheatsAllowed())
                 undo.undo();
             break;

            //
            // Up-Down-Left-Right controls
            //
            case 'k': game.move(Up); break;
            case 'j': game.move(Down); break;
            case 'l': game.move(Right); break;
            case 'h': game.move(Left); break;

            /* TODO Add  command registration  too Display 
             *      for  plugable  debug  commands  inside
             */
            case 'i':
              if (Cvar_CheatsAllowed())
              {
                game.insert(8);
                game.insert(16);
                game.insert(32);
                game.insert(64);
                game.insert(128);
                game.insert(256);
                game.insert(512);
                game.insert(1024);
                game.insert(2048);
              }
              break;

            case 'q': goto quit;
        }
	} while ( (c = tolower(getchar())) );

quit:

    Cvar_Shutdown();
    tcsetattr(STDIN_FILENO, TCSANOW, &saved);
	return 0;
}
