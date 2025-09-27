#include "Connect4.h"

void setupBoard(char board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board[i][j] = '.';
        }
    }
}

void printBoard(char board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
    for (int i = 1; i<= COLS; i++) {
        printf("%d ", i);
    }
    printf("\n");
}

bool checkChoice(int col, char board[ROWS][COLS]) {
    int columnIndex = col -1;

    if(columnIndex <0||columnIndex >= COLS){
        return false;
    }

    if(board[0][columnIndex] != '.') {
        return false;
    }

    return true;
}

void makeMove(int col, char player, char board[ROWS][COLS]) {
    int colIndex = col -1;
    for (int i = ROWS- 1; i >= 0; i--) {
        if (board[i][colIndex]== '.') {
            board[i][colIndex]= player;
            break;
        }
    }
}

char switchPlayer(char player) {
    if (player == 'A') {
        return 'B';
    }else {
        return 'A';
    }
}

bool BoardFull(char board[ROWS][COLS]) {
    for (int j =0; j< COLS; j++) {
        if (board[0][j] == '.') {
            return false;  
        }
    }
    return true;
}

bool checkWin(char player, char board[ROWS][COLS]) {
    return false;
}