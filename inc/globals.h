#ifndef GLOBALS_H
#define GLOBALS_H

#include <genesis.h>


void delayVBlank(u16 vblanks);
void delayFrames(u16 frames) ;
void delayFramesWithSound(u16 frames) ;
void delayMilliseconds(u16 milliseconds);
void vblankCallback();

const u16 black_palette[16];
u16 ind;
bool is_start_pressed;
u16 random_number;
extern bool bPlayerCanMove;
bool bBattleStarted;
bool bBattleOngoing;
u16 delayCounter;
u16 turnDelayFrames;
bool bShowMenu;
bool bInsideHouse;
const u16 TURN_DELAY;
u32 worldSeed;
bool bBattleMessageDone;
extern bool canFight;


#endif