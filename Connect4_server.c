#include "Connect4.h"
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define PORT 4444

void startNetworkServer() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    printf("=== Connect4 Server Mode ===\n");
    printf("Waiting for a client to connect...\n");

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind");
        close(server_fd);
        return;
    }

    if (listen(server_fd, 1) < 0) {
        perror("listen");
        close(server_fd);
        return;
    }

    client_fd = accept(server_fd, (struct sockaddr *)&address, &addrlen);
    if (client_fd < 0) {
        perror("accept");
        close(server_fd);
        return;
    }
    printf("Client connected!\n");

    char board[ROWS][COLS];
    setupBoard(board);

    char player = 'A';
    int numMoves = 0;

    while (true) {
        printBoard(board);

        if (player == 'A') {

            int col;
            int valid_input = 0;

            flushInput();

            while (!valid_input) {
                printf("Your move (1-7): ");
                fflush(stdout);

                if (scanf("%d", &col) != 1) {
                    flushInput();
                    printf("Invalid input! Please enter a number.\n");
                    continue;
                }

                flushInput();

                if (!checkChoice(col, board)) {
                    printf("Invalid column, try again: ");
                } else {
                    valid_input = 1;
                }
            }

            makeMove(col, 'A', board);
            numMoves++;

            char msg[16];
            snprintf(msg, sizeof(msg), "%d\n", col);
            write(client_fd, msg, strlen(msg));

        } else {

            printf("Waiting for opponent...\n");

            char buffer[16];
            int bytes = read(client_fd, buffer, sizeof(buffer) - 1);
            if (bytes <= 0) {
                printf("Client disconnected or read error.\n");
                break;
            }

            buffer[bytes] = '\0';
            int col = atoi(buffer);

            makeMove(col, 'B', board);
            numMoves++;
        }

        if (hasWinner(board, player)) {
            printBoard(board);
            printf("\nPlayer %c wins!\n", player);
            break;
        }
        if (BoardFull(board)) {
            printBoard(board);
            printf("It's a draw!\n");
            break;
        }
        player = switchPlayer(player);
    }

    close(client_fd);
    close(server_fd);
}