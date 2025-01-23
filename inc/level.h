#ifndef LEVEL_H
#define LEVEL_H

#include <genesis.h>

void displayLevel();

#define SOLID_TILE 1
#define BLOCKER_TILE 12
#define EXIT_TILE 9
#define MAP_WIDTH 256
#define MAP_HEIGHT 224

extern u16 LENGTH_OF_LEVEL_ARRAY;
extern u16 SIZE_OF_LEVEL_ARRAY;
extern u16 LEVEL_COL[224];
extern u16 LEVEL_COL2[224];

Map* level_1_map;
Map* level_2_map;

#endif