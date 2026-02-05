#include "game_logic.h"
#include "globals.h"
#include "mapUtl.h"

static int runner_won(void)
{
    return (runners[0][0] == core_x && runners[0][1] == core_y);
}

static int runner_lost(void)
{
    return (hunters[0][0] == runners[0][0] && hunters[0][1] == runners[0][1]);
}

//
int moveRunner(char move)
{
    int is_pnp_triggered = 0;
    int move_result = 0;
    char cell = grid[2 * runners[0][0] + 1][2 * runners[0][1] + 1];
    if (cell == '*')
        is_pnp_triggered = 1;

    if (move == 'x' || move == 'X')
        return 1;

    int dir = -1;

    if (move == 'w' || move == 'W')
        dir = 0;
    else if (move == 'd' || move == 'D')
        dir = 1;
    else if (move == 's' || move == 'S')
        dir = 2;
    else if (move == 'a' || move == 'A')
        dir = 3;
    else
        return 0;

    move_result = moveEntity('R', dir, runners[0][0], runners[0][1]);
    if (!move_result)
        return 0;
    if (move_result == 2)
    {
        trigger_pnp_as_runner_on_cell();
    }
    return 1;
}

int moveHunter(void)
{
    int rx = runners[0][0];
    int ry = runners[0][1];
    for (int step = 0; step < 2; step++)
    {
        int moved = 0;
        int hx = hunters[0][0];
        int hy = hunters[0][1];

        if (hx == rx && hy == ry)
            return 1;

        if (ry > hy)
        {
            moved = moveEntity('H', 1, hx, hy);
        }
        else if (ry < hy)
        {
            moved = moveEntity('H', 3, hx, hy);
        }
        if (moved)
        {
            if (hunters[0][0] == rx && hunters[0][1] == ry)
            {
                return 1;
            }
            continue;
        }

        if (rx > hx)
        {

            moved = moveEntity('H', 2, hx, hy);
        }
        else if (rx < hx)
        {
            moved = moveEntity('H', 0, hx, hy);
        }
        if (moved)
        {
            if (hunters[0][0] == rx && hunters[0][1] == ry)
            {
                return 1;
            }
            continue;
        }
        break;
    }
    return (hunters[0][0] == rx && hunters[0][1] == ry);
}

int moveEntity(char indicator, int dir, int x, int y)
{
    int is_boxed = 1;

    int nx = x;
    int ny = y;
    if (dir == 0)
        nx--;
    else if (dir == 1)
        ny++;
    else if (dir == 2)
        nx++;
    else if (dir == 3)
        ny--;
    else
        return 0;

    if (nx < 0 || nx >= n || ny < 0 || ny >= m)
        return 0;

    if (isblocked(x, y, dir))
        return 0;

    int oldR = 2 * x + 1;
    int newR = 2 * nx + 1;
    int oldC = 2 * y + 1;
    int newC = 2 * ny + 1;

    if (grid[2 * nx + 1][2 * ny + 1] == '*')
    {
        is_boxed = 2;
    }
    if (x == core_x && y == core_y)
    {
        grid[oldR][oldC] = 'C';
    }
    else
    {
        grid[oldR][oldC] = ' ';
    }

    grid[newR][newC] = indicator;

    if (indicator == 'R')
    {
        runners[0][0] = nx;
        runners[0][1] = ny;
    }
    else if (indicator == 'H')
    {
        hunters[0][0] = nx;
        hunters[0][1] = ny;
    }

    return is_boxed;
}
