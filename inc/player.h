#ifndef PLAYER_H
#define PLAYER_H

#include <genesis.h>

void displayPlayer();
void joyEvent(u16 joy, u16 changed, u16 state);
void handleInput();
void showStats();
void displayMiniMap(int x, int y);
void screenWarble();
void showSaveMenu();

Sprite* player;
Sprite* merchant;
bool bShowMerchMenu;
u16 playerDir;
fix32 playerPosX;
fix32 playerPosY;
fix32 playerSpeed;
bool player_move_left;
bool player_move_right;
bool player_move_up;
bool player_move_down;
int attack_timer;
int attack_duration;
extern bool bIsMoving;
bool bSaveMenuActive;
int selectedSaveSlot;


fix32 tempPlayerPosX;
fix32 tempPlayerPosY;
static fix16 warblePhase;



#define PLAYER_WIDTH 32
#define PLAYER_HEIGHT 32

#define MERCHANT_WIDTH 32
#define MERCHANT_HEIGHT 32

#define PLAYER_COL_LEFT 12
#define PLAYER_COL_RIGHT 19
#define PLAYER_COL_UP 12
#define PLAYER_COL_DOWN 23

#define PLAYER_LEFTATTACK_COL_LEFT 2
#define PLAYER_LEFTATTACK_COL_RIGHT 22
#define PLAYER_LEFTATTACK_COL_UP 24
#define PLAYER_LEFTATTACK_COL_DOWN 46

#define PLAYER_RIGHTATTACK_COL_LEFT 39
#define PLAYER_RIGHTATTACK_COL_RIGHT 58
#define PLAYER_RIGHTATTACK_COL_UP 24
#define PLAYER_RIGHTATTACK_COL_DOWN 46

#define PLAYER_UPATTACK_COL_LEFT 23
#define PLAYER_UPATTACK_COL_RIGHT 53
#define PLAYER_UPATTACK_COL_UP 13
#define PLAYER_UPATTACK_COL_DOWN 29

#define PLAYER_DOWNATTACK_COL_LEFT 23
#define PLAYER_DOWNATTACK_COL_RIGHT 52
#define PLAYER_DOWNATTACK_COL_UP 41
#define PLAYER_DOWNATTACK_COL_DOWN 53

#define ANIM_IDLE_UP 3
#define ANIM_IDLE_LEFT 1
#define ANIM_IDLE_DOWN 0
#define ANIM_IDLE_RIGHT 2

#define ANIM_WALK_UP 7
#define ANIM_WALK_LEFT 5
#define ANIM_WALK_DOWN 4
#define ANIM_WALK_RIGHT 6



#define ANIM_ATTACK_UP 15
#define ANIM_ATTACK_LEFT 13
#define ANIM_ATTACK_DOWN 12
#define ANIM_ATTACK_RIGHT 14

#endif