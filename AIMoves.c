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

static const int SCORE_3_OPEN = 2000;
static const int SCORE_3_BLOCKED = 1500;
static const int SCORE_2_OPEN = 200;
static const int SCORE_CENTER = 50;

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

static int evaluateWindow(char window[4], char playerChar, char oppChar) {
    int playerCount = 0, oppCount = 0, emptyCount = 0;
    for (int i = 0; i < 4; i++) {
        if (window[i] == playerChar) {
            playerCount++;
        } else if (window[i] == oppChar) {
            oppCount++;
        } else {
            emptyCount++;
        }
    }

    if (playerCount == 4) {
        return WIN_SCORE;
    }
    if (oppCount == 4) {
        return LOSS_SCORE;
    }

    if (playerCount == 3 && emptyCount == 1) {
        return SCORE_3_OPEN;
    }
    if (playerCount == 2 && emptyCount == 2) {
        return SCORE_2_OPEN;
    }
    if (oppCount == 3 && emptyCount == 1) {
        return -SCORE_3_BLOCKED;
    }
    if (oppCount == 2 && emptyCount == 2) {
        return -SCORE_2_OPEN;
    }

    return 0;
}

static int evaluateBoardStrong(char board[ROWS][COLS], char playerChar) {
    char oppChar;
    if (playerChar == 'A') {
        oppChar = 'B';
    } else {
        oppChar = 'A';
    }

    int score = 0;

    int center = COLS / 2;
    for (int r = 0; r < ROWS; r++) {
        if (board[r][center] == playerChar) {
            score += SCORE_CENTER;
        } else if (board[r][center] == oppChar) {
            score -= SCORE_CENTER;
        }
    }

    char window[4];
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c <= COLS - 4; c++) {
            for (int i = 0; i < 4; i++) {
                window[i] = board[r][c + i];
            }
            score += evaluateWindow(window, playerChar, oppChar);
        }
    }

    for (int c = 0; c < COLS; c++) {
        for (int r = 0; r <= ROWS - 4; r++) {
            for (int i = 0; i < 4; i++) {
                window[i] = board[r + i][c];
            }
            score += evaluateWindow(window, playerChar, oppChar);
        }
    }

    for (int r = 0; r <= ROWS - 4; r++) {
        for (int c = 0; c <= COLS - 4; c++) {
            for (int i = 0; i < 4; i++) {
                window[i] = board[r + i][c + i];
            }
            score += evaluateWindow(window, playerChar, oppChar);
        }
    }

    for (int r = 3; r < ROWS; r++) {
        for (int c = 0; c <= COLS - 4; c++) {
            for (int i = 0; i < 4; i++) {
                window[i] = board[r - i][c + i];
            }
            score += evaluateWindow(window, playerChar, oppChar);
        }
    }

    return score;
}

static void generateMoveOrder(int moves[], int *count) {
    int order[COLS] = {4, 3, 5, 2, 6, 1, 7};
    *count = 0;
    for (int i = 0; i < COLS; i++) {
        moves[(*count)++] = order[i];
    }
}

static int negamax_strong(char board[ROWS][COLS], int depth, int alpha,
                          int beta, char player, char opponent) {
    if (hasWinner(board, opponent)) {
        return LOSS_SCORE;
    }

    if (BoardFull(board) || depth == 0) {
        return evaluateBoardStrong(board, player);
    }

    int best = -INF;
    int moves[COLS];
    int mcount;
    generateMoveOrder(moves, &mcount);

    for (int mi = 0; mi < mcount; mi++) {
        int col = moves[mi];
        if (!checkChoice(col, board)) {
            continue;
        }

        makeMove(col, player, board);

        if (hasWinner(board, player)) {
            undoMove(col, board);
            return WIN_SCORE - ((8 - depth));
        }

        int val =
            -negamax_strong(board, depth - 1, -beta, -alpha, opponent, player);

        undoMove(col, board);

        if (val > best) {
            best = val;
        }
        if (val > alpha) {
            alpha = val;
        }
        if (alpha >= beta) {
            break;
        }
    }

    return best;
}

int getAIMoveHard_sequential(char board[ROWS][COLS]) {
    char bot = 'B';
    char human = 'A';
    int bestMove = 4;
    int bestScore = -INF;

    // win if possible
    for (int col = 1; col <= COLS; ++col) {
        if (!checkChoice(col, board)) {
            continue;
        }
        makeMove(col, bot, board);
        if (hasWinner(board, bot)) {
            undoMove(col, board);
            return col;
        }
        undoMove(col, board);
    }

    // block opponent immediate wins
    for (int col = 1; col <= COLS; ++col) {
        if (!checkChoice(col, board)) {
            continue;
        }
        makeMove(col, human, board);
        if (hasWinner(board, human)) {
            undoMove(col, board);
            return col;
        }
        undoMove(col, board);
    }

    int searchDepth = 7;

    int moves[COLS];
    int mcount;
    generateMoveOrder(moves, &mcount);
    for (int mi = 0; mi < mcount; ++mi) {
        int col = moves[mi];
        if (!checkChoice(col, board)) {
            continue;
        }

        makeMove(col, bot, board);

        int score =
            -negamax_strong(board, searchDepth - 1, -INF, INF, human, bot);

        undoMove(col, board);

        if (score > bestScore) {
            bestScore = score;
            bestMove = col;
        }
    }

    return bestMove;
}

int getAIMoveHard(char board[ROWS][COLS]){
    return getAIMoveHard_MT(board);
}


void* evaluateMoveThread(void* arg){
    ThreadData* data = (ThreadData*)arg;
    
    char threadBoard[ROWS][COLS];
    copyBoard(threadBoard, data->board);
    
    makeMove(data->col, data->bot, threadBoard);
    
    int score = -negamax_strong(threadBoard, data->searchDepth - 1, -INF, INF, data->human, data->bot);
    data->score = score;
    
    return NULL;
}

int getAIMoveHard_MT(char board[ROWS][COLS]){
    char bot = 'B';
    char human = 'A';
    int bestMove = 4;
    int bestScore = -INF;

    // this is to check for immediate wins
    for(int col = 1; col <=COLS; ++col){
        if(!checkChoice(col,board)){
            continue;
        }
        makeMove(col, bot, board);
        if(hasWinner(board, bot)){
            undoMove(col,board);
            printf("DEBUG: Found immediate win in column %d\n", col);
            return col;
        }
        undoMove(col,board);
    }

    //this is to block the opponents immediate wins
    for(int col = 1; col <=COLS; ++col){
        if(!checkChoice(col, board)){
            continue;
        }
        makeMove(col, human, board);
        if(hasWinner(board, human)){
            undoMove(col, board);
            // printf("DEBUG: Blocking opponent win in column %d\n", col);
            return col;
        }
        undoMove(col, board);
    }




    // this is the parallel evaluation of all possible moves
    pthread_t threads[COLS];
    ThreadData threadData[COLS];
    int activeThreads = 0;
    int searchDepth = 7;

    //here, we create threads for each valid move
    int moves[COLS];
    int mcount;
    generateMoveOrder(moves, &mcount);

    // printf("DEBUG: Starting parallel evaluation for up to %d possible moves...\n", mcount);

    for(int mi = 0; mi < mcount; ++mi){
        int col = moves[mi];
        if(!checkChoice(col, board)){
            continue;
        }

        copyBoard(threadData[activeThreads].board, board);
        threadData[activeThreads].col = col;
        threadData[activeThreads].searchDepth = searchDepth;
        threadData[activeThreads].bot = bot;
        threadData[activeThreads].human = human;
        threadData[activeThreads].score = -INF;

        // create thread
        if(pthread_create(&threads[activeThreads], NULL, evaluateMoveThread, &threadData[activeThreads]) == 0){
            // printf("DEBUG: Created thread for column %d\n", col); 
            activeThreads++;
        }
    }

    // printf("DEBUG: Created %d threads for valid moves.\n", activeThreads);
    // printf("DEBUG: Waiting for %d threads to complete...\n", activeThreads);


    for(int i =0; i < activeThreads; i++){
        pthread_join(threads[i], NULL);

        // printf("DEBUG: Thread %d (col %d) returned score: %d\n", i, threadData[i].col, threadData[i].score);

        if(threadData[i].score > bestScore){
            bestScore = threadData[i].score;
            bestMove = threadData[i].col;
        }
    }
    // printf("DEBUG: Best move: column %d with score %d\n", bestMove, bestScore);

    if(activeThreads == 0){

        // printf("DEBUG: Threading failed, using sequential fallback\n");
        return getAIMoveHard_sequential(board);
    }

    return bestMove;
}
