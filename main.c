#include <stdio.h>
#include <math.h>
#include "globals.h"

#define Max 100

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

  printf("Press Enter to Play ...");
  getchar();
  clear_screen();
}

void print_map(int n, int m)
{
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < m; j++)
    {
      printf("%c", grid[i][j]);
    }
    printf("\n");
  }
}

int distance(int x, int y, int a, int b)
{
  return (sqrt((pow(x - a, 2) + pow(y - b, 2))));
}

int is_distant(int x, int y, int a, int b)
{
  return distance(x, y, a, b);
}

int main()
{
  // An intro for the game:
  greetings();

  int i, j, n, m;
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

  zeroArr(hunters, n, m);
  zeroArr(runners, n, m);

  int original_n, original_m;
  original_n = n;
  original_m = m;
  n = 2 * n + 1;
  m = 2 * m + 1;

  // Making the board:

  for (i = 0; i < n; i++)
  {
    for (j = 0; j < m; j++)
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
  } while (cx < 0 || cx >= original_n || cy < 0 || cy >= original_m);

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
    } while (x < 0 || x >= original_n || y < 0 || y >= original_m ||
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
    } while (x < 0 || x >= original_n || y < 0 || y >= original_m ||
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

      if (x < 0 || x > original_n || y < 0 || y > original_m)
        continue;
      if (dir != 'H' && dir != 'V')
        continue;
      if (dir == 'H' && y >= original_m)
        continue;
      if (dir == 'V' && x >= original_n)
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
  print_map(n, m);

  getchar();
  getchar();
}