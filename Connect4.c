#include "Connect4.h"

//copy this to immediately compile in terminal
//gcc -Wall -Werror main.c Connect4.c -o connect4
void startGame() {
    char Gamemode = '\0';
    char input[10];       
    bool choseMode = false;

    printf("Welcome to Connect 4!\n");

    while (!choseMode) {
        printf("Select mode:\nPlayer vs Player (P)  Vs AI (A): ");

        if (fgets(input, sizeof(input), stdin)) {
            Gamemode = toupper(input[0]); 

            if (Gamemode == 'P') {
                choseMode = true;
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
