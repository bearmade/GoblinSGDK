#include "../inc/globals.h"

u16 ind = TILE_USER_INDEX;
//bool is_start_pressed = 0;
u16 random_number;
u16 delayCounter;
bool bPlayerCanMove = FALSE;
bool bBattleStarted = FALSE;
bool bBattleOngoing = FALSE;
u16 turnDelayFrames = 0;
const u16 TURN_DELAY = 30;
bool bShowMenu = FALSE;
bool bInsideHouse = FALSE;

void delayVBlank(u16 vblanks) {
    delayCounter = vblanks;
    while (delayCounter > 0) {
        SYS_doVBlankProcess();
    }
}

void delayFrames(u16 frames) {
    for (u16 i = 0; i < frames; i++) {
        VDP_waitVSync();
    }
}

void delayMilliseconds(u16 milliseconds) {
    u16 ticks = milliseconds * (TICKPERSECOND / 1000); // Convert ms to timer ticks
    u16 start = getTick();

    while ((getTick() - start) < ticks) {
        // Wait
    }
}
