#include "Connect4.h"
#include <string.h>

// copy this to quickly compile in terminal
// gcc -Wall -Werror *c -o connect

void startMenu() {
    char playAgain = 'Y';

    while (toupper(playAgain) == 'Y') {
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
            fflush(stdout);

            if (fgets(input, sizeof(input), stdin)) {
                Gamemode = toupper(input[0]);

                if (Gamemode == 'P') {
                    chooseMode = true;
                    printf("Starting Player vs Player game...\n");
                    sleep(2);
                    system("clear");
                    startGamePvP();
                } else if (Gamemode == 'A') {
                    printf("AI mode coming soon! Select P for now.\n");
                    sleep(1);
                }
            }
        }

        printf("\nPlay again? (Y): ");
        fflush(stdout);
        if (fgets(input, sizeof(input), stdin)) {
            playAgain = toupper(input[0]);
        }
    }

    printf("\nThanks for playing Connect 4!\n\n");
}

void startGamePvP() {
    char board[ROWS][COLS];
    setupBoard(board);
    char player = 'A';
    bool gameOver = false;
    char input[10];

    printf("Welcome to Connect Four!\n");

    int numMoves = 0;
    int rowPlaced;

    while (!gameOver) {
        printBoard(board);

        int colChosen;
        printf("Player %c choose a column (1-7): ", player);
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin)) {
            if (sscanf(input, "%d", &colChosen) != 1) {
                printf("Invalid input! Please enter a number.\n");
                fflush(stdout);
                continue;
            }
        }

        bool valid = checkChoice(colChosen, board);
        while (!valid) {
            printf("Invalid choice, try again: ");
            fflush(stdout);

            if (fgets(input, sizeof(input), stdin)) {
                if (sscanf(input, "%d", &colChosen) != 1) {
                    printf("Please enter a valid number: ");
                    fflush(stdout);
                    continue;
                }
            }

            valid = checkChoice(colChosen, board);
        }

        rowPlaced = makeMove(colChosen, player, board);
        numMoves++;

        if (numMoves >= 7 &&
            checkWin(player, board, rowPlaced, colChosen - 1)) {
            printBoard(board);
            printf("\nPlayer %c wins!\n", player);
            fflush(stdout);
            gameOver = true;
        } else if (BoardFull(board)) {
            printBoard(board);
            printf("It's a draw!\n");
            fflush(stdout);
            gameOver = true;
        } else {
            player = switchPlayer(player);
        }
    }
}
