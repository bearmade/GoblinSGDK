#ifndef DUNGEON_GENERATOR_H
#define DUNGEON_GENERATOR_H

#include <genesis.h>

#define DUNGEON_HEIGHT 32
#define DUNGEON_WIDTH  40

void generateDungeon(u16 mapHeight, u16 mapWidth, u16 steps, u16 dungeonLayout[][mapWidth]);
void setupDungeon(void);
void drawDungeonMap(u16 mapHeight, u16 mapWidth, u16 dungeonLayout[][mapWidth]);

u16 myDungeon[DUNGEON_HEIGHT][DUNGEON_WIDTH];

#endif 
