#ifndef GLOBALS_H
#define GLOBALS_H

#include <genesis.h>

void delayVBlank(u16 vblanks);
void delayFrames(u16 frames) ;
void delayMilliseconds(u16 milliseconds);
u16 ind;
bool is_start_pressed;
u16 random_number;
bool bPlayerCanMove;
bool bBattleStarted;
bool bBattleOngoing;
u16 delayCounter;
u16 turnDelayFrames;

const u16 TURN_DELAY;

#endif