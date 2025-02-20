#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <genesis.h>

void sramSave(u8 slot);
void sramLoad(u8 slot);
void showPlayerHouse();
void updatePlayerHouseCooldown(u32 elapsedMs);
extern char player_name[11];
u16 roomCount;
bool MapHasBeenMade;
//extern bool bIsResting;
u32 playerHouseCooldown;

#endif