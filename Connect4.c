#include "Connect4.h"

void startMenu() {

    char Gamemode = '\0';
    char input[10];
    bool chooseMode = false;

    while (!chooseMode) {
        clearScreen();

        printf("=================================\n");
        printf("    Welcome to Connect 4!\n");
        printf("=================================\n\n");
        printf("Get 4 pieces in a row to win!\n\n");

        printf("Select mode:\nLocal Player vs Player (L) - Player vs AI (A) - Network: As server (S) - Network: As client (C): ");
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin)) {
            Gamemode = toupper(input[0]);

            if (Gamemode == 'L') {
                chooseMode = true;
                printf("Starting Local Player vs Player game...\n");
                sleepSeconds(1.5);
                clearScreen();
                startGamePvP();
            } else if (Gamemode == 'A') {
                chooseMode = true;
                printf("Starting Player vs AI game...\n");
                sleepSeconds(1.5);
                clearScreen();
                selectDifficulty();
            } else if (Gamemode == 'S') {
                chooseMode = true;
                printf("Starting Network Server (you are Player A)...\n");
                sleepSeconds(1.0);
                clearScreen();
                startNetworkServer();
            } 
            else if (Gamemode == 'C') {
                chooseMode = true;
                char ipbuf[64];

                printf("Enter server IP (e.g. 192.168.1.10): ");
                fflush(stdout);

                if (fgets(ipbuf, sizeof(ipbuf), stdin)) {
                    for (int i = 0; ipbuf[i] != '\0'; i++) {
                        if (ipbuf[i] == '\n') {
                            ipbuf[i] = '\0';
                            break;
                        }
                    }
                    startNetworkClient(ipbuf);
                }
            }

             else {
                printf("Invalid choice! Please enter P or A,\n");
                sleepSeconds(1);
            }
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
            checkNInRow(player, board, rowPlaced, colChosen - 1, 4, true)) {
            printBoard(board);
            printf("\nPlayer %c wins!\n", player);
            fflush(stdout);
            gameOver = true;
        } else if (BoardFull(board)) {
            printBoard(board);
            printf("\nIt's a draw!\n");
            fflush(stdout);
            gameOver = true;
        } else {
            player = switchPlayer(player);
        }
    }
}

void startGameAi(Difficulty difficulty) {
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

        if (player == 'A') { // human is A, AI is B
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
        } else {
            printf("\nAI is thinking...\n");
            fflush(stdout);
            sleepSeconds(1.0);

            colChosen = getAIMove(board, difficulty);
            printf("\nAI chooses column %d\n\n", colChosen);
            fflush(stdout);
        }

        rowPlaced = makeMove(colChosen, player, board);
        numMoves++;

        if (numMoves >= 7 &&
            checkNInRow(player, board, rowPlaced, colChosen - 1, 4, true)) {
            printBoard(board);
            if (player == 'A') {
                printf("\nYou win!\n");
            } else {
                printf("\nAI wins!\n");
            }
            fflush(stdout);
            sleepSeconds(1.5);
            gameOver = true;
        } else if (BoardFull(board)) {
            printBoard(board);
            printf("\nIt's a draw!\n");
            fflush(stdout);
            sleepSeconds(1.5);
            gameOver = true;
        } else {
            player = switchPlayer(player);
        }
    }
}
