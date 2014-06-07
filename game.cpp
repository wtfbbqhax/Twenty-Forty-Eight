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

#include <unistd.h>
#include <assert.h>

#include "game.h"

void Game::Reset(void)
{
    memset(board, 0, sizeof(board));
//    for ( int x=0; x<COLS; x++ )
//    for ( int y=0; y<ROWS; y++ )
//        board[x][y] = 0;

    this->Update();
    this->Update();
}

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

        if (!ln[a]) {
            if (!ln[b])
                continue;
            ln[a] = ln[b];
            ln[b] = 0;
        }

        a++;
    }
}

static void merge(unsigned ln[], int n)
{
    // a b      a b      a b    
    // 2|2|2|2  4|2|2|⁄  8|4|2|2
    //     a b    a b      a b  
    // 4|⁄|2|2  4|2|2|⁄  8|4|2|2
    //     a b    a b        a b
    // 4|⁄|4|⁄  4|4|⁄|⁄  8|4|2|2
    //                       a b
    //                   8|4|4|⁄
    //
    if (n<2) return;

    unsigned &a = ln[0];
    unsigned &b = ln[1];
    int mov = 1;

    if (a!=0 && a==b)
    {
        a *= 2;
        b = 0;
        mov++;
    }

    merge(&ln[mov], n-mov);
}

bool Game::Move( Direction dir )
{
    unsigned save[COLS][ROWS];
    unsigned frame[COLS][ROWS];

    memcpy(save, board, sizeof(save));
    memcpy(frame, board, sizeof(frame));

    int state = 0;
    do { 
    switch (dir) {
            case UP:
            for (int x=0; x<COLS; x++) {
                unsigned tile[COLS] = {
                    frame[x][0], frame[x][1],
                    frame[x][2], frame[x][3]
                };

                if (state==1) merge(tile,COLS);
                else          slide(tile,COLS);

                frame[x][0]=tile[0];
                frame[x][1]=tile[1];
                frame[x][2]=tile[2];
                frame[x][3]=tile[3];
            }
            break;

            case DOWN:
            for ( int x=0; x<COLS; x++ ) {
                unsigned tile[COLS] = { 
                    frame[x][3], frame[x][2],
                    frame[x][1], frame[x][0] 
                };

                if (state==1) merge(tile,COLS);
                else          slide(tile,COLS);

                frame[x][3]=tile[0];
                frame[x][2]=tile[1];
                frame[x][1]=tile[2];
                frame[x][0]=tile[3];
            }
            break;

            case RIGHT:
            for ( int y=0; y<ROWS; y++ ) {
                unsigned tile[ROWS] = { 
                    frame[3][y], frame[2][y],
                    frame[1][y], frame[0][y] 
                };

                if (state==1) merge(tile,COLS);
                else          slide(tile,COLS);

                frame[3][y]=tile[0];
                frame[2][y]=tile[1];
                frame[1][y]=tile[2];
                frame[0][y]=tile[3];
            }
            break;

            case LEFT:
            for ( int y=0; y<ROWS; y++ ) {
                unsigned tile[ROWS] = { 
                    frame[0][y], frame[1][y],
                    frame[2][y], frame[3][y]
                };

                if (state==1) merge(tile,COLS);
                else          slide(tile,COLS);

                frame[0][y]=tile[0];
                frame[1][y]=tile[1];
                frame[2][y]=tile[2];
                frame[3][y]=tile[3];
            }
            break;
        }

        usleep(61*1000);

        memcpy(board, frame, sizeof(board));
        this->Print();

    } while ( ++state<3 );

    if (!memcmp(board, save, sizeof(board)))
        return false;

    this->Update();
    return true;
}

void Game::Print()
{
    puts("\033[?25h\033[0m\033[H\033[2J");
    for ( int y=0; y<ROWS; y++ ) {
    for ( int x=0; x<COLS; x++ )
#if 0
        { printf("%3d ", board[x][y]); }
#else
        { if (!board[x][y]) printf("    . "); else printf("%5d ", board[x][y]); }
#endif
    puts("\n\r");
    }
    //puts("\r");
}

// TODO Split this up
bool Game::Update()
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

bool Game::GameOver(void)
{
    if (!this->Peek())
        return true;
    return false;
}

bool Game::Peek(void)
{
    // Look Up
    for ( int x=0; x<COLS; x++ ) {
        unsigned tile[COLS] = { 
            board[x][3], board[x][2],
            board[x][1], board[x][0] 
        };

        slide(tile,COLS);
        merge(tile,COLS);

        if (board[x][3]!=tile[0] or board[x][2]!=tile[1] or 
            board[x][1]!=tile[2] or board[x][0]!=tile[3])
            return true;
    }

    // Look Left 
    for ( int y=0; y<ROWS; y++ ) {
        unsigned tile[ROWS] = { 
            board[0][y], board[1][y],
            board[2][y], board[3][y]
        };

        slide(tile,ROWS);
        merge(tile,ROWS);

        if (board[0][y]!=tile[0] or board[1][y]!=tile[1] or 
            board[2][y]!=tile[2] or board[3][y]!=tile[3])
            return true;
    }

    return true;
}
