cmake_minimum_required(VERSION 3.23)
project(Labyrinth C)

set(CMAKE_C_STANDARD 23)

add_executable(Labyrinth main.c mapUtl.c mapUtl.h game_logic.c game_logic.h PnP.c PnP.h save_load.c save_load.h globals.c globals.h)
