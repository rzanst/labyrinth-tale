#include "ai.h"
#include "globals.h"
#include "game_logic.h"
#include "mapUtl.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

void copy_game_state(GameState *state)
{
    state->runner_x = runners[0][0];
    state->runner_y = runners[0][1];

    // COPY ALL HUNTERS
    for (int i = 0; i < H; i++)
    {
        state->hunter_x[i] = hunters[i][0];
        state->hunter_y[i] = hunters[i][1];
    }

    state->num_temp_walls = numberOfTWPlaced;

    for (int i = 0; i < numberOfTWPlaced; i++)
    {
        state->temp_walls[i][0] = AllTempWalls[i].x;
        state->temp_walls[i][1] = AllTempWalls[i].y;
        state->temp_walls[i][2] = AllTempWalls[i].health;
    }
}

// Updated wrapper to accept arrays
int evaluate_position(int runner_x, int runner_y, int *hunter_x, int *hunter_y)
{
    if (preciseEvaluations)
    {
        return evaluate_position_ButMathematicallyBetter(runner_x, runner_y, hunter_x, hunter_y);
    }
    else
    {
        return evaluate_position_(runner_x, runner_y, hunter_x, hunter_y);
    }
}

// Updated Evaluation: Considers closest hunter
int evaluate_position_ButMathematicallyBetter(int runner_x, int runner_y, int *hunter_x, int *hunter_y)
{
    int core_distance = manhattanDistance(runner_x, runner_y, core_x, core_y);

    // FIND NEAREST HUNTER
    int min_hunter_dist = 10000;
    int closest_hunter_index = 0;

    for (int i = 0; i < H; i++)
    {
        int d = manhattanDistance(runner_x, runner_y, hunter_x[i], hunter_y[i]);
        if (d < min_hunter_dist)
        {
            min_hunter_dist = d;
            closest_hunter_index = i;
        }
    }

    // 1. CORE PROGRESS
    int max_possible_distance = n + m - 2;
    int core_progress = (max_possible_distance - core_distance) * 100;

    // 2. HUNTER SAFETY (Based on NEAREST hunter)
    int hunter_safety = 0;
    if (min_hunter_dist == 0)
        hunter_safety = -10000; // Captured
    else if (min_hunter_dist == 1)
        hunter_safety = -5000; // Adjacent (Deadly)
    else if (min_hunter_dist == 2)
        hunter_safety = -1000; // Very close
    else if (min_hunter_dist <= 4)
        hunter_safety = -200; // Close range
    else
        hunter_safety = min_hunter_dist * 50; // Safe zone

    // 3. ESCAPE ROUTES
    int escape_routes = 0;
    for (int dir = 0; dir < 4; dir++)
    {
        if (is_valid_move(runner_x, runner_y, dir))
        {
            int nx = runner_x, ny = runner_y;
            if (dir == 0)
                nx--;
            else if (dir == 1)
                ny++;
            else if (dir == 2)
                nx++;
            else if (dir == 3)
                ny--;

            // Check if this move increases distance from the NEAREST hunter
            int new_dist = abs(nx - hunter_x[closest_hunter_index]) + abs(ny - hunter_y[closest_hunter_index]);

            if (new_dist > min_hunter_dist)
                escape_routes += 3;
            else
                escape_routes += 1;
        }
    }
    escape_routes *= 20;

    // 4. CORNERED PENALTY
    int cornered_penalty = 0;
    int blocked_directions = 0;
    for (int dir = 0; dir < 4; dir++)
    {
        if (!is_valid_move(runner_x, runner_y, dir))
            blocked_directions++;
    }
    if (blocked_directions >= 3)
        cornered_penalty = -1000;
    else if (blocked_directions == 2 && min_hunter_dist <= 2)
        cornered_penalty = -500;

    // COMBINE SCORE
    int total_score = core_progress + hunter_safety + escape_routes + cornered_penalty;

    // WIN/LOSS OVERRIDES
    if (runner_x == core_x && runner_y == core_y)
        return 100000;
    if (min_hunter_dist == 0)
        return -100000;

    return total_score;
}

// Updated fallback evaluation for arrays
int evaluate_position_(int runner_x, int runner_y, int *hunter_x, int *hunter_y)
{
    int core_distance = abs(runner_x - core_x) + abs(runner_y - core_y);

    // Find closest hunter
    int min_hunter_dist = 10000;
    for (int i = 0; i < H; i++)
    {
        int d = abs(runner_x - hunter_x[i]) + abs(runner_y - hunter_y[i]);
        if (d < min_hunter_dist)
            min_hunter_dist = d;
    }

    // Check Win
    if (runner_x == core_x && runner_y == core_y)
        return 10000;

    // Check Loss (Collision with any hunter)
    if (min_hunter_dist == 0)
        return -10000;

    int score = (sqrt(n * n + m * m) - core_distance) * 10 + (min_hunter_dist) * 5;
    score += rand() % 3;

    return score;
}

void simulate_runner_move(GameState *state, int dir, int *new_x, int *new_y)
{
    *new_x = state->runner_x;
    *new_y = state->runner_y;

    if (dir == 0)
        (*new_x)--;
    else if (dir == 1)
        (*new_y)++;
    else if (dir == 2)
        (*new_x)++;
    else if (dir == 3)
        (*new_y)--;
}

int would_hit_core(int x, int y, int dir)
{
    int nx = x, ny = y;
    if (dir == 0)
        nx--;
    else if (dir == 1)
        ny++;
    else if (dir == 2)
        nx++;
    else if (dir == 3)
        ny--;
    return (nx == core_x && ny == core_y);
}

void simulate_hunter_move(GameState *state)
{
    int rx = state->runner_x;
    int ry = state->runner_y;

    // MOVE ALL HUNTERS
    for (int i = 0; i < H; i++)
    {
        int hx = state->hunter_x[i];
        int hy = state->hunter_y[i];

        if (hx == rx && hy == ry)
            continue; // Already caught

        int moved = 0;

        // Same deterministic greedy logic as game_logic.c
        // 1. Vertical
        if (ry > hy)
        {
            if (!would_hit_core(hx, hy, 1) && !isblocked(hx, hy, 1))
            {
                state->hunter_y[i]++;
                moved = 1;
            }
        }
        else if (ry < hy)
        {
            if (!would_hit_core(hx, hy, 3) && !isblocked(hx, hy, 3))
            {
                state->hunter_y[i]--;
                moved = 1;
            }
        }

        if (moved)
            continue;

        // 2. Horizontal
        if (rx > hx)
        {
            if (!would_hit_core(hx, hy, 2) && !isblocked(hx, hy, 2))
            {
                state->hunter_x[i]++;
            }
        }
        else if (rx < hx)
        {
            if (!would_hit_core(hx, hy, 0) && !isblocked(hx, hy, 0))
            {
                state->hunter_x[i]--;
            }
        }
    }
}

void update_temp_walls_simulation(GameState *state)
{
    for (int i = 0; i < state->num_temp_walls; i++)
    {
        state->temp_walls[i][2]--;
    }
}

// The heart of the AI.
int min_max(GameState *state, int depth, int is_maximizing, int alpha, int beta)
{
    // Check Win
    if (state->runner_x == core_x && state->runner_y == core_y)
    {
        return 10000 - depth;
    }

    // Check Loss (Any Hunter)
    for (int i = 0; i < H; i++)
    {
        if (state->runner_x == state->hunter_x[i] && state->runner_y == state->hunter_y[i])
        {
            return -10000 + depth;
        }
    }

    if (depth == 0)
    {
        return evaluate_position(state->runner_x, state->runner_y, state->hunter_x, state->hunter_y);
    }

    // # Runner Turn (Maximizing)
    if (is_maximizing)
    {
        int max_eval = INT_MIN;

        // Try all 4 directions
        for (int dir = 0; dir < 4; dir++)
        {
            if (is_valid_move(state->runner_x, state->runner_y, dir))
            {
                GameState new_state = *state;

                // 1. Sim Runner's Move
                simulate_runner_move(&new_state, dir, &new_state.runner_x, &new_state.runner_y);
                update_temp_walls_simulation(&new_state);

                // 2. Sim Hunter's Move (Deterministic)
                // Note: In min-max, normally we alternate turns.
                // Since hunters are greedy bots, we handle their turn in the 'else' block below
                // OR we can simulate them right here if we treat them as part of the environment.
                // Standard MinMax recurses to the Minimizer (Hunter).

                int eval = min_max(&new_state, depth - 1, 0, alpha, beta); // Switch to minimizing turn (0)

                max_eval = (eval > max_eval) ? eval : max_eval;
                alpha = (alpha > eval) ? alpha : eval;
                if (beta <= alpha)
                    break;
            }
        }

        if (max_eval == INT_MIN)
        {
            return evaluate_position(state->runner_x, state->runner_y, state->hunter_x, state->hunter_y);
        }

        return max_eval;
    }
    // # Hunter Turn (Minimizing/Environment)
    else
    {
        // FIX: Do not branch (try all moves) for hunters.
        // Hunters move deterministically based on `simulate_hunter_move`.
        // We simply apply their move and recurse back to the Runner.

        GameState new_state = *state;
        simulate_hunter_move(&new_state);

        // Recurse back to Maximizing (Runner's turn)
        return min_max(&new_state, depth - 1, 1, alpha, beta);
    }
}

char suggest_best_move(int depth)
{
    GameState current_state;
    copy_game_state(&current_state);

    int best_score = INT_MIN;
    int best_dir = -1;
    char dir_chars[] = {'w', 'd', 's', 'a'};

    printf("\n--- AI Suggestion (Multi-Hunter Mode) ---\n");
    printf("Analyzing possible moves...\n");

    for (int dir = 0; dir < 4; dir++)
    {
        if (is_valid_move(current_state.runner_x, current_state.runner_y, dir))
        {
            GameState new_state = current_state;

            // 1. Runner Moves
            simulate_runner_move(&new_state, dir, &new_state.runner_x, &new_state.runner_y);

            // 2. Hunters Move (Immediate reaction)
            simulate_hunter_move(&new_state);

            update_temp_walls_simulation(&new_state);

            // 3. Evaluate Future
            int score = min_max(&new_state, depth - 1, 0, INT_MIN, INT_MAX);

            printf("Move %c: Score = %d\n", dir_chars[dir], score);

            if (score > best_score)
            {
                best_score = score;
                best_dir = dir;
            }
        }
        else
        {
            printf("Move %c: Blocked\n", dir_chars[dir]);
        }
    }

    if (best_dir == -1)
    {
        printf("No valid moves found! Suggesting 'x' (stay)\n");
        return 'x';
    }

    printf("\nBest move: %c (Score: %d)\n", dir_chars[best_dir], best_score);
    return dir_chars[best_dir];
}