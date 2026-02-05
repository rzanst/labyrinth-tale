#include "PnP.h"
#include "globals.h"
#include "game_logic.h"
#include <stdio.h>
#include <stdlib.h>

int pnp_init_(int command)
{
    if (command == 1)
    {
        extraMove();
    }
    else if (command == 2)
    {
        addTmpWall();
    }
    else if (command == 3)
    {
        earthQuake();
    }
    else if (command == 4)
    {
        tpHunter();
    }
    else
    {
        return 1;
    }
    return 0;
}

int extraMove()
{
    char newMove;
    newMove = getchar();
    moveRunner(newMove);
    while (getchar() != '\n')
        ;
}

int addTmpWall()
{
    tmpWall += 2;
    printf("\n you got 2 temp walls!! \n");
}

int earthQuake()
{
    int dir, r, h;
    for (r = 0; r < R; r++)
    {
        do
        {
            dir = rand() % 4;
        } while (moveEntity('R', dir, runners[r][0], runners[r][1]));
    }
    for (h = 0; h < H; h++)
    {
        do
        {
            dir = rand() % 4;
        } while (moveEntity('R', dir, hunters[h][0], hunters[h][1]));
    }
}

int tpHunter()
{
    int i = 0;
    char dirC;
    int chosenHunter, dir;
    printf("\nyou are given the chance to determine one of hunters cords\n");
    printf("choose the hunter you'd like by typing its number");
    for (i = 0; i < H; i++)
    {
        printf("Hunter Number %d is staying in x:%d, y:%d\n", i + 1, hunters[i][0], hunters[i][1]);
    }
    chosenHunter = getchar() - '0' - 1;
    while (getchar() != '\n')
        ;
    // need a loop here to check if the gamer is sure what he's doing or not !!
    do
    {
        printf("In which direction would you like to move it??\n");
        printf("u,d,r,l\n");
        dirC = getchar();
        switch (dirC)
        {
        case 'u':
            dir = 0;
            break;
        case 'd':
            dir = 2;
            break;
        case 'r':
            dir = 1;
            break;
        case 'l':
            dir = 3;
            break;
        }
    } while (moveEntity('H', dir, hunters[chosenHunter][0], hunters[chosenHunter][1]));
    return 0;
}