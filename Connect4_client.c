#include "Connect4.h"

void startNetworkClient() {
    int sockfd;
    struct sockaddr_in serv_addr;
    char server_ip[32];
    
    printf("=== Connect4 Client Mode ===\n");
    
    // Keep asking until a valid IP is entered
    while (1) {
        printf("Enter server IP (e.g. 192.168.1.10 or 127.0.0.1 for localhost): ");
        fflush(stdout);
        
        if (!fgets(server_ip, sizeof(server_ip), stdin)) {
            printf("Error reading input. Try again.\n");
            continue;
        }
        
        // Remove trailing newline if present
        size_t len = strlen(server_ip);
        if (len > 0 && server_ip[len - 1] == '\n') {
            server_ip[len - 1] = '\0';
        }
        
        // Validate IPv4
        if (!isValidIPv4(server_ip)) {
            printf("Invalid IP address format! Try again.\n");
            continue;
        }
        
        break; // valid IP, exit loop
    }
    
    printf("Creating socket...\n");
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return;
    }
    printf("Socket created successfully (fd=%d)\n", sockfd);
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    printf("Converting IP address...\n");
    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
        perror("inet_pton failed");
        close(sockfd);
        return;
    }
    printf("IP address converted successfully\n");
    
    printf("Attempting to connect to %s:%d...\n", server_ip, PORT);
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        printf("\nTroubleshooting tips:\n");
        printf("1. Make sure the server is running first\n");
        printf("2. Check if the IP address is correct\n");
        printf("3. Verify the PORT number matches (%d)\n", PORT);
        printf("4. Check firewall settings\n");
        printf("5. Try using 127.0.0.1 if testing locally\n");
        close(sockfd);
        return;
    }
    
    printf("✓ Connected to server successfully!\n\n");
    
    // Rest of your game code...
    char board[ROWS][COLS];
    setupBoard(board);
    
    char currentPlayer = 'A';
    
    while (1) {
        printBoard(board);
        
        if (currentPlayer == 'A') {
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
            if (!checkChoice(col, board)) {
                printf("Server sent invalid move!\n");
                break;
            }
            
            makeMove(col, 'A', board);
            
            if (hasWinner(board, 'A')) {
                printBoard(board);
                printf("Player A wins!\n");
                break;
            }
            
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
            
            char msg[16];
            snprintf(msg, sizeof(msg), "%d\n", col);
            if (write(sockfd, msg, strlen(msg)) < 0) {
                perror("Write error");
                break;
            }
            
            if (hasWinner(board, 'B')) {
                printBoard(board);
                printf("Player B wins!\n");
                break;
            }
        }
        
        if (BoardFull(board)) {
            printBoard(board);
            printf("It's a draw!\n");
            break;
        }
        
        currentPlayer = switchPlayer(currentPlayer);
    }
    
    close(sockfd);
}