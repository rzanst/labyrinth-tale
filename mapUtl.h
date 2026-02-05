//
// Created by duke laptop on 12/2/2025.
//

#ifndef LABYRINTH_MAPUTL_H
#define LABYRINTH_MAPUTL_H
#include "globals.h"

// UI
void clear_screen();
void greetings();
void print_map_UI(char grid[MAX][MAX], int n, int m);
void print_map_debug(char grid[MAX][MAX], int n, int m);

// Utilities
void zeroArr(int A[MAX][MAX], int n, int m);
void baseMap();
void borders();
void swap(int *x, int *y);

// Input Management
void getDimen(int *n, int *m);
void epic_print(char *text);
void get_walls_interactive(int *k);
void getRnHnK(int *R, int *H, int *k);
void customBoard(int n, int m);

// ID
int cordID(int x, int y);
void deCordID(int id, int *x, int *y);
int getWallCoords(int id, int *gridRow, int *gridCol);

// Path Finding
int isblocked(int r, int c, int k);
int bfs(int sr, int sc);
int stepsFromDistance(int a, int b, int id);
int manhattanDistance(int a, int b, int x, int y);

// Random Generators
void randQ(int *x, int *y);
int randWall(int k);
void placeEntities(int H, int R);
int mapGen(int k, int seed);

#endif // LABYRINTH_MAPUTL_H
