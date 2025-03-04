#ifndef DUNGEON_GENERATOR_H
#define DUNGEON_GENERATOR_H

#include <genesis.h>

// Cave generation and handling functions
void generateCaveLevel(u16 level);
void enterCave(u16 level);
void exitCave();
void updateCaves();
bool isInCave();
bool isOnCaveEntrance();
bool isOnCaveExit();
void ensurePathToExit(u16 mapheight, u16 mapwidth);
void ruleTile(u16 mapheight, u16 mapwidth, u8 type);
void generateMaze(u16 startX, u16 startY, u16 maxY, u16 maxX);
void enhanceMaze(u16 mapheight, u16 mapwidth) ;
void ensureClearPath(u16 startX, u16 startY, u16 exitX, u16 exitY);
void checkTreasureCollisions();
void showTreasurePopup(u16 x, u16 y, u16 goldAmount);
extern bool inCave;
Sprite* treasure_sprite;

#endif 
