/*
 * GameBoard.cpp
 *
 *  Created on: May 15, 2014
 *      Author: kmccormick
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "GameBoard.h"
using namespace std;
namespace Game {

GameBoard::GameBoard() {
	// TODO Auto-generated constructor stub
	int i=0;
	for (i=0;i<16;i++)
	{
		board[i]=0;
	}
	over=false;
	this->setRandCell();
	this->setRandCell();

}
bool GameBoard::move(int direction)
{
	int i=0;
	int j=0;
	bool move=false;
		switch(direction)
		{
		case 8:
			//move up
			//First pass To move tiles next to each other
			for (i=0;i<4;i++)//Column
			{
				for(j=0;j<4;j++)//rows
				{
					if(board[j*4+i]==0)//If elemnet is 0
					{
						int k=j;
						for(k=j;k<4;k++) //Check for non 0 element in column
						{
							if(board[k*4+i]!=0) //Sweet swap and continue to next row
							{
								board[j*4+i]=board[k*4+i];
								board[k*4+i]=0;
								move=true;
								j++;
								continue;
							}
						}
					}
				}
			}
			//Second pass to merge matching tiles
			for (i=0;i<4;i++)//Column
			{
				for(j=0;j<4;j++)//rows
				{
					int k= j+1;
					if((k<4)&&(board[j*4+i]==board[k*4+i]))//If elemnet is 0
					{
						board[j*4+i]*=2;
						board[k*4+i]=0;
						move=true;
					}
				}
			}
			//Third pass re align after merge
			for (i=0;i<4;i++)//Column
			{
				for(j=0;j<4;j++)//rows
				{
					if(board[j*4+i]==0)//If elemnet is 0
					{
						int k=j;
						for(k=j;k<4;k++) //Check for non 0 element in column
						{
							if(board[k*4+i]!=0) //Sweet swap and continue to next row
							{
								board[j*4+i]=board[k*4+i];
								board[k*4+i]=0;
								move=true;
								j++;
								continue;
							}
						}
					}
				}
			}
			this->setRandCell();
			break;
		case 6:
			//move right
			for (j=3;j>=0;j--)//rows
			{
				for(i=3;i>=0;i--)//columns
				{
					if(board[j*4+i]==0)//If elemnet is 0
					{
						int k=i;
						for(k=i;k>=0;k--) //Check for non 0 element in column
						{
							if(board[j*4+k]!=0) //Sweet swap and continue to next row
							{
								board[j*4+i]=board[j*4+k];
								board[j*4+k]=0;
								move=true;
								i--;
								continue;
							}
						}
					}
				}
			}
			//Second pass to merge matching tiles
			for (j=3;j>=0;j--)//Column
			{
				for(i=3;i>=0;i--)//rows
				{
					int k= i-1;
					if((k<4)&&(board[j*4+i]==board[j*4+k]))//If elemnet is 0
					{
						board[j*4+i]*=2;
						board[j*4+k]=0;
						move=true;
					}
				}
			}
			//Third pass re align after merge
			for (j=3;j>=0;j--)//Column
			{
				for(i=3;i>=0;i--)//rows
				{
					if(board[j*4+i]==0)//If elemnet is 0
					{
						int k=i;
						for(k=i;k>=0;k--) //Check for non 0 element in column
						{
							if(board[j*4+k]!=0) //Sweet swap and continue to next row
							{
								board[j*4+i]=board[k*4+k];
								board[j*4+k]=0;
								move=true;
								i--;
								continue;
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
			for (i=3;i>=0;i--)//Column
			{
				for(j=3;j>=0;j--)//rows
				{
					if(board[j*4+i]==0)//If elemnet is 0
					{
						int k=j;
						for(k=j;k>=0;k--) //Check for non 0 element in column
						{
							if(board[k*4+i]!=0) //Sweet swap and continue to next row
							{
								board[j*4+i]=board[k*4+i];
								board[k*4+i]=0;
								move=true;
								j--;
								continue;
							}
						}
					}
				}
			}
			//Second pass to merge matching tiles
			for (i=3;i>=0;i--)//Column
			{
				for(j=3;j>=0;j--)//rows
				{
					int k= j-1;
					if((k<4)&&(board[j*4+i]==board[k*4+i]))//If elemnet is 0
					{
						board[j*4+i]*=2;
						board[k*4+i]=0;
						move=true;
					}
				}
			}
			//Third pass re align after merge
			for (i=3;i>=0;i--)//Column
			{
				for(j=3;j>=0;j--)//rows
				{
					if(board[j*4+i]==0)//If elemnet is 0
					{
						int k=j;
						for(k=j;k>=0;k--) //Check for non 0 element in column
						{
							if(board[k*4+i]!=0) //Sweet swap and continue to next row
							{
								board[j*4+i]=board[k*4+i];
								board[k*4+i]=0;
								move=true;
								j--;
								continue;
							}
						}
					}
				}
			}
			this->setRandCell();
			break;
		case 4:
			//move left
						for (j=0;j<4;j++)//Column
			{
				for(i=0;i<4;i++)//rows
				{
					if(board[j*4+i]==0)//If elemnet is 0
					{
						int k=i;
						for(k=i;k<4;k++) //Check for non 0 element in column
						{
							if(board[j*4+k]!=0) //Sweet swap and continue to next row
							{
								board[j*4+i]=board[j*4+k];
								board[j*4+k]=0;
								move=true;
								i++;
								continue;
							}
						}
					}
				}
			}
			//Second pass to merge matching tiles
			for (j=0;j<4;j++)//Column
			{
				for(i=0;i<4;i++)//rows
				{
					int k= i+1;
					if((k<4)&&(board[j*4+i]==board[j*4+k]))//If elemnet is 0
					{
						board[j*4+i]*=2;
						board[j*4+k]=0;
						move=true;
					}
				}
			}
			//Third pass re align after merge
			for (j=0;j<4;j++)//rows
			{
				for(i=0;i<4;i++)//columns
				{
					if(board[j*4+i]==0)//If elemnet is 0
					{
						int k=i;
						for(k=i;k<4;k++) //Check for non 0 element in column
						{
							if(board[j*4+k]!=0) //Sweet swap and continue to next row
							{
								board[j*4+i]=board[k*4+k];
								board[j*4+k]=0;
								move=true;
								i++;
								continue;
							}
						}
					}
				}
			}
			this->setRandCell();
			break;
		default :
			return false;
		}
	return move;
}
void GameBoard::printBoard()
{
	int i=0;
	printf("\n");
	for (i=0;i<16;i++)
	{
		if((i%4)==0)
		{
			printf("\n");
		}
		printf("%d ",board[i]);
	}
}

bool GameBoard::setRandCell()
{
	int i=0;
	bool full=true;
	for(i=0;i<16;i++)
	{
		if(board[i]==0)
		{
			full=false;
		}
	}
	if(!full)
	{
		i=rand()%16;
		srand(time(NULL));
		while(board[i]!=0)
		{
			i=rand()%16;
		}
		int value=rand()%9;
		if(value==4)
		{
			board[i]=4;
		}else{
			board[i]=2;
		}
		return true;
	}
	return false;
}
bool GameBoard::gameOver()
{
	int i;
	for(i=2;i<10;i++)
	{
		if(this->peek(i))
		{
			return true;
		}
	}
	return false;
}
bool GameBoard::peek(int direction)
{
	int i=0;
	int j=0;
	bool move=false;
			switch(direction)
			{
			case 8:
				//move up
				//First pass To move tiles next to each other
				for (i=0;i<4;i++)//Column
				{
					for(j=0;j<4;j++)//rows
					{
						if(board[j*4+i]==0)//If elemnet is 0
						{
							int k=j;
							for(k=j;k<4;k++) //Check for non 0 element in column
							{
								if(board[k*4+i]!=0) //Sweet swap and continue to next row
								{
									move=true;
									j++;
									continue;
								}
							}
						}
					}
				}
				//Second pass to merge matching tiles
				for (i=0;i<4;i++)//Column
				{
					for(j=0;j<4;j++)//rows
					{
						int k= j+1;
						if((k<4)&&(board[j*4+i]==board[k*4+i]))//If elemnet is 0
						{
							move=true;
						}
					}
				}
				//Third pass re align after merge
				for (i=0;i<4;i++)//Column
				{
					for(j=0;j<4;j++)//rows
					{
						if(board[j*4+i]==0)//If elemnet is 0
						{
							int k=j;
							for(k=j;k<4;k++) //Check for non 0 element in column
							{
								if(board[k*4+i]!=0) //Sweet swap and continue to next row
								{
									move=true;
									j++;
									continue;
								}
							}
						}
					}
				}
				break;
			case 6:
				//move right
				for (j=3;j>=0;j--)//rows
				{
					for(i=3;i>=0;i--)//columns
					{
						if(board[j*4+i]==0)//If elemnet is 0
						{
							int k=i;
							for(k=i;k>=0;k--) //Check for non 0 element in column
							{
								if(board[j*4+k]!=0) //Sweet swap and continue to next row
								{
									move=true;
									i--;
									continue;
								}
							}
						}
					}
				}
				//Second pass to merge matching tiles
				for (j=3;j>=0;j--)//Column
				{
					for(i=3;i>=0;i--)//rows
					{
						int k= i-1;
						if((k<4)&&(board[j*4+i]==board[j*4+k]))//If elemnet is 0
						{
							move=true;
						}
					}
				}
				//Third pass re align after merge
				for (j=3;j>=0;j--)//Column
				{
					for(i=3;i>=0;i--)//rows
					{
						if(board[j*4+i]==0)//If elemnet is 0
						{
							int k=i;
							for(k=i;k>=0;k--) //Check for non 0 element in column
							{
								if(board[j*4+k]!=0) //Sweet swap and continue to next row
								{
									move=true;
									i--;
									continue;
								}
							}
						}
					}
				}
				break;
			case 2:
				//move down
							//First pass To move tiles next to each other
				for (i=3;i>=0;i--)//Column
				{
					for(j=3;j>=0;j--)//rows
					{
						if(board[j*4+i]==0)//If elemnet is 0
						{
							int k=j;
							for(k=j;k>=0;k--) //Check for non 0 element in column
							{
								if(board[k*4+i]!=0) //Sweet swap and continue to next row
								{
									move=true;
									j--;
									continue;
								}
							}
						}
					}
				}
				//Second pass to merge matching tiles
				for (i=3;i>=0;i--)//Column
				{
					for(j=3;j>=0;j--)//rows
					{
						int k= j-1;
						if((k<4)&&(board[j*4+i]==board[k*4+i]))//If elemnet is 0
						{
							move=true;
						}
					}
				}
				//Third pass re align after merge
				for (i=3;i>=0;i--)//Column
				{
					for(j=3;j>=0;j--)//rows
					{
						if(board[j*4+i]==0)//If elemnet is 0
						{
							int k=j;
							for(k=j;k>=0;k--) //Check for non 0 element in column
							{
								if(board[k*4+i]!=0) //Sweet swap and continue to next row
								{
									move=true;
									j--;
									continue;
								}
							}
						}
					}
				}
				break;
			case 4:
				//move left
							for (j=0;j<4;j++)//Column
				{
					for(i=0;i<4;i++)//rows
					{
						if(board[j*4+i]==0)//If elemnet is 0
						{
							int k=i;
							for(k=i;k<4;k++) //Check for non 0 element in column
							{
								if(board[j*4+k]!=0) //Sweet swap and continue to next row
								{
									move=true;
									i++;
									continue;
								}
							}
						}
					}
				}
				//Second pass to merge matching tiles
				for (j=0;j<4;j++)//Column
				{
					for(i=0;i<4;i++)//rows
					{
						int k= i+1;
						if((k<4)&&(board[j*4+i]==board[j*4+k]))//If elemnet is 0
						{
							move=true;
						}
					}
				}
				//Third pass re align after merge
				for (j=0;j<4;j++)//rows
				{
					for(i=0;i<4;i++)//columns
					{
						if(board[j*4+i]==0)//If elemnet is 0
						{
							int k=i;
							for(k=i;k<4;k++) //Check for non 0 element in column
							{
								if(board[j*4+k]!=0) //Sweet swap and continue to next row
								{
									move=true;
									i++;
									continue;
								}
							}
						}
					}
				}
				break;
			default :
			return false;
		}
	return move;
}

GameBoard::~GameBoard() {
	// TODO Auto-generated destructor stub
}

} /* namespace Game */

