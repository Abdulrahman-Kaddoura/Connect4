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
    for (int i = 1; i <= COLS; i++) {
        printf("%d ", i);
    }
    printf("\n");
}

bool checkChoice(int col, char board[ROWS][COLS]) {
    int columnIndex = col - 1;

    if (columnIndex < 0 || columnIndex >= COLS) {
        return false;
    }

    if (board[0][columnIndex] != '.') {
        return false;
    }

    return true;
}

int makeMove(int col, char player, char board[ROWS][COLS]) {
    int colIndex = col - 1;
    for (int i = ROWS - 1; i >= 0; i--) {
        if (board[i][colIndex] == '.') {
            board[i][colIndex] = player;
            return i;
        }
    }
    return -1;
}

char switchPlayer(char player) {
    if (player == 'A') {
        return 'B';
    } else {
        return 'A';
    }
}

bool BoardFull(char board[ROWS][COLS]) {
    for (int j = 0; j < COLS; j++) {
        if (board[0][j] == '.') {
            return false;
        }
    }
    return true;
}

bool checkWin(char player, char board[ROWS][COLS], int last_row, int last_col) {
    int r, c;
    int count;

    int directions[4][2] = {
        {0, 1}, // Horizontal
        {1, 0}, // Vertical
        {1, 1}, // Diagonal
        {1, -1} // Diagonal
    };

    for (int d = 0; d < 4; d++) {
        int dr = directions[d][0];
        int dc = directions[d][1];
        count = 1; // Include the last piece itself

        // Check one direction
        r = last_row + dr;
        c = last_col + dc;
        while (r >= 0 && r < ROWS && c >= 0 && c < COLS &&
               board[r][c] == player) {
            count++;
            r += dr;
            c += dc;
        }

        // Check opposite direction
        r = last_row - dr;
        c = last_col - dc;
        while (r >= 0 && r < ROWS && c >= 0 && c < COLS &&
               board[r][c] == player) {
            count++;
            r -= dr;
            c -= dc;
        }

        if (count >= 4) {
            return true; // Win found
        }
    }

    return false; // No win
}
