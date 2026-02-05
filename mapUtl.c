#include <stdio.h>
#include <math.h>
#include "globals.h"
#include <time.h>
#include <stdlib.h>
#include "mapUtl.h"
#include <unistd.h> // برای تابع usleep (ایجاد تاخیر)

int dist[MAXV];

// UI
void clear_screen()
{
  printf("\033[H\033[J");
  fflush(stdout);
}

void print_map_UI(char grid[MAX][MAX], int n, int m)
{
  int newN = 2 * n + 1;
  int newM = 2 * m + 1;
  for (int i = 0; i < newN; i++)
  {
    for (int j = 0; j < newM; j++)
    {
      if (grid[i][j] == '.' || grid[i][j] == ':')
        printf(" ");
      else
        printf("%c ", grid[i][j]);
    }
    printf("\n");
  }
}
void print_map_debug(char grid[MAX][MAX], int n, int m)
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
}

// Utilities
void zeroArr(int A[MAX][MAX], int n, int m)
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
      if (i % 2 == 0)
      {
        if (j % 2 == 0)
        {
          grid[i][j] = '+';
        }
        else
          grid[i][j] = '.';
      }
      else
      {
        if (j % 2 == 0)
        {
          grid[i][j] = ':';
        }
        else
          grid[i][j] = ' ';
      }
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
      grid[0][j] = '+';
      grid[2 * n][j] = '+';
    }
    else
    {
      grid[0][j] = '-';
      grid[2 * n][j] = '-';
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

// Input Management
void getDimen(int *n, int *m)
{
  int sw = 0;
  do
  {
    if (sw)
    {
      printf("Please Enter a valid number for dimensions: (rows and columns can't be less than 2!)\n");
    }
    printf("Enter the board dimensions (rows columns):\n>>> ");
    scanf("%d %d", &*n, &*m);
    sw = 1;
  } while (*n < 2 || *m < 2);
  getchar();
}
void epic_print(char *text)
{
  while (*text)
  {
    printf("%c", *text++);
    fflush(stdout);
    usleep(30000); // سرعت تایپ متن (۳۰ میلی ثانیه)
  }
  printf("\n");
}
void get_walls_interactive(int *k)
{
  int t, a, random_val;
  int sw = 0;

  while (1)
  {
    if (sw == -1)
    {
      printf("\n\033[1;33m"); // تغییر رنگ به طلایی
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
        *k = 0;
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
    { // مدیریت ورودی اشتباه
      while (getchar() != '\n')
        ;
      continue;
    }

    if (t <= 0)
    {
      sw = -1;
      continue;
    }

    // فرض می‌کنیم تابع randWall تعداد دیوارهای قابل جایگذاری را برمی‌گرداند
    int max_possible = randWall(t);
    if (t > max_possible)
    {
      printf("\033[1;31m\n[!] The cavern is too small for such hubris! The stone cannot take form.\033[0m\n");
      printf("You requested %d walls, but only %d can be carved from the rock.\n", t, max_possible);
    }
    else
    {
      *k = t;
      printf("\033[1;32mThe earth groans as the stones take their place...\033[0m\n");
      break;
    }
  }
}

void getRnHnK(int *R, int *H, int *k)
{
  int t;
  printf("How many brave Runners will challenge the darkness?\n");
  scanf("%d", &t);
  *R = t;
  printf("How many silent Hunters lurk in the shadows?\n");
  scanf("%d", &t);
  *H = t;
  get_walls_interactive(k);
  //    printf("How many Ancient Barriers shall rise from the depths to block the forgotten paths? (The Great Borders already stand eternal.)\n");
  //    scanf("%d",&t);
  //    *k = t;
}
void customBoard(int n, int m)
{
  int i, j;
  int sw = 0;

  zeroArr(hunters, n, m);
  zeroArr(runners, n, m);

  int nl, ml;
  nl = 2 * n + 1;
  ml = 2 * m + 1;

  baseMap();

  // Light Core placement
  int cx, cy;
  sw = 0;
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
    runners[x][y] = 1;
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

  int k;
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
  print_map_debug(grid, n, m);

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

  // Set every cell distance to -1 (means: "we haven't reached it yet")
  for (int i = 0; i < V; i++)
    dist[i] = -1;

  int q[MAXV];   // our queue: a line of cells to process
  int front = 0; // front = where we remove from the line
  int back = 0;  // back  = where we add to the line

  // Turn (row, col) into ONE number so it fits in the queue easily.
  // Think of it like giving each cell a unique sticker number.
  int start = sr * m + sc;

  dist[start] = 0;   // start cell is 0 steps from itself
  q[back++] = start; // put start cell into the queue (line)

  int visitedCount = 1; // we already visited the start cell

  // These arrays tell us how to move:
  // up, right, down, left (4 directions)
  int dr[4] = {-1, 0, 1, 0}; // row changes
  int dc[4] = {0, 1, 0, -1}; // col changes

  // While the line (queue) is NOT empty...
  while (front < back)
  {

    int cur = q[front++]; // take the first cell in the line
    // front++ means "move the front forward"

    int r = cur / m; // convert cur ID back into row
    int c = cur % m; // convert cur ID back into col

    // Try all 4 directions from this cell
    for (int k = 0; k < 4; k++)
    {

      int nr = r + dr[k]; // neighbor row (new row)
      int nc = c + dc[k]; // neighbor col (new col)

      // If neighbor is outside the board, skip it
      if (nr < 0 || nr >= m || nc < 0 || nc >= m)
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
  return ((fabs(a - x) + fabs(b - y)));
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

  // پیمایش روی تمام لبه‌های ممکن به صورت خطی
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
    // اگر تعداد سلول‌های بازدید شده برابر کل سلول‌ها باشد، یعنی مسیر باز است
    if (bfs(0, 0) == n * m)
    {
      wallsPlaced++;
      // Wall stays
    }
    else
    {
      // Wall breaks connectivity, remove it (restore)
      grid[gridRow][gridCol] = backup; // likely ' ' or '.' depending on your map
    }
  }

  free(edges); // پاکسازی حافظه

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
        grid[2 * x + 1][2 * y + 1] = 'H';
        placedH++;
      }
    }
  }
}
int mapGen(int H, int R, int k, int seed)
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
}
