#include <stdio.h>
#include "mapUtl.h"
#include "globals.h"
#include <time.h>
#include <stdlib.h>
#include "game_logic.h"
#include "PnP.h"

int main()
{
    int k, command, response;
    //    seed = time(NULL);
    seed = time(NULL);
    srand(seed);

    greetings();
    getDimen(&n, &m);
    getRnHnK(&R, &H, &k);
    mapGen(k, seed);

    while (1)
    {
        clear_screen();
        print_map_debug(grid, n, m);

        //        printf("\n[W/A/S/D] Move | [Q] Quit Game\n>>> ");
        //        command = getchar();
        //
        //        if (command == 'q' || command == 'Q') {
        //            printf("Exiting the Labyrinth...\n");
        //            break; // Quit
        //        }
        //
        //        else if (command == 'w' || command == 'a' || command == 's' || command == 'd' ||
        //            command == 'W' || command == 'A' || command == 'S' || command == 'D') {
        //                response =  moveRunner(command);
        //                if (response==0) {
        //                    printf("This move is impossible to make!\n");
        //                }
        //                else pnp_init_(command);
        //        }
        //        else printf("Invalid input !!\n");
        while (getchar() != '\n')
            ; // Enter Buffer
    }
}