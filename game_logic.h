//
// Created by duke laptop on 12/13/2025.
//

#ifndef LABYRINTH_GAME_LOGIC_H
#define LABYRINTH_GAME_LOGIC_H
#include "globals.h"

void refresh_tmp_walls();
int runner_won(void);
int runner_lost(void);
int is_valid_move(int x, int y, int dir);
int moveRunner(char move); // Zero : Invalid move, One: Normal Move, Two: Normal Move + PnP triggered
int moveHunter();
int moveEntity(char indicator, int dir, int x, int y);
int a_star_path(int sx, int sy, int tx, int ty, int *next_move_dir);

#endif // LABYRINTH_GAME_LOGIC_H
