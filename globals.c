//
// Created by duke laptop on 12/19/2025.
//

#include "globals.h"
#include <time.h>

#define MAX 100
#define MAXV 500

int n, m;
int core_x, core_y;
char grid[MAX][MAX];
int runners[MAX][MAX];
int hunters[MAX][MAX];
int runnersID[MAX];
int huntersID[MAX];
int tmpWall;
int tempWallsPlaced[MAX];
int seed;
int R, H;

// A place for other global
// variables !!