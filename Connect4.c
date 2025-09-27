#include "Connect4.h"
#include <string.h>

//copy this to immediately compile in terminal
//gcc -Wall -Werror main.c Connect4.c -o connect4
//welcome rayan
void startGame() {
    char Gamemode = '\0';
    char input[10];       
    bool chooseMode = false;

    printf("=================================\n");
    printf("    Welcome to Connect 4!\n");
    printf("=================================\n");
    printf("Get 4 pieces in a row to win!\n");

    while (!chooseMode) {
        printf("Select mode:\nPlayer vs Player (P) or Player vs AI (A): ");

        if (fgets(input, sizeof(input), stdin)) {
            Gamemode = toupper(input[0]); 

            if (Gamemode == 'P') {
                chooseMode = true;
                printf("Starting Player vs Player game...\n");
                // call your player vs player function here
            } else if (Gamemode == 'A') {
                printf("AI not yet implemented, please enter P.\n");
            } else {
                printf("Invalid input. Enter P or A.\n");
            }
        }
    }
}
