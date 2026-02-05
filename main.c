#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "mapUtl.h"
#include "globals.h"
#include "game_logic.h"
#include "PnP.h"

static int read_non_newline_char(void)
{
    int c;
    do
    {
        c = getchar();
    } while (c == '\n' || c == '\r');
    return c;
}

static int runner_won(void)
{
    return (runners[0][0] == core_x && runners[0][1] == core_y);
}

static int runner_lost(void)
{
    return (hunters[0][0] == runners[0][0] && hunters[0][1] == runners[0][1]);
}

int main(void)
{

    seed = 10; // time(NULL);
    srand(seed);

    greetings();
    getDimen(&n, &m);
    getRnHnK(&R, &H, &k);
    mapGen(k, seed, 1);

    pnp_just_happened = 0;

    while (1)
    {
        if (!pnp_just_happened)
        {
            print_map_debug(grid, n, m);
        }
        pnp_just_happened = 0;

        if (runner_won())
        {
            print_map_debug(grid, n, m);
            printf("\nYOU WIN! Runner reached the Core.\n");
            break;
        }

        if (runner_lost())
        {
            print_map_debug(grid, n, m);
            printf("\nYOU LOSE! Hunter caught the Runner.\n");
            break;
        }

        printf("\nMove: W/A/S/D | Stay: X | Quit: q\n");
        printf(">>> ");

        int cmd = read_non_newline_char();
        while (getchar() != '\n')
            ;

        if (cmd == 'q' || cmd == 'Q')
        {
            printf("Exiting the Labyrinth...\n");
            break;
        }

        int acted = 0;

        if (cmd == 'w' || cmd == 'a' || cmd == 's' || cmd == 'd' ||
            cmd == 'W' || cmd == 'A' || cmd == 'S' || cmd == 'D' ||
            cmd == 'x' || cmd == 'X')
        {
            int result = moveRunner((char)cmd);

            if (result == 0)
            {
                printf("Invalid move! Press Enter...\n");
                getchar();
                continue;
            }
            acted = 1;
        }
        else
        {
            printf("Invalid command! Press Enter...\n");
            getchar();
            continue;
        }

        if (acted)
        {
            if (runner_won())
                continue;

            if (moveHunter())
            {
                print_map_debug(grid, n, m);
                printf("\nYOU LOSE! Hunter caught the Runner.\n");
                break;
            }
        }
    }

    getchar();
    return 0;
}