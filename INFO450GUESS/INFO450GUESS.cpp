
#include "stdafx.h"
#include <iostream>
#include <time.h>

using namespace std;

const int ROWS = 8;
const int COLS = 8;

void giveInstructions();
void createBoard(char gameBoard[ROWS][COLS], char trueBoard[ROWS][COLS]);
void placeItems(char trueBoard[ROWS][COLS]);
void printBoard(char showBoard[ROWS][COLS]);
void choosePoints(char trueBoard[ROWS][COLS], int& score);
void afterGame(char trueBoard[ROWS][COLS], int score);

int main()
{
	//Initializing the gameboard. Take note: gameBoard is just a grid of X's displayed to the user. trueBoard contains the
	//values used to figure out if there's a bomb, gold, or if the player chooses a spot they've already picked.
	int score = 0;
	bool play = true;
	char gameBoard[ROWS][COLS];
	char trueBoard[ROWS][COLS];
	//We start by displaying the game's instructions. This is it's own method so that the player can ask to see them again
	//if they want to keep playing after a game.
	giveInstructions();
	//Finally we get to the main loop of the game. Every time the game is started, a fresh gameBoard and trueBoard are populated,
	//then the gameBoard is displayed. The player goes through a loop of choosing coordinates until they hit a bomb or run
	//out of guesses (thus, choosePoints()). After that, the trueBoard is revealed, points are shown, and the user is asked if they want
	//to play again. They can also ask to see the instructions before they start a new game. Otherwise, if they choose not to play,
	//the program ends.
	do
	{
		char playAgain;
		createBoard(gameBoard, trueBoard);
		placeItems(trueBoard);
		printBoard(gameBoard);
		cout << endl << endl;
		choosePoints(trueBoard, score);
		afterGame(trueBoard, score);
		score = 0;
		cout << endl << "Do you wanna play again? Y / N" << endl;
		cin >> playAgain;
		if (playAgain == 'Y' || playAgain == 'y')
		{
			char repeatInstructions = 'n';
			play = true;
			cout << endl << "Cool! Do you need to hear the instructions again? Y / N" << endl;
			cin >> repeatInstructions;
			if (repeatInstructions == 'Y' || repeatInstructions == 'y')
			{
				giveInstructions();
			}
		}
		else
		{
			play = false;
		}
	} while (play == true);
	return 0;
}

void giveInstructions()
{
	bool proceed = true;
	char repeatRules;
	//A simple loop to give the player the chance to see the instructions multiple times.
	do
	{
		cout << "Noah's Weird Game Thats 100% Luck-Based!" << endl;
		cout << "Welcome to NWGT100L. The rules of the game are simple: " << endl;
		cout << "You are given an 8x8 board. There are 5 pieces of gold and 1 bomb on the board, but you don't know where they are." << endl;
		cout << "You will be asked to enter x, y coordinates corresponding to the board. You start with 5 of these guesses." << endl;
		cout << "If you land on gold, you will gain one point to your score and get an additional chance to guess. Hit a bomb, and it's game over." << endl;
		cout << "Do you want me to repeat the rules? Y or N" << endl;
		cin >> repeatRules;
		if (repeatRules == 'Y' || repeatRules == 'y')
		{
			proceed = false;
			cout << "OK, I'll repeat that for you." << endl;
		}
		else
		{
			proceed = true;
		}
	} while (proceed == false);
}

//Both gameBoard and trueBoard are initially filled with X's.
void createBoard(char gameBoard[ROWS][COLS], char trueBoard[ROWS][COLS])
{
	int r, c;
	for (r = 0; r < ROWS; r++)
	{
		for (c = 0; c < COLS; c++)
		{
			gameBoard[r][c] = 'X';
			trueBoard[r][c] = 'X';
		}
	}
}

//We have another function to place gold and bombs randomly in the trueBoard for simplicity and flexibility.
void placeItems(char trueBoard[ROWS][COLS])
{
	int b = 0;
	int g = 0;
	static int count = 0;
	if (count == 0)
	{
		srand(time(NULL));
	}
	count++;
	do
	{
		int goldR, goldC = 0;
		goldR = rand() % 8 + 1;
		goldC = rand() % 8 + 1;
		if (trueBoard[goldR][goldC] == 'X')
		{
			trueBoard[goldR][goldC] = 'G';
			g++;
		}
	} while (g < 5);
	do
	{
		int bombR, bombC = 0;
		bombR = rand() % 8 + 1;
		bombC = rand() % 8 + 1;
		if (trueBoard[bombR][bombC] == 'X')
		{
			trueBoard[bombR][bombC] = 'B';
			b++;
		}
	} while (b < 1);
}

//This is the key interaction loop where the player chooses coordinates to hopefully win gold.
void choosePoints(char trueBoard[ROWS][COLS], int& score)
{
	int guess = 5;
	do
	{
		int choiceR;
		int choiceC;
		cout << "Decision time! Please enter coordinates as prompted, taking care to use integers between 1 and 8." << endl << endl;
		cout << "Enter x coordinate: ";
		cin >> choiceR;
		cout << "Enter y coordinate: ";
		cin >> choiceC;
		if (choiceR > 8 || choiceC > 8 || choiceR < 1 || choiceC < 1)
		{
			cout << "Oops, you entered an invalid coordinate. Make sure your coordinates are integers between 1 and 8!" << endl << endl;
		}
		else if (trueBoard[choiceR - 1][choiceC - 1] == 'C' || trueBoard[choiceR - 1][choiceC - 1] == 'F')
		{
			cout << "You already chose that point! Try again." << endl << endl;
		}
		else if (trueBoard[choiceR - 1][choiceC - 1] == 'X')
		{
			trueBoard[choiceR - 1][choiceC - 1] = 'C';
			guess--;
			cout << "Sorry, no luck this time." << endl << endl;
		}
		else if (trueBoard[choiceR - 1][choiceC - 1] == 'G')
		{
			trueBoard[choiceR - 1][choiceC - 1] = 'F';
			score++;
			cout << "Nice! You just struck gold. You've gained another guess and a point." << endl << endl;
		}
		else if (trueBoard[choiceR - 1][choiceC - 1] == 'B')
		{
			guess = 0;
			cout << "Whoops! You just hit a bomb. Game over." << endl << endl;;
		}
		cout << "Guesses left: " << guess << endl;
		cout << "Current score: " << score << endl << endl;;
	} while (guess > 0 && score < 5);
}

//This method can print the gameBoard or trueBoard depending on the scenario. It prints the gameBoard at the beginning
//and trueBoard after a game to reveal where the gold and bomb was hidden.
void printBoard(char showBoard[ROWS][COLS])
{
	int r, c;
	cout << "\t";
	for (int i = 1; i < (COLS + 1); i++)
	{
		cout << i << "\t";
	}
	cout << endl;
	for (r = 0; r < ROWS; r++)
	{
		cout << r + 1 << "\t";
		for (c = 0; c < COLS; c++)
		{
			cout << showBoard[r][c] << "\t";
		}
		cout << endl;
	}
}

//After a user runs out of guesses, this method reveals the trueBoard and the final score.
void afterGame(char trueBoard[ROWS][COLS], int score)
{
	cout << endl << endl;
	cout << "Thanks for playing! Here's your final score, and the gameboard you were playing with. Key:" << endl;
	cout << "F: found gold" << endl;
	cout << "G: unfound gold" << endl;
	cout << "C: spot checked, no gold or bomb" << endl;
	cout << "B: bomb" << endl;
	printBoard(trueBoard);
	cout << endl << "-\t-\t-\t-\t-\t-\t" << endl;
	cout << "YOUR FINAL SCORE IS... " << endl << score;
}
