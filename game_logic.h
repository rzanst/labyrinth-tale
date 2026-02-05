//
// Created by duke laptop on 12/13/2025.
//

#ifndef LABYRINTH_GAME_LOGIC_H
#define LABYRINTH_GAME_LOGIC_H
#include "globals.h"

static int runner_won(void);
static int runner_lost(void);
int moveRunner(char move);
int moveHunter();
int moveEntity(char indicator, int dir, int x, int y);

#endif // LABYRINTH_GAME_LOGIC_H
