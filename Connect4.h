#ifndef CONNECT4_H
#define CONNECT4_H

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>
#include <pthread.h>


#define ROWS 6
#define COLS 7

#define INF 1000000
#define WIN_SCORE 100000
#define LOSS_SCORE -100000

#define COLOR_RESET "\033[0m"
#define COLOR_RED "\033[31m"
#define COLOR_YELLOW "\033[33m"

typedef enum Difficulty { EASY = 1, MEDIUM, HARD } Difficulty;

typedef struct {
    char board[ROWS][COLS];
    int col;
    int score;
    int searchDepth;
    char bot;
    char human;
} ThreadData;

void copyBoard(char dest[ROWS][COLS], char src[ROWS][COLS]);
int getAIMoveHard_MT(char board[ROWS][COLS]);
void* evaluateMoveThread(void* arg);

void sleepSeconds(double seconds);
void clearScreen();

void startMenu();
void selectDifficulty();

void startGamePvP();
void startGameAi(Difficulty difficulty);

void startNetworkServer();
void startNetworkClient(char* server_ip);

int getAIMove(char board[ROWS][COLS], Difficulty difficulty);
int getAIMoveEasy(char board[ROWS][COLS]);
int getAIMoveMedium(char board[ROWS][COLS]);
int getAIMoveHard(char board[ROWS][COLS]);

void simulateMove(char board[ROWS][COLS], char tempBoard[ROWS][COLS], int col,
                  char player, int *sim_row);

bool checkChoice(int choice, char board[ROWS][COLS]);
int makeMove(int col, char player, char board[ROWS][COLS]);
char switchPlayer(char player);
bool hasWinner(char board[ROWS][COLS], char player);

void setupBoard(char board[ROWS][COLS]);
void printBoard(char board[ROWS][COLS]);
bool BoardFull(char board[ROWS][COLS]);

bool checkNInRow(char player, char board[ROWS][COLS], int last_row,
                 int last_col, int target, bool doHighlight);



#endif
