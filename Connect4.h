#ifndef CONNECT4_H
#define CONNECT4_H
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

#define ROWS 6
#define COLS 7

typedef enum Difficulty {
    EASY = 1,
    MEDIUM,
    HARD
} Difficulty;

void sleepSeconds(double seconds);

void startMenu();
void selectDifficulty();

void startGamePvP();
void startGameAi(Difficulty difficulty);
int getAIMove(char board[ROWS][COLS], Difficulty difficulty);

void setupBoard(char board[ROWS][COLS]);
void printBoard(char board[ROWS][COLS]);
bool BoardFull(char board[ROWS][COLS]);

bool checkChoice(int choice, char board[ROWS][COLS]);
int makeMove(int col, char player, char board[ROWS][COLS]);
char switchPlayer(char player);
bool checkWin(char player, char board[ROWS][COLS], int last_row, int last_col);
#endif
