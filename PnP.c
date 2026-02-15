#include "PnP.h"
#include "globals.h"
#include "game_logic.h"
#include <stdio.h>
#include <stdlib.h>
#include "mapUtl.h"

int pnp_init(char command)
{
    // allow numeric shortcuts too
    if (command == '1')
        command = 'M';
    if (command == '2')
        command = 'T';
    if (command == '3')
        command = 'Q';
    if (command == '4')
        command = '%';

    if (command == 'M')
        return extraMove();
    if (command == 'T')
        return addTmpWall();
    if (command == 'Q')
        return earthQuake();
    if (command == '%')
        return tpHunter();

    return 1; // unknown command
}

int extraMove(void)
{
    printf("\n[Power] Extra Move activated! Enter W/A/S/D or X to stay:\n>>> ");
    while (1)
    {
        int c = read_non_newline_char();
        while (getchar() != '\n')
            ;
        if (moveRunner((char)c))
            break;
        printf("Invalid extra move. Try again:\n>>> ");
    }
    print_map_UI();
    return 0;
}

int addTmpWall(void)
{
    tmpWallsToPlace += 2;
    printf("\n[Power] You got 2 more temp walls! (Your total Temp Walls = %d)\n", tmpWallsToPlace);
    getchar();
    return 0;
}

int earthQuake(void)
{
    printf("\n[Power] Earthquake!\nEveryone tries to move randomly once.\n");
    getchar();

    // runners
    for (int r = 0; r < R; r++)
    {
        int moved = 0;
        for (int tries = 0; tries < 10 && !moved; tries++)
        {
            int dir = rand() % 4;
            moved = moveEntity('R', dir, runners[r][0], runners[r][1]);
        }
        // Check if this Runner hit any Hunter
        for (int h = 0; h < H; h++)
        {
            if (runners[r][0] == hunters[h][0] && runners[r][1] == hunters[h][1])
            {
                printf("\n[Power] Earthquake threw you into a Hunter! GAME OVER.\n");
                printf("Press [ENTER] to close...");
                getchar();
                exit(0);
            }
        }
    }

    // hunters
    for (int h = 0; h < H; h++)
    {
        int moved = 0;
        for (int tries = 0; tries < 10 && !moved; tries++)
        {
            int dir = rand() % 4;
            moved = moveEntity('H', dir, hunters[h][0], hunters[h][1]);
        }
        // Check if this Hunter hit any Runner
        for (int r = 0; r < R; r++)
        {
            if (hunters[h][0] == runners[r][0] && hunters[h][1] == runners[r][1])
            {
                printf("\n[Power] Earthquake threw a Hunter onto you! GAME OVER.\n");
                printf("Press [ENTER] to close...");
                getchar();
                exit(0);
            }
        }
    }
    print_map_UI();
    return 0;
}

int tpHunter(void)
{
    printf("\n[Power] Move a hunter by 1 step.\n");
    printf("Press Enter to continue...");
    getchar();

    // list hunters
    for (int i = 0; i < H; i++)
    {
        printf("Hunter #%d at (x=%d, y=%d)\n", i + 1, hunters[i][0], hunters[i][1]);
    }

    printf("Choose hunter number :\n>>> ");
    int c = read_non_newline_char();
    while (getchar() != '\n')
        ;

    int chosen = (c - '0') - 1;
    if (chosen < 0 || chosen >= H)
    {
        printf("Invalid hunter number.\n");
        return 1;
    }

    while (1)
    {
        printf("Direction: W/A/S/D\n>>> ");
        char dirC = (char)read_non_newline_char();
        while (getchar() != '\n')
            ;

        int dir = -1;
        if (dirC == 'w' || dirC == 'W')
            dir = 0;
        else if (dirC == 'd' || dirC == 'D')
            dir = 1;
        else if (dirC == 's' || dirC == 'S')
            dir = 2;
        else if (dirC == 'a' || dirC == 'A')
            dir = 3;
        else
        {
            printf("Invalid direction.\n");
            continue;
        }

        if (moveEntity('H', dir, hunters[chosen][0], hunters[chosen][1]))
            break;
        printf("Blocked. Try another direction.\n");
    }
    print_map_UI();

    return 0;
}
