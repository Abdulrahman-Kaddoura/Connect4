#include "Connect4.h"
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define PORT 4444

void startNetworkClient(char* server_ip) {

    int sockfd;
    struct sockaddr_in serv_addr;

    printf("=== Connect4 Client Mode ===\n");
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, server_ip, &serv_addr.sin_addr);

    connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    printf("Connected to server.\n");

    char board[ROWS][COLS];
    setupBoard(board);

    char player = 'A';
    int numMoves = 0;

    while (true) {
        printBoard(board);

        if (player == 'A') {

            printf("Waiting for opponent...\n");

            char buffer[16];
            int bytes = read(sockfd, buffer, sizeof(buffer)-1);
            if (bytes <= 0) break;

            buffer[bytes] = '\0';
            int col = atoi(buffer);

            makeMove(col, 'A', board);
            numMoves++;

        } else {

            flushInput();

            int col;
            int valid_input = 0;

            while(!valid_input){
                printf("Your move (1-7): ");
                fflush(stdout);

                if(scanf("%d", &col) != 1){
                    flushInput();
                    printf("Invalid input! Please enter a number.\n");
                    continue;
                }

                flushInput();

                if(!checkChoice(col, board)){
                    printf("Invalid column! Try again.\n");
                } else {
                    valid_input = 1;
                }
            }
            

            makeMove(col, 'B', board);
            numMoves++;

            char msg[16];
            snprintf(msg, sizeof(msg), "%d\n", col);
            write(sockfd, msg, strlen(msg));
        }

        if(hasWinner(board, player)) {
            printBoard(board);
            printf("Player %c wins!\n", player);
            break;
        }
        if(BoardFull(board)) {
            printBoard(board);
            printf("It's a draw!\n");
            break;
        }

        player = switchPlayer(player);
    }

    close(sockfd);
}
