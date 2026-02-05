#include "PnP.h"
#include "globals.h"
#include "game_logic.h"
#include <stdio.h>
#include <stdlib.h>

static int read_non_newline_char(void)
{
    int c;
    do
    {
        c = getchar();
    } while (c == '\n' || c == '\r');
    return c;
}

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
    printf("Press Enter to continue...");
    getchar();

    while (1)
    {
        int c = read_non_newline_char();
        // clear rest of line
        while (getchar() != '\n')
            ;

        if (moveRunner((char)c))
            return 0;
        printf("Invalid extra move. Try again:\n>>> ");
    }
}

int addTmpWall(void)
{
    tmpWall += 2;
    printf("\n[Power] You got 2 temp walls! (tmpWall=%d)\n", tmpWall);
    printf("Press Enter to continue...");
    getchar();
    return 0;
}

int earthQuake(void)
{
    printf("\n[Power] Earthquake! Everyone tries to move randomly once.\n");
    printf("Press Enter to continue...");
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
    }

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
        printf("Hunter %d at (x=%d, y=%d)\n", i + 1, hunters[i][0], hunters[i][1]);
    }

    printf("Choose hunter number (1..%d):\n>>> ", H);
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
        printf("Direction: u/d/r/l\n>>> ");
        char dirC = (char)read_non_newline_char();
        while (getchar() != '\n')
            ;

        int dir = -1;
        if (dirC == 'u')
            dir = 0;
        else if (dirC == 'r')
            dir = 1;
        else if (dirC == 'd')
            dir = 2;
        else if (dirC == 'l')
            dir = 3;
        else
        {
            printf("Invalid direction.\n");
            continue;
        }

        if (moveEntity('H', dir, hunters[chosen][0], hunters[chosen][1]))
            return 0;
        printf("Blocked. Try another direction.\n");
    }
}
