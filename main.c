#include "Connect4.h"

#ifdef TEST_BOTS
int main() {

    srand((unsigned)time(NULL));
    char board[ROWS][COLS];
    int winsA = 0, winsB = 0, draws = 0;
    int games = 200;

    for (int g = 0; g < games; g++) {
        setupBoard(board);
        char current = 'A';
        while (!BoardFull(board) && !hasWinner(board, 'A') &&
               !hasWinner(board, 'B')) {
            int move;
            if (current == 'A') {
                move = getAIMoveHard(board); // bot A
            } else {
                move = getAIMoveMedium(board); // bot B
            }
            makeMove(move, current, board);
            current = switchPlayer(current);
        }

        if (hasWinner(board, 'A'))
            winsA++;
        else if (hasWinner(board, 'B'))
            winsB++;
        else
            draws++;
    }

    printf("Bot A (Hard) wins: %d\n", winsA);
    printf("Bot B (Medium) wins: %d\n", winsB);
    printf("Draws: %d\n", draws);

    return 0;
}

#else
int main() {
    srand((unsigned)time(NULL));
    startMenu();
    return 0;
}
#endif
