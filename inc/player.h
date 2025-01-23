#ifndef PLAYER_H
#define PLAYER_H

#include <genesis.h>

void displayPlayer();
void joyEvent(u16 joy, u16 changed, u16 state);
void handleInput();

Sprite* player;
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


#define PLAYER_WIDTH 16
#define PLAYER_HEIGHT 16

#define PLAYER_COL_LEFT 4
#define PLAYER_COL_RIGHT 12
#define PLAYER_COL_UP 8
#define PLAYER_COL_DOWN 10

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