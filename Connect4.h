#ifndef CONNECT4_H
#define CONNECT4_H
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#define ROWS 6
#define COLS 7


void startGame();
void setupBoard(char board[ROWS][COLS]);
void printBoard(char board[ROWS][COLS]);


#endif
