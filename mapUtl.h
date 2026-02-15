#ifndef LABYRINTH_MAPUTL_H
#define LABYRINTH_MAPUTL_H
#include "globals.h"

// UI
void clear_screen();       // Basic Function to clear screen/ ! Might not work in all terminals !
void greetings();          // Greetings
void print_map_UI();       // Developing ...
void print_map_numbered(); // The function which is used when user wants to put Temp Walls in the map and is number to some extend.

// Pnp
int trigger_pnp_as_runner_on_cell(); // this init Function initiates another function in pnp.c
int how_many_mystery_boxes();        // Determines the possible amount of mystery boxes based on a logical formula
void map_PnPed();                    // Places boxes in the map

// Utilities
void zero2DArr(int A[MAX][2], int n, int m); // Makes a matrix with only zeros
void baseMap();
void borders();
void swap(int *x, int *y);
void insert_tmp_wall(int x, int y);
int read_non_newline_char(void);

// Input Management
void getDimen(); // Gets dimensions
void epic_print(char *text);
void get_walls_interactive(); // Just messing around
void getRnHnK();
void customBoard(); // Phase one is here

// ID
int cordID(int x, int y); // Uses the x*m + y formula to store both x and y in only one number called ID
void deCordID(int id, int *x, int *y);
int getWallCoords(int id, int *gridRow, int *gridCol);

// Path Finding
int isblocked(int r, int c, int k); // Checking if the entity staying in Row : r , Columns : c can move to direction : k or not
int bfs(int sr, int sc);            // Uses BFS Algorithm to find out if there's a possible route
int stepsFromDistance(int a, int b, int id);
int manhattanDistance(int a, int b, int x, int y);

// Random Generators
void randQ(int *x, int *y); // generates random coordination for an entity
int randWall(int k);
void placeEntities(int H, int R); // Randomly places entities
void mapGen(int seed);            // Generates a sudo random map

#endif // LABYRINTH_MAPUTL_H
