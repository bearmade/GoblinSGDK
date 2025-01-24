#ifndef PLAYER_H
#define PLAYER_H

#include <genesis.h>

void displayPlayer();
void joyEvent(u16 joy, u16 changed, u16 state);
void handleInput();
void initBattle();
void randomEncounter();
void displayBattle();
void nameGenerator();
void updateSelection();
void endBattle();
void attack();
void battleMessage();
void goblinAttack();

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
int randChance;
bool bIsMoving;
char Name[19];
bool selection;
char pointer[1];
char attack_message[8][20];
bool turn;
char damageMessage[8];

char gHP[5];
char gATK[5];
char pHP[5];
char pATK[5];

//player stats
s16 player_hp;
u16 player_attack;
u16 player_defense;
u16 player_gold;
u16 player_level;
u16 player_exp;
u16 player_exp_needed;


//goblin stats
s16 goblin_hp;
u16 goblin_attack;
u16 goblin_defense;




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