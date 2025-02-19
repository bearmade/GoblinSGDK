#ifndef BATTLE_H
#define BATTLE_H

#include <genesis.h>

void initBattle();
void randomEncounter();
void displayBattle();
void nameGenerator();
void updateSelection();
void endBattle();
void attack();
void battleMessage();
void goblinAttack();
void levelUp();
void itemDrop();
void drawBox(u16 x, u16 y, u16 width, u16 height);
void updateBattleAnimation();

Sprite* goblin_sprite;
char Name[19];
bool selection;
char pointer[1];
char attack_message[13][20];
bool turn;
char damageMessage[8];

char gHP[5];
char gATK[5];
char pHP[5];
char pHPMax[5];
char pATK[5];
char  gDEF[5];
char pLevel[5];
char pGold[5];
char pExp[5];
char pDefense[5];
char pAttack[5];
char expChar[5];
char goblinsKilledChar[5];

//player stats
s16 player_hp;
s16 player_hp_max;
s16 player_attack;
s16 player_defense;
extern u16 player_gold;
s16 player_level;
u16 player_exp;
u16 player_exp_needed;
u16 goblinsKilled;
u16 goblinType;
u16 goblinOffset;
extern bool isAnimating;
extern u16 battleAnimationTimer;

//goblin stats
s16 goblin_hp;
s16 goblin_attack;
s16 goblin_defense;
u16 goldDrop;
u16 experience_gained;
int randChance;


#define SFX_SWOOSH 64
#define goblin_sprite1 0
#define goblin_sprite2 1
#define goblin_sprite3 2
#define goblin_sprite4 3
#define goblin_sprite5 4
#define goblin_sprite6 5
#define goblin_sprite7 6


#endif
