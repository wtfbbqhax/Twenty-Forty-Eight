#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cstring>

#include <unistd.h>
#include <assert.h>

#include "game.h"

void Game::notify()
{
    for (auto obs : views)
        obs->update();
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

#define getCol(grid,n)      \
{                           \
    grid[0][n], grid[1][n], \
    grid[2][n], grid[3][n]  \
}

#define getRow(grid,n)      \
{                           \
    grid[n][0], grid[n][1], \
    grid[n][2], grid[n][3]  \
}

void Game::move(Move m)
{
    for (int n=0; n<_grid.size(); n++)
    switch (m)
    {
        case Up:
        {
            Tile tile[] = getCol(_grid,n);
            //Tile tile[] = {_grid[0][n],_grid[1][n],_grid[2][n],_grid[3][n]};
            _score += _move(tile,_grid.size());
            _grid[0][n]=tile[0]; _grid[1][n]=tile[1];
            _grid[2][n]=tile[2]; _grid[3][n]=tile[3];
            break;
        }
        case Left:
        {
            Tile tile[] = getRow(_grid,n);
            //Tile tile[] = {_grid[n][0],_grid[n][1],_grid[n][2],_grid[n][3]};
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

    this->gameover = !addRandomTile();

    notify();
}

bool Game::addRandomTile(Tile tile)
{
    vector<pair<int,int> > empty_list;

    for (size_t x=0; x<_grid.size(); x++)
    for (size_t y=0; y<_grid.size(); y++)
        if (!_grid[x][y])
            empty_list.push_back(make_pair(x,y));

    if (!empty_list.size())
        return false;

    srand(unsigned(time(0)));
    random_shuffle(empty_list.begin(), empty_list.end()); 

    int x = get<0>(empty_list[0]);
    int y = get<1>(empty_list[0]);
    _grid[x][y] = tile;

    return true;
}

bool Game::addRandomTile()
{
    Tile _2or4 = (rand() % 100) < 90 ? 2 : 4; 
    return addRandomTile(_2or4);
}

// Observers
void Display::update()
{
    Game *game = this->model();

    cout << "\033[?25h\033[0m\033[H\033[2J" << endl;
    cout << "   Score: " << game->score() << endl;

    for (auto const& line: game->grid())
    {
        for (auto const& tile: line) 
        {
            if (!tile)
            {
                cout << "    .";
            }
            else
            {
                cout << setw(5) << tile;
            }
        }
        cout << endl << endl;
    }

    if ( game->gameOver() )
    {
        cout << "Game Over" << endl;
    }
}
