#include "Connect4.h"

void startNetworkClient() {
    int sockfd;
    struct sockaddr_in serv_addr;
    char server_ip[32];

    printf("=== Connect4 Client Mode ===\n");

    // Keep asking until a valid IP is entered
    while (1) {
        printf("Enter server IP (e.g. 192.168.1.10): ");
        fflush(stdout);
        if (scanf("%31s", server_ip) != 1) {
            flushInput();
            printf("Invalid input! Try again.\n");
            continue;
        }
        flushInput();

        if (!isValidIPv4(server_ip)) {
            printf("Invalid IP address format! Try again.\n");
            continue;
        }
        break; // valid IP, exit loop
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
        perror("inet_pton failed");
        close(sockfd);
        return;
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        close(sockfd);
        return;
    }

    printf("Connected to server.\n");

    char board[ROWS][COLS];
    setupBoard(board);

    char player = 'A';
    int numMoves = 0;

    while (1) {
        printBoard(board);

        if (player == 'A') {
            printf("Waiting for opponent...\n");

            char buffer[16];
            int bytes = read(sockfd, buffer, sizeof(buffer) - 1);

            if (bytes <= 0) {
                if (bytes == 0)
                    printf("Server closed the connection.\n");
                else
                    perror("Read error");
                break;
            }

            buffer[bytes] = '\0';
            int col = atoi(buffer);

            makeMove(col, 'A', board);
            numMoves++;

        } else {
            flushInput();

            int col;
            int valid_input = 0;

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
                    printf("Invalid column! Try again.\n");
                } else {
                    valid_input = 1;
                }
            }

            makeMove(col, 'B', board);
            numMoves++;

            char msg[16];
            snprintf(msg, sizeof(msg), "%d\n", col);
            if (write(sockfd, msg, strlen(msg)) < 0) {
                perror("Write error");
                break;
            }
        }

        if (hasWinner(board, player)) {
            printBoard(board);
            printf("Player %c wins!\n", player);
            break;
        }

        if (BoardFull(board)) {
            printBoard(board);
            printf("It's a draw!\n");
            break;
        }

        player = switchPlayer(player);
    }

    close(sockfd);
}
