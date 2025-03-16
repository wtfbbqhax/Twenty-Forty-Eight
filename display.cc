#include <iostream>

#include <termios.h>
#include <unistd.h>
#include "q_trie.h"
#include "cvar.h"
#include "prompt.h"

#include "display.h"
#include "game.h"
#include "cmds.h"

extern struct termios saved, term;
extern cvar_t * g_drawResetAnim;
extern cvar_t * g_console;
extern cvar_t * g_consoleSize;


const char **colors;
const char *blues[] = {
    "\033[48;5;63m",  /* 2    */
    "\033[48;5;69m",  /* 4    */
    "\033[48;5;75m",  /* 8    */
    "\033[48;5;81m",  /* 16   */
    "\033[48;5;87m",  /* 32   */
    "\033[48;5;118m", /* 64   */
    "\033[48;5;119m", /* 128  */
    "\033[48;5;120m", /* 256  */
    "\033[48;5;190m", /* 512  */
    "\033[48;5;220m", /* 1024 */
    "\033[48;5;226m", /* 2048 */
};
const char *greens[] = {
    "\033[48;5;195m",  /* 2    */
    "\033[48;5;194m",  /* 4    */
    "\033[48;5;193m",  /* 8    */
    "\033[48;5;192m",  /* 16   */
    "\033[48;5;191m",  /* 32   */
    "\033[48;5;190m", /* 64   */
    "\033[48;5;229m", /* 128  */
    "\033[48;5;118m", /* 256  */
    "\033[48;5;228m", /* 512  */
    "\033[48;5;227m", /* 1024 */
    "\033[48;5;226m", /* 2048 */
};
const char *pinks[] = {
    "\033[48;5;225m",  /* 2    */
    "\033[48;5;219m",  /* 4    */
    "\033[48;5;213m",  /* 8    */
    "\033[48;5;207m",  /* 16   */
    "\033[48;5;212m",  /* 32   */
    "\033[48;5;200m", /* 64   */
    "\033[48;5;201m", /* 128  */
    "\033[48;5;199m", /* 256  */
    "\033[48;5;198m", /* 512  */
    "\033[48;5;197m", /* 1024 */
    "\033[48;5;196m", /* 2048 */
};
const char *browns[] = {
    "\033[48;5;181m",  /* 2    */
    "\033[48;5;183m",  /* 4    */
    "\033[48;5;189m",  /* 8    */
    "\033[48;5;194m",  /* 16   */
    "\033[48;5;192m",  /* 32   */
    "\033[48;5;228m", /* 64   */
    "\033[48;5;226m", /* 128  */
    "\033[48;5;202m", /* 256  */
    "\033[48;5;214m", /* 512  */
    "\033[48;5;208m", /* 1024 */
    "\033[48;5;202m", /* 2048 */
};
const char *purples[] = {
    "\033[48;5;5m",  /* 2    */
    "\033[48;5;4m",  /* 4    */
    "\033[48;5;3m",  /* 8    */
    "\033[48;5;10m",  /* 16   */
    "\033[48;5;2m",  /* 32   */
    "\033[48;5;1m", /* 64   */
    "\033[48;5;7m", /* 128  */
    "\033[48;5;8m", /* 256  */
    "\033[48;5;9m", /* 512  */
    "\033[48;5;3m", /* 1024 */
    "\033[48;5;11m", /* 2048 */
};

static inline const char *
tile_color(Tile t)
{
    switch (t) {
        case 2: return colors[ 0 ];    
        case 4: return colors[ 1 ];    
        case 8: return colors[ 2 ];    
        case 16: return colors[ 3 ];   
        case 32: return colors[ 4 ];   
        case 64: return colors[ 7 ];  
        case 128: return colors[ 6 ];  
        case 256: return colors[ 5 ];   
        case 512: return colors[ 8 ];  
        case 1024: return colors[ 9 ]; 
        case 2048: return colors[ 10 ];
        default: return "";
    }
}

void Display::clearScreen(void)
{ 
    cout << "\033[?25h\033[0m\033[H\033[2J" << endl;
}

void Display::drawWaterfall(void)
{
    const char *linecolors[4];

    for (int cnt = 0; cnt < 20; cnt++)
    {
        for (int i = 0; i < 4; ++i)
        {
            linecolors[0] = linecolors[1]
                = linecolors[2]
                = linecolors[3]
                = "";
            int idx=0;

            for (int j = 0; j < 4; ++j) 
            {
#define MARGIN  60
                Tile _2or4orEtc = (rand() % 100) < MARGIN ?    2
                                : (rand() % 100) < MARGIN ?    4
                                : (rand() % 100) < MARGIN ?   16
                                : (rand() % 100) < MARGIN ?   64
                                : (rand() % 100) < MARGIN ?  256
                                : (rand() % 100) < MARGIN ? 1024
                                : 2048; 
                linecolors[idx] = tile_color(_2or4orEtc);
                ++idx;
            }
        }

        for (int i=0; i < 5; ++i) {
            cout<< linecolors[0] << "          " << "\033[0m"
                << linecolors[1] << "          " << "\033[0m"
                << linecolors[2] << "          " << "\033[0m"
                << linecolors[3] << "          " << "\033[0m"
                << endl;
            usleep(9600);
        }
    }
}

void Display::Console(void)
{
    Game *game = this->model();
    char *in = simple_prompt("\033[1m\033[K|> ", 4096, true);
    console.push_back(in);
    Cmd_Exec(game, in);
    free(in);
}

void Display::reset(void)
{
    clearScreen();

    int i = rand() % 5;
    switch (i)
    {
        case 0:
            colors = blues;
            break;
        case 1:
            colors = greens;
            break;
        case 3:
            colors = pinks;
            break;
        case 4:
            colors = browns;
            break;
        default:
            colors = purples;
            break;
    }

    if (g_drawResetAnim->integer == 1)
        drawWaterfall();

};

void Display::update(void)
{
    Game *game = this->model();
    Grid grid = game->grid();
    const char *linecolors[4];
    int i;

    clearScreen();
    cout << "SCORE: " << game->score() << endl << endl;

    for (auto const& line: grid)
    {
        i = 0;
        linecolors[0] = linecolors[1] = linecolors[2] = linecolors[3] = "";

        for (auto const& tile: line) 
        {
            if (!tile)
            {
                cout << "          ";
            }
            else
            {
                linecolors[i] = tile_color(tile);
                cout << tile_color(tile) << setw(10) << tile << "\033[0m";
            }
            ++i;
        }

        cout << endl
            << linecolors[0] << "          " << "\033[0m"
            << linecolors[1] << "          " << "\033[0m"
            << linecolors[2] << "          " << "\033[0m"
            << linecolors[3] << "          " << "\033[0m"
            << endl
            << linecolors[0] << "          " << "\033[0m"
            << linecolors[1] << "          " << "\033[0m"
            << linecolors[2] << "          " << "\033[0m"
            << linecolors[3] << "          " << "\033[0m"
            << endl
            << linecolors[0] << "          " << "\033[0m"
            << linecolors[1] << "          " << "\033[0m"
            << linecolors[2] << "          " << "\033[0m"
            << linecolors[3] << "          " << "\033[0m"
            << endl
            << linecolors[0] << "          " << "\033[0m"
            << linecolors[1] << "          " << "\033[0m"
            << linecolors[2] << "          " << "\033[0m"
            << linecolors[3] << "          " << "\033[0m"
            << endl;
    }

    if (Cvar_GetIntegerValue(g_console) > 0)
    {
        tcsetattr(STDIN_FILENO, TCSANOW, &saved);
        for (unsigned n=0; n<console.size(); ++n) {
            size_t _size = (g_consoleSize->integer > 0) ? g_consoleSize->integer : 0;
            if (console.size() > _size)
                console.erase(console.begin());

            cout << "|> " << console.at( n ) << endl;
        }
        Console();
        tcsetattr(STDIN_FILENO, TCSANOW, &term);
        game->notify();
    }

    if ( game->gameOver() )
        cout << "Game Over" << endl;
}

