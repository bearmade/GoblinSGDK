#ifndef TITLE_SCREEN_H
#define TITLE_SCREEN_H

#include <genesis.h>

void displayTitle();
void displayLoadMenu();
void playerNameInput();
void all3playerNames();
void cheatCode();
void displaySoundTestMenu();
void delayWithScrolling(u16 frames);
void showSegaLogo();

char player_name1[11];
char player_name2[11];
char player_name3[11];
int hoffset;
int loadSelection;
extern bool bMenuVisible;

bool showTitleScreen;

#endif