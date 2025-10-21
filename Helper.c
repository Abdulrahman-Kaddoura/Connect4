#include "Connect4.h"

void selectDifficulty() {
    char input[10];
    char difficulty = '\0';
    bool diff_selected = false;

    while (!diff_selected) {
        clearScreen();
        printf("Select AI difficulty: \n");
        printf("Easy(E) - Medium(M) - Hard(H): ");
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin)) {
            difficulty = toupper(input[0]);
            if (difficulty == 'E') {
                diff_selected = true;
                printf("Starting game in easy mode...\n\n");
                sleepSeconds(1.5);
                startGameAi(EASY);
            } else if (difficulty == 'M') {
                printf("MEDIUM mode is currently unavailable. Please select "
                       "(E).\n");
                sleepSeconds(1.5);
                // startGameAi(MEDIUM);
            } else if (difficulty == 'H') {
                printf(
                    "HARD mode is currently unavailable. Please select (E).\n");
                sleepSeconds(1.5);
                // startGameAi(HARD);
            } else {
                printf("Invalid input, please enter E, M or H.\n");
                sleepSeconds(1.5);
            }
        }
    }
}

int getAIMove(char board[ROWS][COLS], Difficulty difficulty) {
    if (difficulty == EASY) {
        return getAIMoveEasy(board);
    } else if (difficulty == MEDIUM) {
        return getAIMoveEasy(board); // change when implementing medium bot
    } else {
        return getAIMoveEasy(board); // change when implementing hard bot
    }
}

int getAIMoveEasy(char board[ROWS][COLS]) {
    int col;
    do {
        col = (rand() % COLS) + 1;
    } while (!checkChoice(col, board));
    return col;
}

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
            char piece = board[i][j];
            if (highlight[i][j]) {
                if (piece == 'A') {
                    printf(COLOR_RED "%c " COLOR_RESET, piece);
                } else if (piece == 'B') {
                    printf(COLOR_YELLOW "%c " COLOR_RESET, piece);
                }
            } else {
                printf("%c ", piece);
            }
        }
        printf("\n");
    }

    for (int i = 1; i <= COLS; i++) {
        printf("%d ", i);
    }
    printf("\n");

    fflush(stdout);
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

bool highlight[ROWS][COLS] = {false};

bool checkWin(char player, char board[ROWS][COLS], int last_row, int last_col) {
    int r, c;
    int count;

    // Reset highlights every check
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            highlight[i][j] = false;
        }
    }

    int directions[4][2] = {
        {0, 1}, // Horizontal
        {1, 0}, // Vertical
        {1, 1}, // Diagonal
        {1, -1} // Diagonal
    };

    for (int d = 0; d < 4; d++) {
        int dr = directions[d][0];
        int dc = directions[d][1];
        count = 1;

        int coords[7][2];
        int coordCount = 0;

        coords[coordCount][0] = last_row;
        coords[coordCount][1] = last_col;
        coordCount++;

        r = last_row + dr;
        c = last_col + dc;
        while (r >= 0 && r < ROWS && c >= 0 && c < COLS &&
               board[r][c] == player) {
            coords[coordCount][0] = r;
            coords[coordCount][1] = c;
            coordCount++;
            count++;
            r += dr;
            c += dc;
        }

        r = last_row - dr;
        c = last_col - dc;
        while (r >= 0 && r < ROWS && c >= 0 && c < COLS &&
               board[r][c] == player) {
            coords[coordCount][0] = r;
            coords[coordCount][1] = c;
            coordCount++;
            count++;
            r -= dr;
            c -= dc;
        }

        if (count >= 4) {
            for (int k = 0; k < coordCount; k++) {
                highlight[coords[k][0]][coords[k][1]] = true;
            }
            return true;
        }
    }
    return false;
}

void sleepSeconds(double seconds) {
    struct timespec ts;
    ts.tv_sec = (time_t)seconds;                      // seconds
    ts.tv_nsec = (long)((seconds - ts.tv_sec) * 1e9); // nanoseconds
    nanosleep(&ts, NULL);
}

void clearScreen() { printf("\033[H\033[J"); }