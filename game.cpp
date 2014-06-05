/* GameBoard.cpp */

// wtfbbqhax, 05-31-2014:
//  * Unrolling loops will ease the eye strain
//  * #define MOVE_UP, MOVE_DOWN etc..
//  * Move board rendering to a separate class

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cstring>

#include <assert.h>

#include "game.h"

namespace Game {

Board::Board()
{
    // TODO Auto-generated constructor stub
    for ( int x=0; x<4; x++ )
    for ( int y=0; y<4; y++ )
        board[x][y] = 0;

    this->Update();
    this->Update();
    over = false;
}

// TODO Auto-generated destructor stub
Board::~Board() { }

static void slide(unsigned ln[], int n)
{
    for ( int a=0,b=1; b<n; ++b ) {
        // a,b,_,_  a,b,_,_  a,b,_,_
        // 0,0,0,2  0,2,0,2  2,0,2,0
        // a,_,b,_  _,a,b,_  _,a,b,_
        // 0,0,0,2  2,0,0,2  2,0,2,0
        // a,_,_,b  _,a,_,b  _,a,b,_
        // 0,0,0,2  2,0,0,2  2,2,0,0
        // a,_,_,b  _,a,_,b  _,_,a,b
        // 2,0,0,0  2,2,0,0  2,2,0,0

        if (!ln[a])
        {
            if (!ln[b])
                continue;

            ln[a++] = ln[b];
            ln[b] = 0;
        }
        else
        {
            a++;
        }
    }
}

static void merge(unsigned ln[], int n)
{
    if ( n<2 )
        return;

    unsigned &t1 = ln[n-1];
    unsigned &t2 = ln[n-2];

    // _  _ t2 t1   n=4   _  _ t2 t1   n=4
    // 2  2  2  0         2  2  2  2
    // _ t2 t1  _   n=3   2  2  4  0
    // 2  2  2  0         t2 t1 _  _   n=2
    // 2  4  0  0         2  2  4  0
    //              n<2   4  0  4  0
                         
    if ( t1 && (t1==t2) )
    {
        --n;
        t2 += t1;
        t1 = 0;
    }

    merge(ln, --n);
}

bool Board::Move( MoveDir dir )
{
    unsigned save[4][4];
    memcpy(save, board, sizeof(save));

    switch ( dir )
    {
        case MoveUp:
        for ( int x=0; x<4; x++ ) {
            unsigned tile[4] = { 
                board[x][0], board[x][1],
                board[x][2], board[x][3] 
            };

            slide(tile,4);
            merge(tile,4);
            slide(tile,4);

            board[x][0]=tile[0];
            board[x][1]=tile[1];
            board[x][2]=tile[2];
            board[x][3]=tile[3];
        }
        break;

    case MoveDown:
        for ( int x=0; x<4; x++ ) {
            unsigned tile[4] = { 
                board[x][3], board[x][2],
                board[x][1], board[x][0] 
            };

            slide(tile,4);
            merge(tile,4);
            slide(tile,4);

            board[x][3]=tile[0];
            board[x][2]=tile[1];
            board[x][1]=tile[2];
            board[x][0]=tile[3];
        }
        break;

    case MoveRight:
        for ( int y=0; y<4; y++ ) {
            unsigned tile[4] = { 
                board[3][y], board[2][y],
                board[1][y], board[0][y] 
            };

            slide(tile,4);
            merge(tile,4);
            slide(tile,4);

            board[3][y]=tile[0];
            board[2][y]=tile[1];
            board[1][y]=tile[2];
            board[0][y]=tile[3];
        }
        break;

    case MoveLeft:
        for ( int y=0; y<4; y++ ) {
            unsigned tile[4] = { 
                board[0][y], board[1][y],
                board[2][y], board[3][y]
            };

            slide(tile,4);
            merge(tile,4);
            slide(tile,4);

            board[0][y]=tile[0];
            board[1][y]=tile[1];
            board[2][y]=tile[2];
            board[3][y]=tile[3];
        }
        break;
    }

    if (!memcmp(board, save, sizeof(board)))
        return false;

    this->Update();
    return true;
}

void Board::Print()
{
    //puts("\033[?25h\033[0m\033[H\033[2J");
    for ( int y=0; y<4; y++ )
    {
        for ( int x=0; x<4; x++ )
        {
            if ( !board[x][y] )
                printf(". ");
            else
                printf("%d ", board[x][y]);
        }
        puts("");
    }
}

// TODO Split this up
bool Board::Update()
{
    unsigned empty[16][2];
    int n = 0;

    // collect coordinates of all empty tiles
    for ( int x=0; x<4; x++ )
    for ( int y=0; y<4; y++ )
    if ( board[x][y] == 0 )
    {
        empty[n][0] = x;
        empty[n][1] = y;
        n++;
    }

    if ( n == 0 )
        return false; 

    // shuffle available tiles
    srand(time(NULL));
    int i = (rand() % n);
    int x = empty[i][0];
    int y = empty[i][1];

    srand(time(NULL));
    int _2or4 = (rand() % 10) < 9 ? 2 : 4;
    board[x][y] = _2or4;

    return true;
}

// Looks over complicated
bool Board::GameOver(void)
{
    bool p1 = this->Peek(MoveUp);
    bool p2 = this->Peek(MoveRight);
    bool p3 = this->Peek(MoveDown);
    bool p4 = this->Peek(MoveLeft);

    if ( p1 || p2 || p3 || p4 )
        return true;

    return false;
}

bool Board::Peek( MoveDir dir )
{
    // Look Up
    for ( int x=0; x<4; x++ )
    {
        unsigned tile[4] = { 
            board[x][3], board[x][2],
            board[x][1], board[x][0] 
        };

        slide(tile,4);
        merge(tile,4);
        slide(tile,4);

        if (board[x][3] != tile[0] or 
            board[x][2] != tile[1] or 
            board[x][1] != tile[2] or 
            board[x][0] != tile[3])
            return true;
    }

    // Look Left 
    for ( int y=0; y<4; y++ )
    {
        unsigned tile[4] = { 
            board[0][y], board[1][y],
            board[2][y], board[3][y]
        };

        slide(tile,4);
        merge(tile,4);
        slide(tile,4);

        if (board[0][y] != tile[0] or 
            board[1][y] != tile[1] or 
            board[2][y] != tile[2] or 
            board[3][y] != tile[3])
            return true;
    }

    return true;
}

} // namespace Game
