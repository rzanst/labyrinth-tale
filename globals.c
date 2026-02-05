
#include "globals.h"
#include <time.h>

#define MAX 100
#define MAXV 500

int n, m, k;
int core_x, core_y;
char grid[MAX][MAX];
int runners[MAX][2];
int hunters[MAX][2];
int runnersID[MAX];
int huntersID[MAX];
int tmpWall;
int tempWallsPlaced[MAX];
int seed;
int R, H;
int box[MAX][MAX];
int pnp_just_happened;

// A place for other global
// variables !!