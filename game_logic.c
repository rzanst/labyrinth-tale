#include "game_logic.h"
#include "globals.h"
#include "mapUtl.h"

//
int moveRunner(char move)
{
    //    check if is blocked
    //    return 0 if not possible
    //    return type of cell if move possible
    // extra move 'M' => 1
    // extra tmp wall 'T' => 2
    // earth quake 'Q' => 3
    // tp hunter '%' => 4
    //    change grid
}

int moveHunter()
{
    //    update grid
}

int moveEntity(char indicator, int dir, int x, int y)
{
    // indicator indicates that what type of entity it is and how it is
    // Showed on the board.
    // dir = 0 up 1 right 2 down 3 left

    // change map if the move possible
    // return 0
    // return 1 if it blocked or if the dir is incorrect or any other problem
}
