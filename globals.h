#ifndef LABYRINTH_GLOBALS_H
#define LABYRINTH_GLOBALS_H

#define MAX 100
#define MAXV 500
// Globals:

extern int n, m, k;
extern int core_x, core_y;
extern char grid[MAX][MAX];
extern int runnersID[MAX];
extern int huntersID[MAX];
extern int runners[MAX][2];
extern int hunters[MAX][2];
extern int tmpWallsToPlace;
extern int seed;
extern int R, H;

extern int isPnP;

extern int MinMaxActivate;
extern int DEPTH;
extern int preciseEvaluations;

extern int defaultSettings;

struct tmpWall
{
    int x;
    int y;
    int health;
};
extern struct tmpWall AllTempWalls[100];
extern int numberOfTWPlaced;

#endif
