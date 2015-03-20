#include <iostream>
//#include <stdio.h>
//#include <stdlib.h>
#include <time.h>
//#include <cstring>

//#include <unistd.h>
#include <assert.h>

#include "game.h"

void Game::notify()
{
    for (auto obs : views)
        obs->update();
}

void Game::notifyReset()
{
    for (auto obs : views)
    {
        obs->reset();
        obs->update();
    }
}

static void slide(Tile tile[], int n)
{
    for ( int a=0,b=1; b<n; ++b )
    {
        if (!tile[a]) {
            if (!tile[b])
                continue;

            tile[a] = tile[b];
            tile[b] = 0;
        }

        a++;
    }
}

static unsigned merge(Tile tile[], int n)
{
    unsigned rval = 0;

    if (n<2)
        return 0;

    Tile &a = tile[0];
    Tile &b = tile[1];
    int mov = 1;

    if (a!=0 && a==b)
    {
        a *= 2;
        b = 0;
        mov++;

        rval = a;
    }

    return merge(&tile[mov], n-mov) + rval;
}

static unsigned _move(Tile tile[], int n)
{
    unsigned rval = 0;

    slide(tile,n);
    rval = merge(tile,n);
    slide(tile,n);

    return rval;
}

void Game::move(Move m)
{
    Grid prev = _grid;
    for (int n=0; n<_grid.size(); n++)
        switch (m)
        {
            case Up:
            {
                Tile tile[] = {_grid[0][n],_grid[1][n],_grid[2][n],_grid[3][n]};
                _score += _move(tile,_grid.size());
                _grid[0][n]=tile[0]; _grid[1][n]=tile[1];
                _grid[2][n]=tile[2]; _grid[3][n]=tile[3];
                break;
            }
            case Left:
            {
                Tile tile[] = {_grid[n][0],_grid[n][1],_grid[n][2],_grid[n][3]};
                _score += _move(tile,_grid.size());
                _grid[n][0]=tile[0]; _grid[n][1]=tile[1];
                _grid[n][2]=tile[2]; _grid[n][3]=tile[3];
                break;
            }
            case Down:
            {
                Tile tile[] = {_grid[3][n],_grid[2][n],_grid[1][n],_grid[0][n]};
                _score += _move(tile,_grid.size());
                _grid[3][n]=tile[0]; _grid[2][n]=tile[1];
                _grid[1][n]=tile[2]; _grid[0][n]=tile[3];
                break;
            }
            case Right:
            {
                Tile tile[] = {_grid[n][3],_grid[n][2],_grid[n][1],_grid[n][0]};
                _score += _move(tile,_grid.size());
                _grid[n][3]=tile[0]; _grid[n][2]=tile[1];
                _grid[n][1]=tile[2]; _grid[n][0]=tile[3];
                break;
            }
        }

    if (_grid != prev)
    {
        addRandomTile();
        notify();
    }
}

bool Game::addRandomTile(Tile tile)
{
    if ( _grid.isFull() )
        return false;

    CoordList emptys = _grid.findAll(0);

    srand(unsigned(time(0)));
    random_shuffle(emptys.begin(),emptys.end()); 
    _grid.set(emptys[0],tile);

    return true;
}

bool Game::addRandomTile()
{
    Tile _2or4 = (rand() % 100) < 90 ? 2 : 4; 
    return addRandomTile(_2or4);
}

const char *colors[] = {
    "\033[38;5;63m",
    "\033[38;5;69m",  /* 4    */
    "\033[38;5;75m",  /* 8    */
    "\033[38;5;81m",  /* 16   */
    "\033[38;5;87m",  /* 32   */
    "\033[38;5;118m",  /* 64   */
    "\033[38;5;119m", /* 128  */
    "\033[38;5;120m", /* 256  */
    "\033[38;5;190m", /* 512  */
    "\033[38;5;220m", /* 1024 */
    "\033[38;5;226m", /* 2048 */
};

/* TODO: Identify out a oneliner bitshift to
 *       replace  this  brainfart   solution.
 */
static inline const char *
tile_color(Tile t)
{
    switch (t) {
    case 2: return colors[ 0 ];    
    case 4: return colors[ 1 ];    
    case 8: return colors[ 2 ];    
    case 16: return colors[ 3 ];   
    case 32: return colors[ 4 ];   
    case 64: return colors[ 5 ];   
    case 128: return colors[ 6 ];  
    case 256: return colors[ 7 ];  
    case 512: return colors[ 8 ];  
    case 1024: return colors[ 9 ]; 
    // Highlighting logic bombs after surpassing
    // 2048  tile.  Just  like  the  real  game!
    case 2048: return colors[ 10 ];
    default: return "";
    }
}

// Observers
void Display::update()
{
    Game *game = this->model();
    Grid grid = game->grid();

    char prompt[1024];

    cout << "\033[?25h\033[0m\033[H\033[2J"
         << endl
         << "         SCORE: " << game->score() << endl << endl;
    for (auto const& line: grid)
    {
        for (auto const& tile: line) 
        {
            if (!tile)
            {
                cout << "         .";
            }
            else
            {
                cout << tile_color(tile) << setw(10) << tile << "\033[0m";
            }
        }
        cout << endl << endl << endl << endl;
    }

    if ( game->gameOver() )
        cout << "Game Over" << endl;
}
