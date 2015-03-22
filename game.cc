#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
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

uint64_t Random()
{
    uint64_t value = 0;
    int i;

    FILE *fp = fopen("/dev/random", "r");
    if (!fp)
    {
        return time(0);
    }
    for (i=0; i<sizeof(value); i++) {
        value <<= 8;
        value |= fgetc(fp);
    }
    fclose(fp);
    return value;
}


unsigned int devrand(void)
{
    int fn;
    unsigned int r;

    fn = open("/dev/urandom", O_RDONLY);
    if (fn == -1)
        assert(false);

    if ( read(fn, &r, 4) != 4)
        assert(false);

    close(fn);
    return r;
}

static unsigned int x = 20226789,y = 362493900,z = 521285629,c = 76543221;
/* Initialise KISS generator using /dev/urandom */
void init_KISS()
{
    x = devrand();
    while (!(y = devrand())); /* y must not b e zero !  */
    z = devrand();
    c = devrand() % 698769068 + 1;
}
/* Seed variables */
unsigned int KISS()
{
    unsigned long long t, a = 698769069ULL;
    x = 69069*x+12345;
    y ^= (y<<13);
    y ^= (y>>17);
    y ^= (y<<5);
    /* y must never be set to zero! */
    t = a*z+c;
    c = (t>>32);
    /* Also a void setting z=c=0! */
    return x+y+(z=t);
}


bool Game::addRandomTile(Tile tile)
{
    if ( _grid.isFull() )
        return false;

    CoordList emptys = _grid.findAll(0);

    //srand(unsigned(time(0));
    init_KISS();
    srand(KISS());
    random_shuffle(emptys.begin(),emptys.end()); 
    _grid.set(emptys[0],tile);

    return true;
}

bool Game::addRandomTile()
{
    Tile _2or4 = (rand() % 100) < 90 ? 2 : 4; 
    return addRandomTile(_2or4);
}
