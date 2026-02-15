#ifndef LABYRINTH_AI_H
#define LABYRINTH_AI_H

#include "globals.h" // Required for MAX constant

typedef struct
{
    int runner_x, runner_y;
    int hunter_x[MAX], hunter_y[MAX]; // Changed to arrays for multiple hunters
    int temp_walls[100][3];           // x, y, health
    int num_temp_walls;
    int turns_left_for_walls[100];
} GameState;

void copy_game_state(GameState *state);

// Updated signatures to accept pointer/arrays for hunters
int evaluate_position(int runner_x, int runner_y, int *hunter_x, int *hunter_y);
int evaluate_position_ButMathematicallyBetter(int runner_x, int runner_y, int *hunter_x, int *hunter_y);
int evaluate_position_(int runner_x, int runner_y, int *hunter_x, int *hunter_y);

int is_valid_move(int x, int y, int dir);
void simulate_runner_move(GameState *state, int dir, int *new_x, int *new_y);
void simulate_hunter_move(GameState *state);
void update_temp_walls_simulation(GameState *state);
int min_max(GameState *state, int depth, int is_maximizing, int alpha, int beta);
char suggest_best_move(int depth);

#endif // LABYRINTH_AI_H