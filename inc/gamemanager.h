#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <genesis.h>

void sramSave();
void sramLoad();

u16 roomCount;
bool MapHasBeenMade;

#endif