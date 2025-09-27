#ifndef CONNECT4_H
#define CONNECT4_H
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>

#define ROWS 6
#define COLS 7


void startMenu();

void startGamePvP();
// void startGameAiEasy();
// void startGameAiMedium();
// void startGameAiHard();

void setupBoard(char board[ROWS][COLS]);
void printBoard(char board[ROWS][COLS]);
bool BoardFull(char board[ROWS][COLS]);

bool checkChoice(int choice, char board[ROWS][COLS]);
void makeMove(int col, char player, char board[ROWS][COLS]);
char switchPlayer(char player);
bool checkWin(char player, char board[ROWS][COLS]);
#endif
