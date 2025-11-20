#include "Connect4.h"

int getAIMove(char board[ROWS][COLS], Difficulty difficulty) {
    if (difficulty == EASY) {
        return getAIMoveEasy(board);
    } else if (difficulty == MEDIUM) {
        return getAIMoveMedium(board);
    } else {
        return getAIMoveHard(board);
    }
}

//---------------------------------------------------
// easy move
//---------------------------------------------------

int getAIMoveEasy(char board[ROWS][COLS]) {
    int col;
    do {
        col = (rand() % COLS) + 1;
    } while (!checkChoice(col, board));
    return col;
}

//---------------------------------------------------
// everything used for medium move here
//---------------------------------------------------

void simulateMove(char board[ROWS][COLS], char tempBoard[ROWS][COLS], int col,
                  char player, int *sim_row) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            tempBoard[i][j] = board[i][j];
        }
    }

    int colIndex = col - 1;
    *sim_row = -1;
    for (int i = ROWS - 1; i >= 0; i--) {
        if (tempBoard[i][colIndex] == '.') {
            tempBoard[i][colIndex] = player;
            *sim_row = i;
            break;
        }
    }
}

int getAIMoveMedium(char board[ROWS][COLS]) {
    char botChar = 'B';
    char playerChar = 'A';
    char tempBoard[ROWS][COLS];
    int sim_row;

    // check if bot can win, if it can it will
    for (int col = 1; col <= COLS; col++) {
        if (checkChoice(col, board)) {
            simulateMove(board, tempBoard, col, botChar, &sim_row);
            if (sim_row != -1 &&
                checkNInRow(botChar, tempBoard, sim_row, col - 1, 4, false)) {
                return col;
            }
        }
    }

    // check if player can win, if yes block it
    for (int col = 1; col <= COLS; col++) {
        if (checkChoice(col, board)) {
            simulateMove(board, tempBoard, col, playerChar, &sim_row);
            if (sim_row != -1 && checkNInRow(playerChar, tempBoard, sim_row,
                                             col - 1, 4, false)) {
                return col;
            }
        }
    }

    // here it tries to make a 3 connected line
    int setupMoves[COLS];
    int setupCount = 0;
    for (int col = 1; col <= COLS; col++) {
        if (checkChoice(col, board)) {
            simulateMove(board, tempBoard, col, botChar, &sim_row);
            if (sim_row != -1 &&
                checkNInRow(botChar, tempBoard, sim_row, col - 1, 3, false)) {
                setupMoves[setupCount++] = col;
            }
        }
    }
    if (setupCount > 0) {
        return setupMoves[rand() % setupCount];
    }

    // here it makes the bot play in the center rather than the edges
    int centerCols[] = {3, 4, 5};
    int available[3];
    int count = 0;

    for (int i = 0; i < 3; i++) {
        int col = centerCols[i];
        if (checkChoice(col, board)) {
            available[count++] = col;
        }
    }

    if (count > 0) {
        return available[rand() % count];
    }

    // if everything else fails, we pick a random col
    for (int col = 1; col <= COLS; col++) {
        if (checkChoice(col, board)) {
            return col;
        }
    }

    return 4; // it never reaches this
}

//-------------------------------------------------
// everything used for hard move here
//-------------------------------------------------

static void undoMove(int col, char board[ROWS][COLS]) {
    int colIndex = col - 1;
    for (int r = 0; r < ROWS; r++) {
        if (board[r][colIndex] != '.') {
            board[r][colIndex] = '.';
            return;
        }
    }
}

bool hasWinner(char board[ROWS][COLS], char player) {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            if (board[r][c] == player) {
                if (checkNInRow(player, board, r, c, 4, false)) {
                    return true;
                }
            }
        }
    }
    return false;
}

static int evaluateBoard(char board[ROWS][COLS], char aiPlayer,
                         char humanPlayer) {
    if (hasWinner(board, aiPlayer))
        return 100000;
    if (hasWinner(board, humanPlayer))
        return -100000;
    if (BoardFull(board))
        return 0;

    int score = 0;
    int centerColIndex = COLS / 2;
    for (int r = 0; r < ROWS; r++) {
        if (board[r][centerColIndex] == aiPlayer)
            score += 5;
        else if (board[r][centerColIndex] == humanPlayer)
            score -= 5;
    }
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            if (board[r][c] == aiPlayer) {
                if (checkNInRow(aiPlayer, board, r, c, 3, false))
                    score += 20;
                if (checkNInRow(aiPlayer, board, r, c, 2, false))
                    score += 5;
            } else if (board[r][c] == humanPlayer) {
                if (checkNInRow(humanPlayer, board, r, c, 3, false))
                    score -= 25;
                if (checkNInRow(humanPlayer, board, r, c, 2, false))
                    score -= 7;
            }
        }
    }
    return score;
}

static int negamax(char board[ROWS][COLS], int depth, int alpha, int beta, char player, char opponent) {
    if (hasWinner(board, opponent))  // opponent just played
        return -100000;  // losing position
    if (BoardFull(board) || depth == 0)
        return evaluateBoard(board, player, opponent);

    int maxScore = -1000000;
    for (int col = 1; col <= COLS; col++) {
        if (!checkChoice(col, board))
            continue;

        makeMove(col, player, board);
        int score = -negamax(board, depth - 1, -beta, -alpha, opponent, player);
        undoMove(col, board);

        if (score > maxScore)
            maxScore = score;

        if (score > alpha)
            alpha = score;

        if (alpha >= beta)
            break;  // alpha-beta pruning
    }
    return maxScore;
}

int getAIMoveHard(char board[ROWS][COLS]) {
    int bestMove = 1;
    int bestScore = -1000000;
    char bot = 'B';
    char human = 'A';
    int depth = 6; // tune for performance vs intelligence

    for (int col = 1; col <= COLS; col++) {
        if (!checkChoice(col, board))
            continue;

        makeMove(col, bot, board);
        int score = -negamax(board, depth - 1, -1000000, 1000000, human, bot);
        undoMove(col, board);

        if (score > bestScore) {
            bestScore = score;
            bestMove = col;
        }
    }

    return bestMove;
}
