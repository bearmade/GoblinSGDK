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
extern bool inCave;

#endif 
