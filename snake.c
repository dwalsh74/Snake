#include "snake.h"
#include "ai.h"

//passing board into functions doesn't work without pre defining size of expected array
//temp fix by setting them to 11

int boardSize = 11;
int length = 2;
int pathLength = 0;
char direction = up;
int snake[1000][2];
int fruitTime = 0;
int fruitCount = 0;
int fruitIndex[256][2];
//char* board[11][11];

int main()
{
	game();				 
}

void game()
{
	int x = 0;
	int mode = 0;
	boardSize = 11;
	char c[50];
	printf("Welcome to Snake!\nEnter 1 to play the game, 2 to watch an ai play, or 3 to exit the program.\n");
	while (0 == mode)
	{
		gets(c);
		sscanf(c,"%d",&x);
	
		if (1 == x)
		{
			printf("You have chosen to start a new game! The game will begin shortly...\n");
			mode = 1;
		}
		else if (2 == x)
		{
			printf("You have chosen to watch an ai play! The game will begin shortly...\n");
			mode = 2;
		}
		else if (3 == x)
		{
			printf("Thanks for playing!\n");
			//exit(0);
		}
		else
		{
			printf("The number you have entered is invalid.Enter 1 to start the game or 2 to exit the program.\n");
		}
	}

	//create the board and snake
	char* board[boardSize][boardSize];
	for (int i=0;i<boardSize;i++)
	{
		for (int j=0;j<boardSize;j++)
		{
			//board[i][j] = empty;
			board[i][j] = empty;
			
			for (int k=0;k<2;k++)
			{
				snake[(10*i)+j][k] = 0;
			}
				
		}
	}
	printf("Use the wasd keys to control the snake's movement.\n");
	//starting position
	snake[0][0] = 5;
	snake[0][1] = 5;
	snake[1][0] = 6;
	snake[1][1] = 5;
	board[5][5] = head;
	board[6][5] = body;
	
	//base game
	switch(mode)
	{
		case 1:
			//place the first fruit
			fruitCheck(board);
			while(1)
			{
				showBoard(board);

				//check for user input and change direction if valid
				char ch;
				//ch = getchar();
				gets(c);
				sscanf(c,"%c",&ch);
				if (ch == up || ch == down || ch == left || ch == right)
				{
					// prevent 180 degree turns
					switch(direction)
					{
						case up:
							if (ch != down)
							{
								direction = ch;
							}
							break;
						case left:
							if (ch != right)
							{
								direction = ch;
							}
							break;
						case down:
							if (ch != up)
							{
								direction = ch;
							}
							break;
						case right:
							if (ch != left)
							{
								direction = ch;
							}
							break;
					}
				}
				snakeMove(board);
				fruitCheck(board);
			}
		case 2:
			fruitCheck(board);
			char aiPath[256];
			pathLength = 0;
			while(1)
			{
				showBoard(board);
				//short delay
				//Sleep(100);
				if (pathLength > 0)
				{
					//continue on path
					direction = aiPath[pathLength-1];
					pathLength--;
					//do the next turn
					snakeMove(board);
					//check if there are enough fruit and add one if not
					fruitCheck(board);
				}
				else
				{
					findPath(aiPath,board);
				}
			}
	}
}

char* snakeMove(char* board[][11])
{
	//move all body parts of the snake forward
	for (int i=length;i>0;i--)
	{
		snake[i][0] = snake[i-1][0];
		snake[i][1] = snake[i-1][1];
	}
	//change the position of the head based on the current direction
	switch(direction)
	{
		case up:
			snake[0][0]--;
			break;
		case left:
			snake[0][1]--;
			break;
		case down:
			snake[0][0]++;
			break;
		case right:
			snake[0][1]++;
			break;
	}
	//check for collission
	if (0 > snake[0][0] || 0 > snake[0][1] || boardSize <= snake[0][0] || boardSize <= snake[0][1] || board[snake[0][0]][snake[0][1]] == body)
	{
		printf ("Game Over!\n\n");
		game();
	}
	
	//check if the snake ate a fruit
	if (board[snake[0][0]][snake[0][1]] == fruit)
	{
		length++;
		for (int i = closestFruit;i < fruitCount;i++)
		{
			fruitIndex[i][0] = fruitIndex[i+1][0];
			fruitIndex[i][1] = fruitIndex[i+1][1];
		}
		fruitCount--;
		//***need to shift elements in fruit index so no spaces between them
	}

	//place the snakes head
	board[snake[0][0]][snake[0][1]] = head;
	
	//place the snakes body
	for (int i=length;i>0;i--)
	{
		board[snake[i][0]][snake[i][1]] = body;
	}
	//clear the end of the snake
	board[snake[length][0]][snake[length][1]] = empty;

	return **board;
}

char* fruitCheck(char* board[][11])
{
	if (1 > fruitCount || fruitTime >= 10) //***needs to prevent self from placing fruit on snakes body
	{
		int newFruit = 0;
		while (0 == newFruit)
		{
			//generate numbers from the clock time
			//time_t t = (clock()/10);
			//clock_t ct = (clock());
            int t = rand();
            int ct = rand();
			//reduce the numbers to a size less then boardSize
			while (t>=boardSize)
			{
				t = t%boardSize;
			}
			while (ct>=boardSize)
			{
				ct = ct%boardSize;
			}
		
			//Place a fruit and store its location
			if (board[t][ct] == empty)
			{
				fruitCount++;  
				board[t][ct] = fruit;
				fruitIndex[fruitCount-1][0] = t;
				fruitIndex[fruitCount-1][1] = ct;
				newFruit = 1;
			
			}
			//reset fruitTime
			fruitTime = 0;
		}
	}
	else
	{
		fruitTime++;
	}
	return **board;
}
							


//display the board
void showBoard(char* board[][11]) //***doesn't like variable array size as input
{
	for (int i=0;i<boardSize;i++)
	{
		for (int j=0;j<boardSize;j++)
		{
			printf("%s",board[i][j]);
		}
		printf("\n");
	}
	printf("\n");
} 



