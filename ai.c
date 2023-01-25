//****This code is unfinished****//

#include "snake.h"
#include "ai.h"

int closestFruit = 0;
//*****needs the zig zag
// - attempt to keep body close together
//if no path to fruit, zig zag to group body
//make sure it has a way out
// - have part of function check to make sure it doesn't create spaces it won't have an exit from
// - opening and turn around points must be an even number of spaces wide

//find the best path to a fruit  
extern char* findPath(char aiPath[256], char* board[][11])
{
	closestFruit = 0;
	int x = 0;
	int y = 0;
	int z = 0;
	
	//find the closest fruit
	for (int i=0; i<fruitCount; i++)
	{
		x = abs(snake[0][0]-fruitIndex[i][0]);
		y = abs(snake[0][1]-fruitIndex[i][1]);
		
		if ((x+y) >= z)
		{
			closestFruit = i;
			z = x+y;
		}
	}
	//find clear (hugging) paths, choose shortest
	x = 0;
	y = 0;
	char xDir = up;
	char yDir = left;
	int xMod = 0;
	int yMod = 0;
	//get x distances to fruit. Leave at 0 if coordinates are equal
	//***implement turning mechanic for when it needs to go in the opposite direction
	if (snake[0][0] > fruitIndex[closestFruit][0])
	{
		x = snake[0][0] - fruitIndex[closestFruit][0];
		xDir  = up;
		xMod = (-1);
	}
	else if (snake[0][0] < fruitIndex[closestFruit][0])
	{
		x = fruitIndex[closestFruit][0] - snake[0][0];
		xDir = down;
		xMod = 1;
	}
	
	//get y distances to fruit. Leave at 0 if coordinates are equal
	if (snake[0][1] > fruitIndex[closestFruit][1])
	{
		y = snake[0][1] - fruitIndex[closestFruit][1];
		yDir = left;
		yMod = (-1);
	}
	else if (snake[0][1] < fruitIndex[closestFruit][1])
	{
		y = fruitIndex[closestFruit][1] - snake[0][1];
		yDir = right;
		yMod = 1;
	}
	//Find viable path
	//draw path one block at a time, avoiding blocked squares **only direct routes at the moment
	int xMoved = 0;
	int yMoved = 0;
	int numMoved = 0;
	
	while (x > 0 || y > 0)
	{
		//try moving directly to the fruit
		if (x>0 && (board[snake[0][0] + xMoved + xMod][snake[0][1] + yMoved][1] == empty[1] || board[snake[0][0] + xMoved + xMod][snake[0][1] + yMoved][1] == fruit[1]))
		{
			for (int i = numMoved+1; i>0; i--)
			{
				//shift elements forward in array
				aiPath[i] = aiPath[i-1];
			}
				aiPath[0] = xDir;
				pathLength++;
				numMoved++;
				xMoved = xMoved + xMod;
				x--;
		}
		//try moving on the y plane
		else if (y>0 && (board[snake[0][0] + xMoved][snake[0][1] + yMoved + yMod][1] == empty[1] || board[snake[0][0] + xMoved][snake[0][1] + yMoved + yMod][1] == fruit[1]))
		{
			for (int i = numMoved+1; i>0; i--)
			{
				//shift elements forward in array
				aiPath[i] = aiPath[i-1];
			}
			//enter the next step in the path
			aiPath[0] = yDir;
			pathLength++;
			numMoved++;
			yMoved = yMoved + yMod;
			y--;
		}
		//if can't do either move
		//if (pathLength == 0);
		else
		{
			lightningPath(aiPath,board);
			//exit loop
			x = 0;
			y = 0;
			for (int i=0;i<boardSize;i++)
			{
				for (int j=0;j<boardSize;j++)
				{
					if (board[i][j][1] == 'x')
					{
						board[i][j] = empty;
					}
			
				}
			}
		}
	}
	//find more complex path if no straightforward one is found
	return aiPath;
}

//***purge dead ends and repeat until one remains
extern char* lightningPath(char aiPath[256], char* board[][11])
{
	int numPaths = 1;
	//up = 1, left = 2, down = 3, right = 4
	//first2 blocks holds all streams, second holds contents of each stream and movement counts
	//254 stores total x movement for each stream and 255 stores tottal y
	char paths[256][256];
	//fill the array with zeros
	for (int i = 0; i<256; i++)
	{
		for (int j = 0; j<256; j++)
		{
			paths[i][j] = 0;
		}
	}
	
	//how many moves are in each path
	int pathWidth = 0;
	
	//make 2 copies of each stream
	//add 0, 1 and 2 to each variation
	//purge failures
	//repeat
	
	//loop indefinitely
	while (1)
	{
		//make 4 copies of all variations
		for (int i = 1; i<4;i++)
		{
			//make 4 groups of copies of all the current paths
			for (int j = 0; j<(numPaths);j++)
			{
				for (int k = 0; k<256;k++)
				{ 
					paths[j+(numPaths*i)][k] = paths[j][k];
				}
			}
		}
		//append 1,2,3,4 to each group respectively and check if the move is possible
		int i = 0;
		for (i = 0; i<numPaths*3;i++)
		{
			//***try adding every one to every variation, mark for delete if doesn't work
			//try up
			if (snake[0][0]+paths[i][254]-1 >= 0 && board[snake[0][0]+paths[i][254]-1][snake[0][1]+paths[i][255]][1] == empty[1])
			{
				paths[i][pathWidth] = 1;
				//x movement
				paths[i][254]--;
				board[snake[0][0]+paths[i][254]][snake[0][1]+paths[i][255]] = spread;
																			  
			}
			//check for fruit
			else if (snake[0][0]+paths[i][254]-1 >= 0 && board[snake[0][0]+paths[i][254]-1][snake[0][1]+paths[i][255]][1] == fruit[1])
			{   
				paths[i][pathWidth] = 1;
				//generate an array of directions from this path
				pathLength = pathWidth+1; //not absoutely sure about needing the +1
				for (int j = pathWidth; j>=0;j--)
				{
					//up = 1, left = 2, down = 3, right = 4
					switch(paths[i][pathWidth - j])
					{
						case 1:
							aiPath[j] = up;
							break;
						case 2:
							aiPath[j] = left;
							break;
						case 3:
							aiPath[j] = down;
							break;
						case 4:
							aiPath[j] = right;
							break;
					}
				}
				return aiPath;
			}
			//try down
			else if (snake[0][0]+paths[i][254]+1 < boardSize && board[snake[0][0]+paths[i][254]+1][snake[0][1]+paths[i][255]][1] == empty[1])
			{
				paths[i][pathWidth] = 3;
				//x movement
				paths[i][254]++;
				board[snake[0][0]+paths[i][254]][snake[0][1]+paths[i][255]] = spread;
			}
			//check for fruit
			else if (snake[0][0]+paths[i][254]+1 < boardSize && board[snake[0][0]+paths[i][254]+1][snake[0][1]+paths[i][255]][1] == fruit[1])
			{   
				paths[i][pathWidth] = 3;
				//generate an array of directions from this path
				pathLength = pathWidth+1; //not absoutely sure about needing the +1
				for (int j = pathWidth; j>=0;j--)
				{
					//up = 1, left = 2, down = 3, right = 4
					switch(paths[i][pathWidth - j])
					{
						case 1:
							aiPath[j] = up;
							break;
						case 2:
							aiPath[j] = left;
							break;
						case 3:
							aiPath[j] = down;
							break;
						case 4:
							aiPath[j] = right;
							break;
					}
				}
				return aiPath;
			}
			//try left
			else if (snake[0][1]+paths[i][255]-1 >= 0 && board[snake[0][0]+paths[i][254]][snake[0][1]+paths[i][255]-1][1] == empty[1])
			{
				paths[i][pathWidth] = 2;
				//y movement
				paths[i][255]--;
				board[snake[0][0]+paths[i][254]][snake[0][1]+paths[i][255]] = spread;
			}
			//check for fruit
			else if (snake[0][1]+paths[i][255]-1 >= 0 && board[snake[0][0]+paths[i][254]][snake[0][1]+paths[i][255]-1][1] == fruit[1])
			{   
				paths[i][pathWidth] = 2;
				//generate an array of directions from this path
				pathLength = pathWidth+1; //not absoutely sure about needing the +1
				for (int j = pathWidth; j>=0;j--)
				{
					//up = 1, left = 2, down = 3, right = 4
					switch(paths[i][pathWidth - j])
					{
						case 1:
							aiPath[j] = up;
							break;
						case 2:
							aiPath[j] = left;
							break;
						case 3:
							aiPath[j] = down;
							break;
						case 4:
							aiPath[j] = right;
							break;
					}
				}
				return aiPath;
			}
			//try right
			else if (snake[0][1]+paths[i][255]+1 < boardSize && board[snake[0][0]+paths[i][254]][snake[0][1]+paths[i][255]+1][1] == empty[1])
			{
				paths[i][pathWidth] = 4;
				//y movement
				paths[i][255]++;
				board[snake[0][0]+paths[i][254]][snake[0][1]+paths[i][255]] = spread;
			}
			//check for fruit
			else if (snake[0][1]+paths[i][255]+1 < boardSize && board[snake[0][0]+paths[i][254]][snake[0][1]+paths[i][255]+1][1] == fruit[1])
			{   
				paths[i][pathWidth] = 4;
				//generate an array of directions from this path
				pathLength = pathWidth+1; //not absoutely sure about needing the +1
				for (int j = pathWidth; j>=0;j--)
				{
					//up = 1, left = 2, down = 3, right = 4
					switch(paths[i][pathWidth - j])
					{
						case 1:
							aiPath[j] = up;
							break;
						case 2:
							aiPath[j] = left;
							break;
						case 3:
							aiPath[j] = down;
							break;
						case 4:
							aiPath[j] = right;
							break;
					}
				}
				return aiPath;
			}
			//mark for delete
			else
			{
				paths[i][pathWidth] = 5;
			}
			//showBoard(board);
		}
		//update the number of paths
		numPaths = i;
		
		//check if valid path found and delete all marked groups
		for (i = 0; i<numPaths;i++)
		{
			if (5 == paths[i][pathWidth])
			{   
				
				//shift all paths down
				for (int j = i; j<numPaths; j++)
				{
					//replace contents with those of the one above it
					for (int k = 0; k<256; k++)
					{
						paths[j][255-k] = paths[j+1][255-k];
					}
				}
				numPaths--;
				//keep i the same so a row isn't skipped
				i--;
				
				//if no valid path found, simply return the direction up
				if (numPaths == 0)
				{
					//printf("no path found");
					pathLength = 1;
					aiPath[0] = up;
					return aiPath;
				}
			}
		}
		pathWidth++;
	}
}
	
		
		

			
			
					
				
				
					
						
				
				
				
				
					
			
			
			
				

