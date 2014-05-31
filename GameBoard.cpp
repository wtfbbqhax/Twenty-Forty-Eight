/*
 * GameBoard.cpp
 *
 *  Created on: May 15, 2014
 *      Author: kmccormick
 */

// wtfbbqhax, 05-31-2014:
//  * Unrolling loops will ease the eye strain
//  * #define MOVE_UP, MOVE_DOWN etc..
//  * Move board rendering to a separate class

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "GameBoard.h"

namespace Game {

Board::Board()
{
    // TODO Auto-generated constructor stub
    for ( int i = 0; i < 16; i++ )
    {
        board[i] = 0;
    }
    this->setRandCell();
    this->setRandCell();
    over = false;
}

// TODO Auto-generated destructor stub
Board::~Board() { }

bool Board::move(int direction)
{
    int i = 0;
    int j = 0;
    bool move = false;

    switch (direction)
    {
        case 8:
            //move up
            //First pass To move tiles next to each other
            for ( i = 0; i < 4; i++ )//Column
            {
                for ( j = 0; j < 4; j++ )//rows
                {
                    if ( board[j*4+i] == 0 )//If elemnet is 0
                    {
                        for ( int k = j; k < 4; k++ ) //Check for non 0 element in column
                        {
                            if ( board[k*4+i] != 0 ) //Sweet swap and continue to next row
                            {
                                board[j*4+i]=board[k*4+i];
                                board[k*4+i]= 0;
                                move = true;
                                j++;
                            }
                        }
                    }
                }
            }

            //Second pass to merge matching tiles
            for ( i = 0; i < 4; i++ )//Column
            {
                for ( j = 0; j < 4; j++ )//rows
                {
                    int k = j+1;
                    if ( (k < 4) && (board[j*4+i] == board[k*4+i]) )//If elemnet is 0
                    {
                        board[j*4+i]*= 2;
                        board[k*4+i]= 0;
                        move = true;
                    }
                }
            }

            //Third pass re align after merge
            for ( i = 0; i < 4; i++ )//Column
            {
                for ( j = 0; j < 4; j++ )//rows
                {
                    if ( board[j*4+i] == 0 )//If elemnet is 0
                    {
                        for ( int k = j; k < 4; k++ ) //Check for non 0 element in column
                        {
                            if ( board[k*4+i] != 0 ) //Sweet swap and continue to next row
                            {
                                board[j*4+i] = board[k*4+i];
                                board[k*4+i] = 0;
                                move = true;
                                j++;
                            }
                        }
                    }
                }
            }

            this->setRandCell();
            break;

        case 6:
            //move right
            for ( j = 3; j >= 0; j-- )//rows
            {
                for ( i = 3; i >= 0; i-- )//columns
                {
                    if ( board[j*4+i]== 0 )//If elemnet is 0
                    {
                        for ( int k = i; k >= 0; k-- ) //Check for non 0 element in column
                        {
                            if ( board[j*4+k] != 0 ) //Sweet swap and continue to next row
                            {
                                board[j*4+i] = board[j*4+k];
                                board[j*4+k] = 0;
                                move = true;
                                i--;
                            }
                        }
                    }
                }
            }

            //Second pass to merge matching tiles
            for ( j = 3; j >= 0; j-- )//Column
            {
                for ( i = 3; i >= 0; i-- )//rows
                {
                    int k =  i-1;
                    if ( (k < 4) && (board[j*4+i] == board[j*4+k]) )//If elemnet is 0
                    {
                        board[j*4+i]*= 2;
                        board[j*4+k]= 0;
                        move = true;
                    }
                }
            }

            //Third pass re align after merge
            for ( j = 3; j >= 0; j-- )//Column
            {
                for ( i = 3; i >= 0; i-- )//rows
                {
                    if ( board[j*4+i] == 0 )//If elemnet is 0
                    {
                        for ( int k = i; k >= 0; k-- ) //Check for non 0 element in column
                        {
                            if ( board[j*4+k] != 0 ) //Sweet swap and continue to next row
                            {
                                board[j*4+i] = board[k*4+k];
                                board[j*4+k] = 0;
                                move = true;
                                i--;
                            }
                        }
                    }
                }
            }

            this->setRandCell();
            break;

        case 2:
            //move down
            //First pass To move tiles next to each other
            for ( i = 3; i >= 0; i-- )//Column
            {
                for ( j = 3; j >= 0; j-- )//rows
                {
                    if ( board[j*4+i] == 0 )//If elemnet is 0
                    {
                        for ( int k = j; k >= 0; k-- ) //Check for non 0 element in column
                        {
                            if ( board[k*4+i] != 0 ) //Sweet swap and continue to next row
                            {
                                board[j*4+i] = board[k*4+i];
                                board[k*4+i] = 0;
                                move = true;
                                j--;
                            }
                        }
                    }
                }
            }

            //Second pass to merge matching tiles
            for ( i = 3; i >= 0; i-- )//Column
            {
                for ( j = 3; j >= 0; j-- )//rows
                {
                    int k =  j-1;
                    if ( (k < 4 )&&(board[j*4+i]==board[k*4+i]))//If elemnet is 0
                    {
                        board[j*4+i]*= 2;
                        board[k*4+i]= 0;
                        move = true;
                    }
                }
            }

            //Third pass re align after merge
            for ( i = 3; i >= 0; i-- )//Column
            {
                for ( j = 3; j >= 0; j-- )//rows
                {
                    if ( board[j*4+i] == 0 )//If elemnet is 0
                    {
                        for ( int k = j; k >= 0; k-- ) //Check for non 0 element in column
                        {
                            if ( board[k*4+i] != 0 ) //Sweet swap and continue to next row
                            {
                                board[j*4+i] = board[k*4+i];
                                board[k*4+i] = 0;
                                move = true;
                                j--;
                            }
                        }
                    }
                }
            }

            this->setRandCell();
            break;

        //move left
        case 4:
            for ( j = 0; j < 4; j++ )//Column
            {
                for ( i = 0; i < 4; i++ )//rows
                {
                    if ( board[j*4+i] == 0 )//If elemnet is 0
                    {
                        for ( int k = i; k < 4; k++ ) //Check for non 0 element in column
                        {
                            if ( board[j*4+k] != 0 ) //Sweet swap and continue to next row
                            {
                                board[j*4+i] = board[j*4+k];
                                board[j*4+k] = 0;
                                move = true;
                                i++;
                                continue;
                            }
                        }
                    }
                }
            }

            //Second pass to merge matching tiles
            for ( j = 0; j < 4; j++ )//Column
            {
                for ( i = 0; i < 4; i++ )//rows
                {
                    int k = i+1;
                    // if element is 0
                    if ( (k < 4) && (board[j*4+i] == board[j*4+k]) )
                    {
                        board[j*4+i] *= 2;
                        board[j*4+k] = 0;
                        move = true;
                    }
                }
            }

            //Third pass re align after merge
            for ( j = 0; j < 4; j++ )//rows
            {
                for ( i = 0; i < 4; i++ )//columns
                {
                    if ( board[j*4+i] == 0 )//If elemnet is 0
                    {
                        for ( int k = i; k < 4; k++ ) //Check for non 0 element in column
                        {
                            if ( board[j*4+k] != 0 ) //Sweet swap and continue to next row
                            {
                                board[j*4+i] = board[k*4+k];
                                board[j*4+k] = 0;
                                move = true;
                                i++;
                            }
                        }
                    }
                }
            }

            this->setRandCell();
            break;

        default:
            return false;
    }

    return move;
}

void Board::printBoard()
{
    for ( int i = 0; i < 16; i++ )
    {
        if ( (i%4) == 0 )
            puts("");

        printf("%d ", board[i]);
    }
}

// TODO Split this up
bool Board::setRandCell()
{
    bool full = true;

    for ( int i = 0; i < 16; i++ )
    {
        if ( board[i] == 0 )
            full = false;
    }

    if ( !full )
    {
        int i = rand() % 16;
        srand(time(NULL));

        while ( board[i] != 0 )
            i = rand() % 16;

        int value = rand() % 9;
        if ( value == 4 )
            board[i] = 4;
        else
            board[i] = 2;

        return true;
    }

    return false;
}

// Looks over complicated
bool Board::gameOver()
{
    for ( int i = 2; i < 10; i++ )
    {
        if ( this->peek(i) )
            return true;
    }
    return false;
}

// Check if there are any moves?
bool Board::peek(int direction)
{
    int i = 0;
    int j = 0;
    bool move = false;
    switch (direction)
    {
        case 8:
            //move up
            //First pass To move tiles next to each other
            for ( i = 0; i < 4; i++ )//Column
            {
                for ( j = 0; j < 4; j++ )//rows
                {
                    if ( board[j*4+i] == 0 )//If elemnet is 0
                    {
                        for ( int k = j; k < 4; k++ ) //Check for non 0 element in column
                        {
                            if ( board[k*4+i] != 0 ) //Sweet swap and continue to next row
                            {
                                move = true;
                                j++;
                            }
                        }
                    }
                }
            }

            //Second pass to merge matching tiles
            for ( i = 0; i < 4; i++ )//Column
            {
                for ( j = 0; j < 4; j++ )//rows
                {
                    int k = j+1;
                    if ( (k < 4) && (board[j*4+i] == board[k*4+i]) )//If elemnet is 0
                    {
                        move = true;
                    }
                }
            }

            //Third pass re align after merge
            for ( i = 0; i < 4; i++ )//Column
            {
                for ( j = 0; j < 4; j++ )//rows
                {
                    if ( board[j*4+i] == 0 )//If elemnet is 0
                    {
                        for ( int k = j; k < 4; k++ ) //Check for non 0 element in column
                        {
                            if ( board[k*4+i] != 0 ) //Sweet swap and continue to next row
                            {
                                move = true;
                                j++;
                            }
                        }
                    }
                }
            }
            break;

        case 6:
            //move right
            for ( j = 3; j >= 0; j-- )//rows
            {
                for ( i = 3; i >= 0; i-- )//columns
                {
                    if ( board[j*4+i] == 0 )//If elemnet is 0
                    {
                        for ( int k = i; k >= 0; k-- ) //Check for non 0 element in column
                        {
                            if ( board[j*4+k] != 0 ) //Sweet swap and continue to next row
                            {
                                move = true;
                                i--;
                            }
                        }
                    }
                }
            }

            //Second pass to merge matching tiles
            for ( j = 3; j >= 0; j-- )//Column
            {
                for ( i = 3; i >= 0; i-- )//rows
                {
                    int k = i-1;
                    if ( (k < 4) && (board[j*4+i] == board[j*4+k]) )//If elemnet is 0
                        move = true;
                }
            }

            //Third pass re align after merge
            for ( j = 3; j >= 0; j-- )//Column
            {
                for ( i = 3; i >= 0; i-- )//rows
                {
                    if ( board[j*4+i]== 0 )//If elemnet is 0
                    {
                        for ( int k = i; k >= 0; k-- ) //Check for non 0 element in column
                        {
                            if ( board[j*4+k]!= 0 ) //Sweet swap and continue to next row
                            {
                                move = true;
                                i--;
                            }
                        }
                    }
                }
            }
            break;

        case 2:
            //move down
            //First pass To move tiles next to each other
            for ( i = 3; i >= 0; i-- )//Column
            {
                for ( j = 3; j >= 0; j-- )//rows
                {
                    if ( board[j*4+i] == 0 )//If elemnet is 0
                    {
                        for ( int k = j; k >= 0; k-- ) //Check for non 0 element in column
                        {
                            if ( board[k*4+i] != 0 ) //Sweet swap and continue to next row
                            {
                                move = true;
                                j--;
                            }
                        }
                    }
                }
            }

            //Second pass to merge matching tiles
            for ( i = 3; i >= 0; i-- )//Column
            {
                for ( j = 3; j >= 0; j-- )//rows
                {
                    int k = j-1;
                    if ( (k < 4) && (board[j*4+i] == board[k*4+i]) )//If elemnet is 0
                        move = true;
                }
            }

            //Third pass re align after merge
            for ( i = 3; i >= 0; i-- )//Column
            {
                for ( j = 3; j >= 0; j-- )//rows
                {
                    if ( board[j*4+i]== 0 )//If elemnet is 0
                    {
                        for ( int k = j; k >= 0; k-- ) //Check for non 0 element in column
                        {
                            if ( board[k*4+i]!= 0 ) //Sweet swap and continue to next row
                            {
                                move = true;
                                j--;
                            }
                        }
                    }
                }
            }
            break;

        case 4:
            //move left
            for ( j = 0; j < 4; j++ )//Column
            {
                for ( i = 0; i < 4; i++ )//rows
                {
                    if ( board[j*4+i] == 0 )//If elemnet is 0
                    {
                        for ( int k = i; k < 4; k++ ) //Check for non 0 element in column
                        {
                            if ( board[j*4+k] != 0 ) //Sweet swap and continue to next row
                            {
                                move = true;
                                i++;
                            }
                        }
                    }
                }
            }

            //Second pass to merge matching tiles
            for ( j = 0; j < 4; j++ )//Column
            {
                for ( i = 0; i < 4; i++ )//rows
                {
                    int k = i+1;
                    if ( (k < 4) && (board[j*4+i] == board[j*4+k]) )//If elemnet is 0
                        move = true;
                }
            }

            //Third pass re align after merge
            for ( j = 0; j < 4; j++ )//rows
            {
                for ( i = 0; i < 4; i++ )//columns
                {
                    if ( board[j*4+i] == 0 )//If elemnet is 0
                    {
                        for ( int k = i; k < 4; k++ ) //Check for non 0 element in column
                        {
                            if ( board[j*4+k] != 0 ) //Sweet swap and continue to next row
                            {
                                move = true;
                                i++;
                            }
                        }
                    }
                }
            }
            break;

        default:
            return false;
    }
    return move;
}


} // namespace Game

