#include <stdio.h>
#include <math.h>
#include "globals.h"
#include <time.h>
#include <stdlib.h>
#include "mapUtl.h"
#include <unistd.h>
#include "PnP.h"

int dist[MAXV];

// UI
void clear_screen()
{
    printf("\033[H\033[J");
    fflush(stdout);
}
void greetings()
{
    printf("\n");
    printf("==================================================\n");
    printf("          THE LABYRINTH OF ARKA                   \n");
    printf("   Fundamentals of Programming - Fall 1404        \n");
    printf("==================================================\n");
    printf("\n");
    printf("Deep beneath the earth, in eternal darkness...\n");
    printf("lies the hidden city of Arka - a deadly maze of stone and shadow.\n");
    printf("Silent hunters prowl the tunnels. Only one Runner remains.\n");
    printf("\n");
    printf("Your mission:\n");
    printf("   -> Reach the Light Core\n");
    printf("   -> Survive the Shadow Hunters\n");
    printf("   -> Become legend...\n");
    printf("\n");
    printf("Good luck, last hope of the surface world.\n");
    printf("--------------------------------------------------\n");

    printf("Press Enter to Play ...  \n");
    getchar();
    clear_screen();
}
void print_map_UI()
{
    int newN = 2 * n + 1;
    int newM = 2 * m + 1;
    for (int i = 0; i < newN; i++)
    {
        for (int j = 0; j < newM; j++)
        {
            printf("%c ", grid[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int min(int a, int b)
{
    if (a < b)
        return a;

    return b;
}

void print_map_numbered()
{
    int newN = 2 * n + 1;
    int newM = 2 * m + 1;
    int maxHor = min(newM, 5);
    int maxVer = min(newN, 5);

    printf("\n");
    printf("     ");
    for (int i = 1; i < maxHor + 4; i++)
    {
        if (i > maxHor)
        {
            printf(".");
            continue;
        }
        printf("%d", i);
        if (i < newM - 2)
            printf(",");
    }
    printf("\n\n");

    for (int i = 0; i < newN; i++)
    {
        if (i == 0)
            printf("   ");
        else
        {
            maxVer--;
            if (maxVer >= 0)
            {
                printf("%d  ", i);
            }
            else if (maxVer == -1)
            {
                printf("...");
            }
            else
                printf("   ");
        }

        for (int j = 0; j < newM; j++)
        {
            printf("%c ", grid[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// PnP
int trigger_pnp_as_runner_on_cell()
{
    clear_screen();
    print_map_UI();

    printf("\n -------- Mystery Box Activated! -------- \n");
    getchar();

    int r = rand() % 4;
    if (r == 0)
        pnp_init('M'); // extra move
    else if (r == 1)
        pnp_init('T'); // +2 temp walls
    else if (r == 2)
        pnp_init('Q'); // earthquake
    else
        pnp_init('%'); // teleport hunter

    return 1;
}
int how_many_mystery_boxes()
{
    int cells = n * m;
    int base = 2;               // Guaranteed minimum
    int runner_bonus = R / 4;   // More runners = more boxes
    int hunter_bonus = H / 3;   // Balance hunter threat
    int size_bonus = cells / 8; // ~12% density
    int wall_bonus = k / 8;     // More walls = more boxes

    int result = base + runner_bonus + hunter_bonus + size_bonus + wall_bonus;

    int min_boxes = cells / 12; // ~8% minimum density
    int max_boxes = cells / 5;  // ~20% maximum density

    if (result < min_boxes)
        result = min_boxes;
    if (result > max_boxes)
        result = max_boxes;

    return result;
}
void map_PnPed()
{
    int x, y;
    const int boxes = how_many_mystery_boxes();
    for (int i = 0; i < boxes;)
    {
        x = rand() % n;
        y = rand() % m;
        if (grid[2 * x + 1][2 * y + 1] == ' ')
        {
            grid[2 * x + 1][2 * y + 1] = '*';
            i++;
        }
    }
}

// Utilities
void zero2DArr(int A[MAX][2], int n, int m)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            A[i][j] = 0;
        }
    }
}
void baseMap()
{
    int i, j;
    for (i = 0; i < 2 * n + 1; i++)
    {
        for (j = 0; j < 2 * m + 1; j++)
        {
            if (i % 2 == 0 && j % 2 == 0)
                grid[i][j] = '+';
            else
                grid[i][j] = ' ';
        }
    }
}
void borders()
{
    int i, j;
    for (j = 0; j < 2 * m + 1; j++)
    {
        if (j % 2 == 0)
        {
            grid[2 * n][j] = '+';
        }
        else
        {
            grid[0][j] = grid[2 * n][j] = '-';
        }
    }
    for (i = 1; i < 2 * n + 1; i += 2)
    {
        grid[i][0] = grid[i][2 * m] = '|';
    }
}
void swap(int *x, int *y)
{
    int t = *x;
    *x = *y;
    *y = t;
}

void insert_tmp_wall(int x, int y)
{
    // Horizontal slots: Odd Row (x), Even Column (y) -> '-'
    // Vertical slots: Even Row (x), Odd Column (y) -> '|'
    if (x % 2 != 0 && y % 2 == 0)
    {
        grid[x][y] = '|';
    }
    else
    {
        grid[x][y] = '-';
    }

    // Storing temp wall's stats
    AllTempWalls[numberOfTWPlaced].x = x;
    AllTempWalls[numberOfTWPlaced].y = y;
    AllTempWalls[numberOfTWPlaced].health = 2;

    numberOfTWPlaced++;
    tmpWallsToPlace--;
}

int read_non_newline_char(void)
{
    int c;
    do
    {
        c = getchar();
    } while (c == '\n' || c == '\r');
    return c;
}

// Input Management
void getDimen()
{
    int sw = 0;
    do
    {
        if (sw)
        {
            printf("Please Enter a valid number for dimensions: (rows and columns can't be less than 2!)\n");
        }
        printf("Enter the board dimensions (rows columns):\n>>> ");
        scanf("%d %d", &n, &m);
        sw = 1;
    } while (n < 2 || m < 2);
    getchar();
}
void epic_print(char *text)
{
    while (*text)
    {
        printf("%c", *text++);
        fflush(stdout);
        usleep(30000);
    }
    printf("\n");
}
void get_walls_interactive()
{
    int t, a, random_val;
    int sw = 0;

    while (1)
    {
        if (sw == -1)
        {
            printf("\n\033[1;33m");
            epic_print("--- THE TRIAL OF NUMBERS ---");
            epic_print("Common wisdom warns against a journey without obstacles, yet some souls crave the path of least resistance.");
            epic_print("To enter a world of nothingness, you must first solve the Trial of Numbers.");
            epic_print("Pick a digit between 0 and 100... Let us see if the stars align for you.");

            printf("\033[1;36mTake your shot: \033[0m");
            scanf("%d", &a);

            random_val = rand() % 101;

            if (a == random_val)
            {
                printf("\033[1;32m\n[!] THE STARS ALIGN. You have earned your hollow victory. The walls shall remain buried.\033[0m\n");
                k = 0;
                break;
            }
            else
            {
                printf("\033[1;31m\n[!] SACRILEGE! The gods do not take kindly to cowards. Your fate is sealed.\033[0m\n");
                getchar();
                epic_print("The earth opens beneath you... Goodbye.");
                exit(1);
            }
        }

        printf("\n\033[1;37mHow many \033[1;33mAncient Barriers\033[1;37m shall rise from the depths to block the forgotten paths?\033[0m\n");
        printf("(The Great Borders already stand eternal.)\n");
        printf("\033[1;36m>>> \033[0m");

        if (scanf("%d", &t) != 1)
        {
            while (getchar() != '\n')
                ;
            continue;
        }

        if (t <= 0)
        {
            sw = -1;
            continue;
        }

        int max_possible = randWall(t);
        if (t > max_possible)
        {
            printf("\033[1;31m\n[!] The cavern is too small for such hubris! The stone cannot take form.\033[0m\n");
            printf("You requested %d walls, but only %d can be carved from the rock.\n", t, max_possible);
        }
        else
        {
            k = t;
            printf("\033[1;32mThe earth groans as the stones take their place...\033[0m\n");
            break;
        }
    }
}

void getRnHnK()
{
    int t;
    do
    {
        printf("How many brave Runners will challenge the darkness?\n");
        scanf("%d", &t);
        if (t == 0)
        {
            printf("Having Zero Runner in such game is as useless as your being!\n");
            continue;
        }
        else if (t > (n * m - 1))
        {
            printf("It's impossible to put %d runners in and %dx%d map while having one core in it!\n", t, n, m);
            continue;
        }
        R = t;
        break;
    } while (1);
    do
    {
        printf("How many silent Hunters lurk in the shadows?\n");
        scanf("%d", &t);
        if (t == 0)
        {
            printf("If you are wondering to play this game with zero hunter go play candycrush!\n");
            continue;
        }
        else if (t > (n * m - 1 - R))
        {
            printf("It's impossible to put %d runners in and %dx%d map while having one core in it beside the %d runners!\n", t, n, m, R);
            continue;
        }
        H = t;
        break;
    } while (1);

    get_walls_interactive();
    //    printf("How many Ancient Barriers shall rise from the depths to block the forgotten paths? (The Great Borders already stand eternal.)\n");
    //    scanf("%d",&t);
    //    *k = t;
}
void customBoard()
{
    int i, j;
    int sw = 0;

    zero2DArr(hunters, n, m);
    zero2DArr(runners, n, m);

    int nl, ml;
    nl = 2 * n + 1;
    ml = 2 * m + 1;

    baseMap();

    // Light Core placement
    int cx, cy;

    do
    {
        if (sw)
            printf("The Light Core cannot be placed there. Try again.\n");
        printf("Reveal the hidden position of the Light Core (row col):\n");
        scanf("%d %d", &cx, &cy);
        sw = 1;
    } while (cx < 0 || cx >= nl || cy < 0 || cy >= ml);

    grid[2 * cx + 1][2 * cy + 1] = 'C';

    // Runners placement
    int R;
    printf("How many brave Runners will challenge the darkness?\n");
    scanf("%d", &R);
    for (int k = 0; k < R; k++)
    {
        int x, y;
        sw = 0;
        do
        {
            if (sw)
                printf("Invalid or occupied position for Runner %d. Try again.\n", k + 1);
            printf("Position the Runner %d in the maze (row col):\n", k + 1);
            scanf("%d %d", &x, &y);
            sw = 1;
        } while (x < 0 || x >= nl || y < 0 || y >= ml ||
                 (x == cx && y == cy) ||
                 grid[2 * x + 1][2 * y + 1] != ' ');
        runners[k][0] = x;
        runners[k][1] = y;
        grid[2 * x + 1][2 * y + 1] = 'R';
    }

    // Hunters placement
    int H;
    printf("How many silent Hunters lurk in the shadows?\n");
    scanf("%d", &H);
    for (int k = 0; k < H; k++)
    {
        int x, y;
        sw = 0;
        do
        {
            if (sw)
                printf("Invalid or occupied position for Hunter %d. Try again.\n", k + 1);
            printf("Unleash Hunter %d into the tunnels (row col):\n", k + 1);
            scanf("%d %d", &x, &y);
            sw = 1;
        } while (x < 0 || x >= nl || y < 0 || y >= ml ||
                 (x == cx && y == cy) ||
                 grid[2 * x + 1][2 * y + 1] != ' ');
        hunters[x][y] = 1;
        grid[2 * x + 1][2 * y + 1] = 'H';
    }

    // Fixed walls placement

    printf("How many eternal stone Walls guard the ancient paths?\n");
    scanf("%d", &k);

    for (int i = 0; i < k; i++)
    {
        int x, y;
        char dir;
        sw = 0;
        do
        {
            if (sw)
                printf("Invalid wall placement. Cannot overlap or go outside!\n");
            printf("Wall %d - position (row col) and direction (H/V):\n", i + 1);
            scanf("%d %d %c", &x, &y, &dir);
            sw = 1;

            if (x < 0 || x > nl || y < 0 || y > ml)
                continue;
            if (dir != 'H' && dir != 'V')
                continue;
            if (dir == 'H' && y >= ml)
                continue;
            if (dir == 'V' && x >= nl)
                continue;

            // Checking whether the wall is placeable or not:
            int already_wall = 0;
            if (dir == 'H' && grid[2 * x][2 * y + 1] == '-')
                already_wall = 1;
            if (dir == 'V' && grid[2 * x + 1][2 * y] == '|')
                already_wall = 1;
            if (already_wall)
                continue;

            // Finally Placing walls:
            if (dir == 'H')
            {
                grid[2 * x][2 * y + 1] = '-';
            }
            else
            {
                grid[2 * x + 1][2 * y] = '|';
            }
            break;

        } while (1);
    }

    clear_screen();
    // Printing the final map:
    print_map_UI();

    getchar();
    getchar();
}

// ID
int cordID(int x, int y)
{
    return x * m + y;
}
void deCordID(int id, int *x, int *y)
{
    *x = id / m;
    *y = id % m;
}
int getWallCoords(int id, int *gridRow, int *gridCol)
{
    int numVertWalls = n * (m - 1); // Walls separating columns (|)

    // Based on our fomula if (id < numVertWalls) is true the wall is vertical
    if (id < numVertWalls)
    {
        // It's a vertical wall (|) between columns
        int r = id / (m - 1);
        int c = id % (m - 1);
        *gridRow = 2 * r + 1;
        *gridCol = 2 * c + 2; // Wall is between col c and c+1
        return 1;             // Vertical
    }
    else
    {
        // It's a horizontal wall (-) between rows
        id -= numVertWalls;
        int r = id / m;
        int c = id % m;
        *gridRow = 2 * r + 2; // Wall is between row r and r+1
        *gridCol = 2 * c + 1;
        return 0; // Horizontal
    }
}

// Path Finding
int isblocked(int r, int c, int k)
{
    switch (k)
    {
    case 0:
        if (grid[2 * r][2 * c + 1] == '-')
            return 1;
        break;
    case 1:
        if (grid[2 * r + 1][2 * c + 2] == '|')
            return 1;
        break;
    case 2:
        if (grid[2 * r + 2][2 * c + 1] == '-')
            return 1;
        break;
    case 3:
        if (grid[2 * r + 1][2 * c] == '|')
            return 1;
        break;
    }
    return 0;
}
int bfs(int sr, int sc)
{
    int V = m * n; // total number of cells = rows * cols

    // Set every cell distance to -1
    for (int i = 0; i < V; i++)
        dist[i] = -1;

    int q[MAXV]; // our queue
    int front = 0;
    int back = 0;

    // Turn (row, col) into ONE number so it fits in the queue easily.
    int start = sr * m + sc;

    dist[start] = 0;
    q[back++] = start;

    int visitedCount = 1; // we already visited the start cell

    // up, right, down, left (4 directions)
    int dr[4] = {-1, 0, 1, 0};
    int dc[4] = {0, 1, 0, -1};

    // While the line (queue) is NOT empty...
    while (front < back)
    {

        int cur = q[front++];

        int r = cur / m; // convert cur ID
        int c = cur % m;

        // Try all 4 directions
        for (int k = 0; k < 4; k++)
        {

            int nr = r + dr[k];
            int nc = c + dc[k];

            // If neighbor is outside the board, skip it
            if (nr < 0 || nr >= n || nc < 0 || nc >= m)
                continue;

            // If a wall blocks between (r,c) and (nr,nc), skip it
            if (isblocked(r, c, k))
                continue;

            // Convert neighbor (nr,nc) into one ID number
            int nxt = nr * m + nc;

            // If dist[nxt] == -1, we have NEVER visited this cell before
            if (dist[nxt] == -1)
            {

                // Neighbor is 1 step farther than current cell
                dist[nxt] = dist[cur] + 1;

                // Put this neighbor into the queue to explore later
                q[back++] = nxt;

                // Count that we visited one more cell
                visitedCount++;
            }
        }
    }

    // Return how many cells we reached from the start cell
    return visitedCount;
}
int stepsFromDistance(int a, int b, int id)
{
    // Distance from the obj in (a,b) from an obj on cell with ID = id:
    bfs(a, b);
    return dist[id];
}
int manhattanDistance(int a, int b, int x, int y)
{
    return ((abs(a - x) + abs(b - y)));
}

// Random Generators
void randQ(int *x, int *y)
{
    *x = rand() % n;
    *y = rand() % m;
}
int randWall(int k)
{
    int i, j;
    // Total possible inner walls
    // Vertical walls (|) : n rows * (m-1) gaps
    // Horizontal walls (-): (n-1) gaps * m cols
    int total_edges = (n * (m - 1)) + ((n - 1) * m);

    int *edges = malloc(total_edges * sizeof(int));
    for (i = 0; i < total_edges; i++)
        edges[i] = i;

    // Shuffle
    for (i = total_edges - 1; i > 0; i--)
    {
        j = rand() % (i + 1);
        swap(&edges[i], &edges[j]);
    }

    int wallsPlaced = 0;
    int gridRow, gridCol, isVert;

    for (i = 0; i < total_edges && wallsPlaced < k; i++)
    {

        isVert = getWallCoords(edges[i], &gridRow, &gridCol);

        // Place the wall temporarily
        char backup = grid[gridRow][gridCol];
        if (isVert)
            grid[gridRow][gridCol] = '|';
        else
            grid[gridRow][gridCol] = '-';

        // Check connectivity
        if (bfs(0, 0) == n * m)
        {
            wallsPlaced++;
            // Wall stays
        }
        else
        {
            // Wall breaks connectivity, remove it (restore)
            grid[gridRow][gridCol] = backup; // likely ' ' depending on the map
        }
    }

    free(edges); // Clearing Memory

    return wallsPlaced;
}
void placeEntities(int H, int R)
{
    int x, y, i, j;
    int placedH = 0, placedR = 0;
    int coreID = cordID(core_x, core_y);

    // 1 Placing Runners
    while (placedR < R)
    {
        randQ(&x, &y);

        if (grid[2 * x + 1][2 * y + 1] == ' ')
        {
            if (stepsFromDistance(x, y, coreID) >= 2)
            {
                runnersID[placedR] = cordID(x, y);
                runners[placedR][0] = x;
                runners[placedR][1] = y;
                grid[2 * x + 1][2 * y + 1] = 'R';
                placedR++;
            }
        }
    }

    // 2 Placing Hunters
    int attempt = 0;
    while (placedH < H && attempt < 1000)
    {
        randQ(&x, &y);
        attempt++;

        if (grid[2 * x + 1][2 * y + 1] == ' ')
        {
            // Distance from Core
            if (stepsFromDistance(x, y, coreID) < 2)
                continue;

            // Distance from each runner
            int tooClose = 0;
            for (i = 0; i < R; i++)
            {
                if (stepsFromDistance(x, y, runnersID[i]) < 2)
                {
                    tooClose = 1;
                    break;
                }
            }

            if (!tooClose)
            {
                huntersID[placedH] = cordID(x, y);
                hunters[placedH][0] = x;
                hunters[placedH][1] = y;
                grid[2 * x + 1][2 * y + 1] = 'H';
                placedH++;
            }
        }
    }
}
void mapGen(int seed)
{
    srand(seed);

    int i, j, x, y, terminate = 0;
    // Making The base of the map
    baseMap();
    borders();
    // print_map(grid, n, m);

    // Core random Placement
    randQ(&core_x, &core_y);
    grid[2 * core_x + 1][2 * core_y + 1] = 'C';
    //    printf("DEBUG : Core is now in cell : x:%d y:%d\n",core_x,core_y);
    // Wall Placement
    randWall(k);
    //    print_map_debug(grid,n,m);
    //    getchar();
    placeEntities(H, R);
    if (isPnP)
        map_PnPed();
}
