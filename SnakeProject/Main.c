// Snake Game
// Name: Oz Hayun
// ID: 209368695
// Date: 09/01/2021

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <stdbool.h>

#define UP 72  // Up Arrow 
#define DOWN 80 // Down Arrow 
#define LEFT 75  // Left Arrow 
#define RIGHT 77  // Right Arrow 
#define ESC 27  // Escape Key
#define SIZE 20 // Size of array
#define FIRSTX 37 // First x 
#define FIRSTY 12 // First Y


//Struct 
typedef struct cordinats
{
	int x;
	int y;
}Cord;

// Functions definitions
void gotoxy(int y, int x);
int getKey();
int menu();
void printMenu();
void initGame(Cord snake[], int snakeLength, Cord* snakeHead, Cord* foodCord);
void printBoard();
void snakeInit(Cord* snakeHead, Cord snake[]);
Cord plantFood(Cord snake[], int snakeLength);
bool ifValidFoodCord(Cord food, Cord snake[], int snakeLength);
bool ifEat(Cord snakeHead, Cord* foodCord);
bool hitHimSelf(Cord  snake[], Cord snakeHead, int snakeLength);
bool hitBorders(Cord snakeHead, int snakeLength);
bool ifLose(Cord  snake[], Cord snakeHead, int snakeLength);
void eatHandle(Cord snake[], int* snakeLength, int* countEat, Cord* foodCord);
void removeTail(Cord snake[], int* snakeLength);
void snakeMove(Cord snake[], Cord snakeHead, Cord* foodCord, int* countEat, int* snakeLength);
void updateSnakeCords(Cord snake[], int* snakeLength, Cord snakeHead);
void displaySnake(Cord snake[], int* snakeLength);
void lose();
void win();


// Snake Game
void main()
{
	char userKey;
	int userChoice, snakeLength = 1, countEat = 0;
	Cord snake[SIZE], snakeHead, foodCord;
	bool b_firstRun = true;

	// Start basic level of the game or exit
	if (menu() == 0)
		exit(0);
		
	// This function init the game- print the board, plant food and init start point
	initGame(snake, snakeLength, &snakeHead, &foodCord);

	// Get keyboard action from user
	userKey = getKey();

	// Start from the middle of the screen
	gotoxy(snakeHead.x, snakeHead.y);
	printf("@");

	while (userKey != ESC)
	{
		// Sleeps for half a second before continuing to the next command
		Sleep(1000);

		// Only if a key was hit we read what key code it was
		if (_kbhit()) // if any key was hit
			userKey = getKey();  // change direction

		// Make a move in current direction if no change
		switch (userKey)
		{
		case RIGHT:
			snakeHead.x++;
			break;
		case DOWN:
			snakeHead.y++;
			break;
		case LEFT:
			snakeHead.x--;
			break;
		case UP:
			snakeHead.y--;
			break;
		}

		// If the snake eat increase the length, count eat and plant new food
		if (ifEat(snakeHead, &foodCord))
			eatHandle(snake, &snakeLength, &countEat, &foodCord);
	
		// Check if the lose
		else
			if (ifLose(snake, snakeHead, snakeLength))
				lose();

		// Update snake cordinates and display the snake in the new position
		snakeMove(snake, snakeHead, &foodCord, &countEat, &snakeLength);
	}

	// The user click ESC, start the game again
	system("cls");
	main();
}


//-------------------------------------------------------------------------------------------------------------
// This function moves the cursor to the line y and column x.
// (0,0) is the upper left corner of the screen.
void gotoxy(int y, int x) {
	printf("\x1b[%d;%df", x + 1, y + 1);
}


//-------------------------------------------------------------------------------------------------------------
// This function gets keyCodes for special keys such as arrows , escape and others
// It returns the Key code such as 72 for arrow up, 27 for Escape... 
int getKey()
{
	char KeyStroke = _getch();
	if (KeyStroke == 0 || KeyStroke == -32)
		KeyStroke = _getch();
	return (KeyStroke);
}


//-------------------------------------------------------------------------------------------------------------
// This function print the menu and get a choice from the user
// The function return int for handle Exit when the user type 0.
int menu()
{
	int userChoice;

	// Print menu of the game and get user choice of level
	printMenu();
	scanf("%d", &userChoice);

	// In this part of the project, only basic level avaliable
	while ((userChoice != 1) && (userChoice != 0))
	{
		printMenu();
		scanf("%d", &userChoice);
	}
	return userChoice;
}


//-------------------------------------------------------------------------------------------------------------
// This function print the menu of the game
void printMenu()
{
	printf("**************************************\n");
	printf("* Pick your level, to exit press 0\n"
		"* 0 - Exit\n"
		"* 1 - Basic Level\n"
		"* 2 - Intermidiate Level\n"
		"* 3 - Advanced Level\n");
	printf("**************************************\n");
}


//-------------------------------------------------------------------------------------------------------------
// This function init the game- print the board, plany food the init snake first cordinates
void initGame(Cord snake[], int snakeLength, Cord* snakeHead, Cord* foodCord)
{
	// Clean the board
	system("cls");

	// Print the board
	printBoard();

	// Using srand function to change the rand numbers every new game
	srand(time(NULL));

	// Plant the food
	*foodCord = plantFood(snake, snakeLength);

	// Init snake Head, tail and first cordinates
	snakeInit(snakeHead, snake);
}


//-------------------------------------------------------------------------------------------------------------
// This function print the board borders
void printBoard()
{

	// Print lines
	for (int ix = 0; ix < 75; ix++)
	{
		// Print first line
		gotoxy(ix, 0);
		printf("#");

		// Print last line
		gotoxy(ix, 25);
		printf("#");
	}

	// Print columns
	for (int iy = 0; iy <= 25; iy++)
	{
		// Print first column
		gotoxy(0, iy);
		printf("#");

		// Print last column
		gotoxy(75, iy);
		printf("#");
	}
}


//-------------------------------------------------------------------------------------------------------------
// This function define first cordinates and init snake cells
void snakeInit(Cord* snakeHead, Cord snake[])
{

	// Init snake cells
	for (int i = 0; i < SIZE; i++)
	{
		snake[i].x = -2;
		snake[i].y = -2;
	}

	// Start Points
	snakeHead->x = FIRSTX;
	snakeHead->y = FIRSTY;
	snake[0].x = snakeHead->x;
	snake[0].y = snakeHead->y;
}


//-------------------------------------------------------------------------------------------------------------
// This function plant random food in the board
// The function return the cordinates of the new food location
Cord plantFood(Cord snake[], int snakeLength)
{
	Cord food;

	// The random function get random food.x between 0-74 and food.y between 0-24
	// because 75 and 25 are the borders
	food.x = (rand() % 73) + 1;
	food.y = (rand() % 23) + 1;


	// If the cordinate is on border or snake, get new cordinate for food
	while (!ifValidFoodCord(food, snake, snakeLength))
	{
		food.x = (rand() % 73) + 1;
		food.y = (rand() % 23) + 1;
	}

	// Print the food
	gotoxy(food.x, food.y);
	printf("$");

	return food;
}


//-------------------------------------------------------------------------------------------------------------
// This function check if rand food cordinate is valid
// The function return bool value to check if the new food cordinates are valid or not
bool ifValidFoodCord(Cord food, Cord snake[], int snakeLength)
{
	bool b_ValidCord = true;
	int index = 0;

	// Check for every cell of the snake if the food is on him
	while ((b_ValidCord) && (index < snakeLength))
	{

		// Check if one of the cordinates are on the snake or the border
		if (((food.x == snake[index].x) && (food.y == snake[index].y)) ||
			(food.x == 0) || (food.y == 0))
			b_ValidCord = false;
		index++;
	}
	return b_ValidCord;
}


//-------------------------------------------------------------------------------------------------------------
// This function check if the snake is in the same cordinate of the food - check if the snake eat
// The function return bool value to check if the snake eat
bool ifEat(Cord snakeHead, Cord* foodCord)
{
	return ((snakeHead.x == foodCord->x) && (snakeHead.y == foodCord->y));
}


//-------------------------------------------------------------------------------------------------------------
// This function print "game over" when the user lose and start the game again.
void lose()
{
	system("cls");
	gotoxy(0, 0);
	printf("##################################\n"
		"            GAME OVER\n"
		"##################################\n");
	main();
}


//-------------------------------------------------------------------------------------------------------------
// This function print "win" when the user won and start the game again.
void win()
{
	system("cls");
	gotoxy(0, 0);
	printf("##################################\n"
		"            You won!\n"
		"##################################\n");
	main();
}


//-------------------------------------------------------------------------------------------------------------
// This function check if the snake hit him self or hit the borders
bool ifLose(Cord  snake[], Cord snakeHead, int snakeLength)
{
	return hitBorders(snakeHead, snakeLength) || hitHimSelf(snake, snakeHead, snakeLength);
}


//-------------------------------------------------------------------------------------------------------------
// This function check check if the snake hit the borders
// The function return bool to check if the snake hit the borders or not
bool hitBorders(Cord snakeHead, int snakeLength)
{
	 return ((snakeHead.x == 0) || (snakeHead.x == 75) || (snakeHead.y == 0) || (snakeHead.y == 25));
}


//-------------------------------------------------------------------------------------------------------------
// This function check if the snake hit him self
bool hitHimSelf(Cord  snake[], Cord snakeHead, int snakeLength)
{
	bool b_hit = false;
	int index = 1;
	while ((!b_hit) && (index < snakeLength))
	{
		if ((snakeHead.x == snake[index].x) && (snakeHead.y == snake[index].y))
			b_hit = true;
		index++;
	}
	return b_hit;
}


//-------------------------------------------------------------------------------------------------------------
// This function update snake array cordinates
void updateSnakeCords(Cord snake[], int* snakeLength, Cord snakeHead)
{
	for (int i = (*snakeLength) - 1; i > 0; i--)
	{
		snake[i].x = snake[i - 1].x;
		snake[i].y = snake[i - 1].y;
	}

	// Update snake[0]
	snake[0].x = snakeHead.x;
	snake[0].y = snakeHead.y;
}


//-------------------------------------------------------------------------------------------------------------
// This function remove tail display when the snake move
void snakeMove(Cord snake[], Cord snakeHead, Cord* foodCord, int* countEat, int* snakeLength)
{
	
	// Increase the snake when < 10
	if (*snakeLength < 10 + *countEat)
		*snakeLength = *snakeLength + 1;

	// Remove the laast @ when moving
	else	
		removeTail(snake, snakeLength);

	// Update the snake cordinates and display the new location
	updateSnakeCords(snake, snakeLength, snakeHead);
	displaySnake(snake, snakeLength);
}


//-------------------------------------------------------------------------------------------------------------
// This function remove the tail in every display
void removeTail(Cord snake[], int* snakeLength)
{
	// Because the snake move 
	gotoxy(snake[*snakeLength - 1].x, snake[*snakeLength - 1].y);
	printf(" ");
}


//-------------------------------------------------------------------------------------------------------------
// This function display the snake
void displaySnake(Cord snake[], int* snakeLength)
{
	gotoxy(snake[0].x, snake[0].y);
	printf("@");

}


//-------------------------------------------------------------------------------------------------------------
// This function handle eat situation- increase snake length, increase count of eat, check win and plant new food
void eatHandle(Cord snake[], int* snakeLength, int* countEat, Cord* foodCord)
{
	(*snakeLength) = *snakeLength + 1;
	(*countEat) = *countEat + 1;
	if (*countEat == 10)
		win();
	*foodCord = plantFood(snake, *snakeLength);
}