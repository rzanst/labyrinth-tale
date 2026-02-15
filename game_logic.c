#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include "mapUtl.h"
#include "globals.h"
#include "game_logic.h"
#include "PnP.h"
#include "save_load.h"

// Win/loss
// Remove 'static'. Now these are public!
int runner_won(void)
{
    return (runners[0][0] == core_x && runners[0][1] == core_y);
}

int runner_lost(void)
{
    // Note: You should loop through ALL hunters here if H > 1
    for (int i = 0; i < H; i++)
    {
        if (hunters[i][0] == runners[0][0] && hunters[i][1] == runners[0][1])
            return 1;
    }
    return 0;
}

void refresh_tmp_walls()
{
    struct tmpWall *tw;
    int x, y, health;
    for (tw = AllTempWalls; tw < AllTempWalls + numberOfTWPlaced; tw++)
    {
        if (tw->health < -1)
            continue;
        health = tw->health;
        x = tw->x;
        y = tw->y;

        if (health == -1)
        {
            grid[x][y] = ' ';

            printf("The temp wall located in x: %d, y: %d is now gone!\n", x, y);
        }

        tw->health--;
    }
}

// Movements
int is_valid_move(int x, int y, int dir)
{
    int nx = x, ny = y;

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

    return 1;
}
int moveRunner(char move)
{
    int move_result = 0;

    if (move == 'x' || move == 'X')
    {
        return 1;
    }

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
    int any_caught = 0;

    // Loop through ALL hunters
    for (int i = 0; i < H; i++)
    {
        int hx = hunters[i][0];
        int hy = hunters[i][1];

        // If this hunter is already on the runner, game is over
        if (hx == rx && hy == ry)
        {
            any_caught = 1;
            continue;
        }

        int moved = 0;

        // Try vertical movement
        if (ry > hy)
        {
            if (hx != core_x || hy + 1 != core_y)
            {
                // Passing hx, hy allows moveEntity to find the correct hunter index
                moved = moveEntity('H', 1, hx, hy);
            }
        }
        else if (ry < hy)
        {
            if (hx != core_x || hy - 1 != core_y)
            {
                moved = moveEntity('H', 3, hx, hy);
            }
        }

        if (moved)
        {
            // Check if this specific hunter caught the runner
            if (hunters[i][0] == rx && hunters[i][1] == ry)
                any_caught = 1;
            continue;
        }

        // Try horizontal movement
        if (rx > hx)
        {
            if (hx + 1 != core_x || hy != core_y)
            {
                moved = moveEntity('H', 2, hx, hy);
            }
        }
        else if (rx < hx)
        {
            if (hx - 1 != core_x || hy != core_y)
            {
                moved = moveEntity('H', 0, hx, hy);
            }
        }

        if (moved)
        {
            if (hunters[i][0] == rx && hunters[i][1] == ry)
                any_caught = 1;
            continue;
        }
    }

    return any_caught;
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

    if (grid[newR][newC] == '*')
        is_boxed = 2;

    if (x == core_x && y == core_y)
        grid[oldR][oldC] = 'C';
    else
        grid[oldR][oldC] = ' ';

    grid[newR][newC] = indicator;

    if (indicator == 'R')
    {
        runners[0][0] = nx;
        runners[0][1] = ny;
    }
    else if (indicator == 'H')
    {
        // FIND which hunter is at the old position (x,y)
        for (int i = 0; i < H; i++)
        {
            if (hunters[i][0] == x && hunters[i][1] == y)
            {
                hunters[i][0] = nx;
                hunters[i][1] = ny;
                break;
            }
        }
    }

    return is_boxed;
}
typedef struct
{
    int x, y;
    int g;        // Distance from start
    int h;        // Heuristic (Manhattan)
    int f;        // Total score (g + h)
    int parentID; // ID of the cell we came from (to backtrack)
    int closed;   // If we have finished checking this node
    int open;     // If this node is in the queue to be checked
} Node;

int a_star_path(int sx, int sy, int tx, int ty, int *next_move_dir)
{
    // 1. Safety Checks
    if (sx == tx && sy == ty)
        return 0; // Already there

    static Node mapNodes[MAX * MAX];
    int maxNodes = n * m;

    for (int i = 0; i < maxNodes; i++)
    {
        mapNodes[i].x = i / m;
        mapNodes[i].y = i % m;
        mapNodes[i].g = INT_MAX; // Assume every node is infinite steps away...
        mapNodes[i].f = INT_MAX; // ...until we prove otherwise.
        mapNodes[i].parentID = -1;
        mapNodes[i].closed = 0;
        mapNodes[i].open = 0;
    }
    int startID = sx * m + sy;
    int targetID = tx * m + ty;

    mapNodes[startID].g = 0;                           // It costs 0 steps to be where you already are.
    mapNodes[startID].h = abs(sx - tx) + abs(sy - ty); // Distance to target
    mapNodes[startID].f = mapNodes[startID].h;
    mapNodes[startID].open = 1; // Add start node to the "To Do" list

    while (1)
    {
        // Find the "best" square in the list
        int currentID = -1;
        int minF = INT_MAX;

        for (int i = 0; i < maxNodes; i++)
        {
            if (mapNodes[i].open && mapNodes[i].f < minF)
            {
                minF = mapNodes[i].f;
                currentID = i;
            }
        }

        if (currentID == -1)
            return -1; // List is empty? No path!

        if (currentID == targetID)
        {
            int steps = 0;
            int curr = targetID;
            int prev = -1;

            // Walk backwards from Goal to Start
            while (curr != startID)
            {
                prev = curr;
                curr = mapNodes[curr].parentID;
                steps++;
            }
            // If the caller requested the immediate next direction
            if (next_move_dir != NULL)
            {
                int px = mapNodes[prev].x;
                int py = mapNodes[prev].y;

                // Compare the First Step (prev) with Start (sx, sy)
                if (px == sx - 1)
                    *next_move_dir = 0; // Up
                else if (py == sy + 1)
                    *next_move_dir = 1; // Right
                else if (px == sx + 1)
                    *next_move_dir = 2; // Down
                else if (py == sy - 1)
                    *next_move_dir = 3; // Left
            }

            return steps; // Return total path length
        }
        // 6. Close the Current Node
        mapNodes[currentID].open = 0;   // Remove from "To Do" list
        mapNodes[currentID].closed = 1; // Mark as "Done"

        int cx = mapNodes[currentID].x;
        int cy = mapNodes[currentID].y;

        // 7. Check Neighbors (Up, Right, Down, Left)
        int dx[] = {-1, 0, 1, 0};
        int dy[] = {0, 1, 0, -1};

        for (int dir = 0; dir < 4; dir++)
        {
            int nx = cx + dx[dir];
            int ny = cy + dy[dir];
            int nID = nx * m + ny;

            // --- Validation Checks ---
            // Is it off the map?
            if (nx < 0 || nx >= n || ny < 0 || ny >= m)
                continue;

            // Is there a wall? (Using your existing function)
            if (isblocked(cx, cy, dir))
                continue;

            // Have we already finished checking this node?
            if (mapNodes[nID].closed)
                continue;

            // --- Path Calculation ---
            // Calculate cost to reach neighbor through current node
            int tentative_g = mapNodes[currentID].g + 1;

            // If this is a new node OR we found a shorter path to it:
            if (!mapNodes[nID].open || tentative_g < mapNodes[nID].g)
            {
                mapNodes[nID].parentID = currentID;
                mapNodes[nID].g = tentative_g;
                mapNodes[nID].h = abs(nx - tx) + abs(ny - ty); // Heuristic
                mapNodes[nID].f = mapNodes[nID].g + mapNodes[nID].h;
                mapNodes[nID].open = 1; // Add to "To Do" list
            }
        }
    }
}
