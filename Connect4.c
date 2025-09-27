#include "Connect4.h"
#include <string.h>

// copy this to quickly compile in terminal
// gcc -Wall -Werror *c -o connect4

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
void startMenu() {
    char Gamemode = '\0';
    char input[10];
    bool chooseMode = false;

    while (!chooseMode) {
        system("clear");

        printf("=================================\n");
        printf("    Welcome to Connect 4!\n");
        printf("=================================\n");
        printf("Get 4 pieces in a row to win!\n\n");

        printf("Select mode:\nPlayer vs Player (P) or Player vs AI (A): ");

        if (fgets(input, sizeof(input), stdin)) {
            Gamemode = toupper(input[0]);

            if (Gamemode == 'P') {
                chooseMode = true;
                printf("Starting Player vs Player game...\n");
                sleep(3);
                system("clear");
                startGamePvP();
            }
        }
    }
}

void startGamePvP() {
    char board[ROWS][COLS];
    setupBoard(board);
    printBoard(board);
}
