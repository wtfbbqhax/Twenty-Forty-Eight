#include <assert.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <time.h>
#include <unistd.h>

#include <algorithm>
#include <random>
#include <iostream>

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
    for (size_t n=0; n<_grid.size(); n++)
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
    
    std::random_device rd;
    std::mt19937 rng(rd());
    std::shuffle(emptys.begin(), emptys.end(), rng);
    _grid.set(emptys[0],tile);

    return true;
}

bool Game::addRandomTile()
{
    Tile _2or4 = (rand() % 100) < 90 ? 2 : 4; 
    return addRandomTile(_2or4);
}
