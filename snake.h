#include <stdio.h>
//#include <ansi_c.h>
//#include <windows.h>
#include <time.h>



//definitions
#define empty "[ ]"
#define fruit "[*]"
#define head "[o]"
#define body "[0]"
#define spread "[x]"
#define up 'w'
#define left 'a'
#define down 's'
#define right 'd'

//global variables
extern int boardSize;
extern int length;
extern int pathLength;
extern char direction;
extern int snake[][2];
extern int fruitTime;
extern int fruitCount;
extern int fruitIndex[256][2];

//controls the game
void game();

//check for a collision. End game if true
int collisionCheck(char* board[][boardSize]); //maybe make this part of snakeMove

//check to see if another fruit should be placed 
char* fruitCheck(char* board[][boardSize]);

//move the snake in the direction last indicated by the player
char* snakeMove(char* board[][boardSize]);

//display the board
void showBoard(char* board[][boardSize]);
  



