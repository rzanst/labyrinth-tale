
#include "globals.h"
#include <time.h>
#include "stdlib.h"

#define MAX 100
#define MAXV 500

int n;               // Rows
int m;               // Columns
int k;               // Walls
int core_x;          // Core X
int core_y;          // Core Y
char grid[MAX][MAX]; // The main Game, it contains all information about the current state of game
int runners[MAX][2]; // Coordinates of all runners x,y
int hunters[MAX][2]; // Coordinates of all hunters x,y
int runnersID[MAX];  // ID of all runners based on x*m+y
int huntersID[MAX];  // ID of all runners based on x*m+y
int tmpWallsToPlace = 2;
int seed;
int R, H;

int isPnP;

int MinMaxActivate;
int DEPTH;
int preciseEvaluations;

int defaultSettings = 1;

struct tmpWall AllTempWalls[100];
int numberOfTWPlaced = 0;
