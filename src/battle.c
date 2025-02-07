#include "../inc/debuginfo.h"
#include "../inc/level.h"
#include "../inc/camera.h"
#include "../inc/player.h"
#include "../inc/globals.h"
#include <resources.h>
#include "../inc/titlescreen.h"
#include "../inc/makemap.h"
#include "../inc/gamemanager.h"
#include "../inc/battle.h"
#include "../inc/inventory.h"

Sprite* goblin_sprite;
int randChance = 0;
bool turn = FALSE;

char attack_message[13][20] = {
	"throw your shoe at  ",
	"clumsily trip into  ",
	"toss pocket lint at ",
	"yell very loudly at ",
	"perform nose boop on",
	"throw a rock towards",
	"attempt to kick     ",
	"do a barrel roll at ",
	"run up on and tickle",
	"kick the shins of   ",
	"bust a move on      ",
	"act like a bear 	 ",
	"offend the goblin   "

};
char damageMessage[8];

//player stats
s16 player_hp = 100;
s16 player_hp_max = 100;
s16 player_attack = 7;
s16 player_defense = 8;
u16 player_gold = 0;
s16 player_level = 1;
u16 player_exp = 0;
u16 player_exp_needed = 10;
u16 goblinsKilled = 0;

//goblin stats
s16 goblin_hp = 0;
s16 goblin_attack = 0;
s16 goblin_defense = 0;
u16 goldDrop = 0;
u16 experience_gained;
u16 goblinType = 0;
u16 goblinOffset = 0;

char pHP[5];
char pHPMax[5];
char pATK[5];
char gHP[5];
char gATK[5];
char gDEF[5];
char gold[5];
char pLevel[5];
char pGold[5];
char pExp[5];
char pDefense[5];
char pAttack[5];
char expChar[5];
char goblinsKilledChar[5];

//#define SFX_SWOOSH 64

void initBattle(){

	bPlayerCanMove = FALSE;
	bBattleStarted = TRUE;
 // Reset all movement states
    player_move_left = FALSE;
    player_move_right = FALSE;
    player_move_up = FALSE;
    player_move_down = FALSE;
    bIsMoving = FALSE;
	goblin_hp = (player_level * 10)+ random() % 25;
	goblin_attack = (player_level * 2)+ (random() % 10);
	goblin_defense = (player_level * 2)+ random() % 10;
	goldDrop = (goblin_attack + goblin_defense)+ random() % 15;
	goblinType = random() % 7;
	PAL_setPalette(PAL1, goblinSprite.palette->data, DMA);
	goblin_sprite = SPR_addSprite(&goblinSprite, 160, 120, TILE_ATTR(PAL1, FALSE, FALSE, FALSE));
	switch (goblinType){
		case 0:
		goblinOffset = 0;
		SPR_setAnim(goblin_sprite, goblin_sprite1);
		break;
		case 1:
		goblinOffset = 37;
		SPR_setAnim(goblin_sprite, goblin_sprite2);
		break;
		case 2:
		goblinOffset = 37*2;
		SPR_setAnim(goblin_sprite, goblin_sprite3);
		break;
		case 3:
		goblinOffset = 37*3;
		SPR_setAnim(goblin_sprite, goblin_sprite4);
		break;
		case 4:
		goblinOffset = 37*4;
		SPR_setAnim(goblin_sprite, 	goblin_sprite5);
		break;
		case 5:
		goblinOffset = 37*5;
		SPR_setAnim(goblin_sprite, 	goblin_sprite6);
		break;
		case 6:
		goblinOffset = 37*6;
		SPR_setAnim(goblin_sprite, goblin_sprite7);
		break;

		}



}

void randomEncounter(){
	randChance = random() % 1000;
	if(randChance <= 5 && !isTransitioning){
		XGM_stopPlay();
        XGM_startPlay(battle_vgm);
		initBattle();
		turn = TRUE;
	}
}

void displayBattle(){
	
	//clear  screen

	//VDP_clearTileMap(BG_B, ind, 0, TRUE);

	SPR_setVisibility(player, HIDDEN);
	SPR_setVisibility(goblin_sprite, VISIBLE);
	if (bBattleStarted == TRUE){
	bBattleMessageDone = FALSE;
	VDP_clearTileMap(BG_A, ind, 0, TRUE);
	bBattleOngoing = TRUE;
	//load font tiles
	VDP_loadFontData(tileset_Font.tiles, 96, CPU);
	//set font palette
	PAL_setPalette(PAL0, palette_Font.data, DMA);

	VDP_drawTextBG( BG_B, "Goblin Encounter!", 8, 0);
	delayFrames(360);
	bBattleMessageDone = TRUE;

	VDP_drawTextBG( BG_B, "                 ", 8, 0);
	//draw text on window plane
	nameGenerator();
	VDP_drawTextBG(BG_B, Name, 5, 4);

	bBattleStarted = FALSE;
	//show battle menu
	
	VDP_drawTextBG( BG_B, ".Attack.", 22, 24);
	VDP_drawTextBG( BG_B, ".Run.", 22, 26);
	//sprintf(pointer, "%c", selection);
	
	
VDP_loadTileSet(goblin.tileset, 1026, DMA);
    PAL_setPalette(PAL1, goblin.palette->data, DMA);
	u16 randoBackdrop = random() % 2;
	switch (randoBackdrop){
		case 0:
		
		PAL_setPalette(PAL2, battleBack.palette->data, DMA);
		VDP_drawImageEx(BG_A, &battleBack, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ind), 0, 0, FALSE, TRUE);
		break;
		case 1:
		
		PAL_setPalette(PAL2, battleBack2.palette->data, DMA);
		VDP_drawImageEx(BG_A, &battleBack2, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ind), 0, 0, FALSE, TRUE);
		break;
	}
	// PAL_setPalette(PAL2, battleBack.palette->data, DMA);
	//     VDP_drawImageEx(BG_A, &battleBack, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ind), 0, 0, FALSE, TRUE);




	}
	while(bBattleOngoing){
	
	SPR_update();
		VDP_drawTextBG( BG_B, "                 ", 4, 0);
		//show player stats
		VDP_drawTextBG( BG_B, "Player", 2, 24);
		VDP_drawTextBG( BG_B, "HP: ", 2, 26);
		sprintf(pHP, "%d", player_hp);
		VDP_drawTextBG( BG_B, pHP, 8, 26);
		VDP_drawTextBG( BG_B, "/", 12, 26);
		sprintf(pHPMax, "%d", player_hp_max);
		VDP_drawTextBG( BG_B, pHPMax, 14, 26);
			VDP_drawTextBG( BG_B, "ATK: ", 10, 8);
			sprintf(gATK, "%d", goblin_attack);
			VDP_drawTextBG( BG_B, gATK, 14, 8);
			VDP_drawTextBG( BG_B, "DEF: ", 10, 10);
			sprintf(gDEF, "%d", goblin_defense);
			VDP_drawTextBG( BG_B, gDEF, 14, 10);


	    //show goblin stats
		if (goblin_hp > 0){
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 1 + goblinOffset +1025), 20, 14);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 2+ goblinOffset+1025), 21, 14);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 3+ goblinOffset+1025), 22, 14);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 4+ goblinOffset+1025), 23, 14);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 5+ goblinOffset+1025), 24, 14);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 6+ goblinOffset+1025), 25, 14);

			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 8+ goblinOffset+1025), 20, 15);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 9+ goblinOffset+1025), 21, 15);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 10+ goblinOffset+1025), 22, 15);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 11+ goblinOffset+1025), 23, 15);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 12+ goblinOffset+1025), 24, 15);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 13+ goblinOffset+1025), 25, 15);

			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 14+ goblinOffset+1025), 20, 16);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 15+ goblinOffset+1025), 21, 16);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 16+ goblinOffset+1025), 22, 16);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 17+ goblinOffset+1025), 23, 16);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 18+ goblinOffset+1025), 24, 16);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 19+ goblinOffset+1025), 25, 16);

			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 20+ goblinOffset+1025), 20, 17);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 21+ goblinOffset+1025), 21, 17);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 23+ goblinOffset+1025), 23, 17);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 24+ goblinOffset+1025), 24, 17);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 25+ goblinOffset+1025), 25, 17);

			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 26+ goblinOffset+1025), 20, 18);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 27+ goblinOffset+1025), 21, 18);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 28+ goblinOffset+1025), 22, 18);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 29+ goblinOffset+1025), 23, 18);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 30+ goblinOffset+1025), 24, 18);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 31+ goblinOffset+1025), 25, 18);

			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 32+ goblinOffset+1025), 20, 19);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 33+ goblinOffset+1025), 21, 19);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 34+ goblinOffset+1025), 22, 19);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 35+ goblinOffset+1025), 23, 19);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 36+ goblinOffset+1025), 24, 19);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 37+ goblinOffset+1025), 25, 19);ALSE, 22+ goblinOffset+1025), 22, 17);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 23+ goblinOffset+1025), 23, 17);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 24+ goblinOffset+1025), 24, 17);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 25+ goblinOffset+1025), 25, 17);

			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 26+ goblinOffset+1025), 20, 18);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 27+ goblinOffset+1025), 21, 18);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 28+ goblinOffset+1025), 22, 18);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 29+ goblinOffset+1025), 23, 18);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 30+ goblinOffset+1025), 24, 18);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 31+ goblinOffset+1025), 25, 18);

			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 32+ goblinOffset+1025), 20, 19);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 33+ goblinOffset+1025), 21, 19);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 34+ goblinOffset+1025), 22, 19);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 35+ goblinOffset+1025), 23, 19);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 36+ goblinOffset+1025), 24, 19);
			// VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 37+ goblinOffset+1025), 25, 19);






			VDP_drawTextBG( BG_B, "HP: ", 10, 6);
			sprintf(gHP, "%d", goblin_hp);
			VDP_drawTextBG( BG_B, gHP, 14, 6);

		}
		else{
			bBattleMessageDone = FALSE;
			experience_gained = (player_level + random() % 2);
			VDP_clearTileMap(BG_B, ind, 1, TRUE);
			VDP_drawTextBG(BG_B, "        ", 19, 8);
			VDP_drawTextBG( BG_B, "        ", 19, 12);
			VDP_drawTextBG( BG_B, "is DEAD", 10, 6);
			VDP_drawTextBG( BG_B, "        ", 10, 8);
			VDP_drawTextBG( BG_B, "        ", 10, 10);
			VDP_drawTextBG( BG_B, "Found", 10, 12);
			sprintf(gold, "%d", goldDrop);
			VDP_drawTextBG( BG_B, gold, 16, 12);
			VDP_drawTextBG( BG_B, "Gold!", 10, 14);
			VDP_drawTextBG( BG_B, "Gained", 10, 16);
			VDP_drawTextBG( BG_B, "Exp!", 10, 18);
			sprintf(expChar, "%d", experience_gained);
			VDP_drawTextBG( BG_B, expChar, 16, 18);
			itemDrop();

		}
		//get player selection
		VDP_drawTextBG( BG_B, " ", 21, ((selection == 0 ? 1 : 0)*2 + 24)); // Clear old cursor
		VDP_drawTextBG( BG_B, "~", 21, ((selection*2) + 24)); // Draw new cursor
		
		if(goblin_hp <= 0){
			bBattleMessageDone = FALSE;
			goblinsKilled++;
			player_gold += goldDrop;
			XGM_startPlay(victory_vgm);
			
			player_exp += experience_gained;
			VDP_clearTileMap(BG_B, ind, 1, TRUE);
			if (player_exp >= player_exp_needed){
				levelUp();
				delayFrames(180);
			}

			delayFrames(180);
			bBattleOngoing = FALSE;
			bBattleMessageDone = FALSE;
			endBattle();
		}

		
		
		SYS_doVBlankProcess();


		
			}
	
	


}
void nameGenerator(){

    char firstName1[6][2] = {"Sn", "Sk", "Kr", "Br", "Gr","D'"};
    char firstName2[5][2] = {"ar", "um", "ay", "ee", "oo"};
    char firstName3[5][3] = {"po ", "py ", "bs ", "ble", "gle"};
    char middleName[5][3] = {"Tum", "Bum", "Rub", "Hum", "Gru"};
    char lastName1[5][4] = {"thum", "bopo", "arum", "atum", "abum"};
    char lastName2[5][4] = {"lo  ", "bles", "gles", "po  ", "py  "};
    u16 rand = random() % 6;
    u16 rand2 = random() % 5;
    u16 rand3 = random() % 5;
    u16 rand4 = random() % 5;
    u16 rand5 = random() % 5;
    u16 rand6 = random() % 5;
    u16 rand7 = random() % 5;
    u16 rand8 = random() % 5;
    Name[0] = firstName1[rand][0];
    Name[1] = firstName1[rand][1];
    Name[2] = firstName2[rand2][0];
    Name[3] = firstName2[rand2][1];
    Name[4] = firstName3[rand3][0];
    Name[5] = firstName3[rand3][1];
    Name[6] = firstName3[rand3][2];
    Name[7] = ' ';
    Name[8] = middleName[rand4][0];
    Name[9] = middleName[rand4][1];
    Name[10] = middleName[rand4][2];
    Name[11] = lastName1[rand5][0];
    Name[12] = lastName1[rand5][1];
    Name[13] = lastName1[rand5][2];
    Name[14] = lastName1[rand5][3];
    Name[15] = lastName2[rand6][0];
    Name[16] = lastName2[rand6][1];
    Name[17] = lastName2[rand6][2];
    Name[18] = lastName2[rand6][3];
    sprintf(Name, "%s", Name);

 }

 void updateSelection(){

	//get joypad input to update selection


 }
 void endBattle() {
    bBattleOngoing = FALSE;
    bPlayerCanMove = TRUE;
	SPR_setVisibility(goblin_sprite, HIDDEN);

    VDP_clearPlane(BG_B, TRUE);
	VDP_clearPlane(BG_A, TRUE);
	VDP_loadTileSet(tileset1.tileset, 1, DMA);
    PAL_setPalette(PAL1, tileset1.palette->data, DMA);
	PAL_setPalette(PAL0, fg2.palette->data, DMA);
	PAL_setPalette(PAL3,palette_Font.data, DMA);
	// clear all battle messages
	VDP_drawTextBG( BG_B, "                 ", 3, 10);
	VDP_drawTextBG( BG_B, "                 ", 5, 4);
	VDP_drawTextBG( BG_B, "                 ", 3, 2);
	VDP_drawTextBG( BG_B, "                 ", 7, 2);
	VDP_drawTextBG( BG_B, "                 ", 14, 6);
	VDP_drawTextBG( BG_B, "                 ", 2, 24);
	VDP_drawTextBG( BG_B, "                 ", 2, 26);
	PAL_setPalette(PAL3, merchantSprite.palette->data, DMA);
	SYS_doVBlankProcess();
	XGM_stopPlay();
	XGM_startPlay(world_vgm);
PAL_setPalette(PAL2, our_sprite.palette->data, DMA);
	SPR_setVisibility(player, VISIBLE);
	displayRoom();
    
}

void attack(){
	

	
	
	battleMessage();
	VDP_drawTextBG(BG_B, "        ", 20, 6);
	VDP_drawTextBG(BG_B, "             ", 2, 22);
	//VDP_drawTextBG(BG_B, "       ", 3, 22);
	//deal damage to goblin
	s16 damage = ((random() % 10)*player_level)+player_attack;
	// damage formula
	damage  = (damage - (goblin_defense));
	if(damage < 0){
		damage = 0;
	}
	goblin_hp -= damage;
	VDP_drawTextBG( BG_B, "    ", 14, 6);

	sprintf(damageMessage, "%d", damage);
	VDP_drawTextBG( BG_B, "-" , 19, 12);
	VDP_drawTextBG(BG_B, damageMessage, 20, 12);
	sprintf(gHP, "%d", goblin_hp);
	for (int i = 0; i < 6; i++){
	delayFrames(5);
	}
	if(goblin_hp > 0){
	VDP_drawTextBG( BG_B, gHP, 14, 6);
	
	}
	
	
	
	
	if(goblin_hp > 0){
	goblinAttack();
	turn = !turn;

	}
	
	
	


}

void battleMessage(){

		char message[40];
		int randIndex = random() % 13; // We have 13 messages in the array
		strncpy(message, attack_message[randIndex], 20);
		VDP_drawTextBG(BG_B, "You ", 3, 2);
		VDP_drawTextBG(BG_B, message, 7, 2);
	
	
}
void goblinAttack(){

	s16 damage = ((random() % 10)*player_level) + goblin_attack;
	damage  = (damage - (player_defense));
	if(damage < 0){
		damage = 0;
	}

	player_hp -= damage;

	
	VDP_drawTextBG(BG_B, "                        ", 3, 2);
	VDP_drawTextBG(BG_B, "         ", 19, 12);
	VDP_drawTextBG(BG_B, "attacks!", 20, 6);


	sprintf(damageMessage, "%d", damage);
	VDP_drawTextBG(BG_B, damageMessage, 2, 22);
	VDP_drawTextBG(BG_B, "damage!", 5, 22);
		VDP_drawTextBG( BG_B, "        ", 8, 26);
		sprintf(pHP, "%d", player_hp);
	VDP_drawTextBG( BG_B, pHP, 8, 26);
	if(player_hp <= 0){
		player_hp = 0;
		VDP_drawTextBG(BG_B, "You died!", 2, 24);
		VDP_drawTextBG(BG_B, "Game Over", 2, 26);
		delayFrames(300);
		
		SYS_hardReset();

		
	}
	
	//delayFrames(120);
	turn = !turn;

}

void levelUp(){


	player_level++;
	player_hp_max += (player_level * 10);
	player_hp = player_hp_max;
	player_attack += 2;
	player_defense += 2;
	player_exp_needed = player_exp_needed * 2;
	//display level up message
	VDP_drawTextBG(BG_B, "You leveled up!", 2, 24);
	delayFrames(120);

}

void itemDrop(){
	u16 rand = random() % 100;
	if(rand < 25){
		rand = random() % 8;
		addItem(rand, 1);
		VDP_drawTextBG(BG_B, "You found ", 10, 20);
		switch (rand){
			case 0:
				VDP_drawTextBG(BG_B, "a skull", 10, 22);
				break;
			case 1:
				VDP_drawTextBG(BG_B, "some meat", 10, 22);
				break;
			case 2:
				VDP_drawTextBG(BG_B, "bones", 10, 22);
				break;
			case 3:
				VDP_drawTextBG(BG_B, "skin", 10, 22);
				break;
			case 4:
				VDP_drawTextBG(BG_B, "eyes", 10, 22);
				break;
			case 5:
				VDP_drawTextBG(BG_B, "fangs", 10, 22);
				break;
			case 6:
				VDP_drawTextBG(BG_B, "horn", 10, 22);
				break;
			case 7:
				VDP_drawTextBG(BG_B, "a tail", 10, 22);
				break;
		//delayFrames(120);
	}

}
}

