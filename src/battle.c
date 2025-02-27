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
bool isAnimating = FALSE;
u16 battleAnimationTimer = 0;


//#define SFX_SWOOSH 64

void initBattle(){
			turn = TRUE;
			bPlayerCanMove = FALSE;
			bBattleStarted = TRUE;
			// Reset movement state
			player_move_left = FALSE;
			player_move_right = FALSE;
			player_move_up = FALSE;
			player_move_down = FALSE;
			bIsMoving = FALSE;
    
			// Reset battle animation state
			isAnimating = FALSE;
			battleAnimationTimer = 0;

			// Calculate new goblin stats
			goblin_hp = (player_level * 10) + random() % 25;
			goblin_attack = (player_level * 2) + (random() % 10);
			goblin_defense = (player_level * 2) + random() % 10;
			goldDrop = (goblin_attack + goblin_defense) + random() % 15;
			goblinType = random() % 7;
    
			PAL_setPalette(PAL1, goblinSprite.palette->data, DMA);
			goblin_sprite = SPR_addSprite(&goblinSprite, 160, 120, TILE_ATTR(PAL1, FALSE, FALSE, FALSE));
			// Establish goblin animation according to type...
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
	if(randChance <= 2 && !isTransitioning){
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
	SPR_setVisibility(merchant, HIDDEN);
	SPR_setVisibility(goblin_sprite, VISIBLE);
	if (bBattleStarted == TRUE){
	bBattleMessageDone = FALSE;
	VDP_clearTileMap(BG_A, ind, 0, TRUE);
	bBattleOngoing = TRUE;
	//load font tiles
	VDP_loadFontData(tileset_Font.tiles, 96, CPU);
	//set font palette
	PAL_setPalette(PAL0, palette_Font.data, DMA);
	drawBox(4, 1, 24, 3);
	VDP_drawTextBG( BG_A, "A Wild Goblin Appears!", 5, 2);
	delayFrames(360);
	bBattleMessageDone = TRUE;

	VDP_drawTextBG( BG_A, "                      ", 5, 2);
	//draw text on window plane
	nameGenerator();
	VDP_drawTextBG(BG_A, Name, 5, 4);

	bBattleStarted = FALSE;
	//show battle menu
	
	VDP_drawTextBG( BG_A, ".Attack.", 22, 24);
	VDP_drawTextBG( BG_A, ".Run.", 22, 26);
	//sprintf(pointer, "%c", selection);
	
	
VDP_loadTileSet(goblin.tileset, 1026, DMA);
    PAL_setPalette(PAL1, goblin.palette->data, DMA);
	u16 randoBackdrop = random() % 2;
	switch (randoBackdrop){
		case 0:
		
		PAL_setPalette(PAL2, battleBack.palette->data, DMA);
		VDP_drawImageEx(BG_B, &battleBack, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ind), 0, 0, FALSE, TRUE);
		break;
		case 1:
		
		PAL_setPalette(PAL2, battleBack2.palette->data, DMA);
		VDP_drawImageEx(BG_B, &battleBack2, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ind), 0, 0, FALSE, TRUE);
		break;
	}
	// PAL_setPalette(PAL2, battleBack.palette->data, DMA);
	//     VDP_drawImageEx(BG_A, &battleBack, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ind), 0, 0, FALSE, TRUE);




	}
	while(bBattleOngoing){
		SPR_update();
		updateBattleAnimation();
	
		VDP_drawText(".Attack.", 22, 24);
		VDP_drawTextBG(BG_A, ".Run.", 22, 26);

		VDP_drawTextBG(BG_A, " ", 21, ((selection == 0 ? 1 : 0)*2 + 24)); // Clear old cursor
		//VDP_clearText(BG_A, 21,((selection == 0 ? 1 : 0)*2 + 24));
		VDP_drawTextBG(BG_A, "~", 21, ((selection*2) + 24));     // Draw new cursor

		VDP_drawTextBG( BG_A, "                 ", 4, 0);
		drawBox(1, 23, 30, 5);
		drawBox(2, 1, 26, 6);
		drawBox(9, 7, 10, 7);
		//show player stats
		VDP_drawTextBG( BG_A, player_name, 2, 24);
		VDP_drawTextBG( BG_A, "HP:", 2, 26);
		sprintf(pHP, "%d", player_hp);
		VDP_drawTextBG( BG_A, pHP, 8, 26);
	VDP_drawTextBG( BG_A, "/", 12, 26);
	sprintf(pHPMax, "%d", player_hp_max);
	VDP_drawTextBG( BG_A, pHPMax, 14, 26);
	VDP_drawTextBG( BG_A, "HP: ", 11, 8);
	VDP_drawTextBG( BG_A, "    ", 15, 8);  
	sprintf(gHP, "%d", goblin_hp);
	
	VDP_drawTextBG( BG_A, gHP, 15, 8);
	VDP_drawTextBG( BG_A, "ATK: ", 11, 10);
	sprintf(gATK, "%d", goblin_attack);
	VDP_drawTextBG( BG_A, gATK, 15, 10);
	VDP_drawTextBG( BG_A, "DEF: ", 11, 12);
	sprintf(gDEF, "%d", goblin_defense);
	VDP_drawTextBG( BG_A, gDEF, 15, 12);

		//show goblin stats
		if (goblin_hp > 0){






			// VDP_drawTextBG( BG_A, "HP: ", 10, 6);
			// sprintf(gHP, "%d", goblin_hp);
			// VDP_drawTextBG( BG_A, gHP, 14, 6);

		}
		else{
			bBattleMessageDone = FALSE;
			experience_gained = (player_level + random() % 2);
			VDP_clearTileMap(BG_A, ind, 1, TRUE);
			VDP_drawTextBG(BG_A, "        ", 19, 8);
			VDP_drawTextBG( BG_A, "        ", 19, 12);
			//VDP_drawTextBG( BG_A, "is DEAD", 10, 6);
			VDP_drawTextBG( BG_A, "        ", 10, 8);
			VDP_drawTextBG( BG_A, "        ", 10, 10);
			VDP_drawTextBG( BG_A, "        ", 10, 12);
			drawBox(9, 11, 10, 9);
			VDP_drawTextBG(BG_A, "         ", 23, 20);
			SPR_setVisibility(goblin_sprite, HIDDEN);
			SPR_update();
			VDP_drawTextBG( BG_A, "Found", 10, 12);
			sprintf(gold, "%d", goldDrop);
			VDP_drawTextBG( BG_A, gold, 16, 12);
			VDP_drawTextBG( BG_A, "Gold!", 10, 14);
			VDP_drawTextBG( BG_A, "Gained", 10, 16);
			VDP_drawTextBG( BG_A, "Exp!", 10, 18);
			sprintf(expChar, "%d", experience_gained);
			VDP_drawTextBG( BG_A, expChar, 16, 18);
			itemDrop();

		}
	
		if(goblin_hp <= 0){

			bBattleMessageDone = FALSE;
			goblinsKilled++;
			player_gold += goldDrop;
			XGM_startPlay(victory_vgm);
		
			player_exp += experience_gained;
			VDP_clearTileMap(BG_A, ind, 1, TRUE);
			if (player_exp >= player_exp_needed){
				XGM_stopPlay();
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
		// Release the goblin sprite so we can create a fresh one next battle.
		if(goblin_sprite != NULL) {
				SPR_releaseSprite(goblin_sprite);
				goblin_sprite = NULL;
		}
	
		VDP_clearPlane(BG_B, TRUE);
		VDP_clearPlane(BG_A, TRUE);
		VDP_loadTileSet(tileset1.tileset, 1, DMA);
		PAL_setPalette(PAL1, tileset1.palette->data, DMA);
		PAL_setPalette(PAL0, fg2.palette->data, DMA);
		PAL_setPalette(PAL3,palette_Font.data, DMA);
		// clear all battle messages
		VDP_drawTextBG( BG_A, "                 ", 3, 10);
		VDP_drawTextBG( BG_A, "                 ", 5, 4);
		VDP_drawTextBG( BG_A, "                 ", 3, 2);
		VDP_drawTextBG( BG_A, "                 ", 7, 2);
		VDP_drawTextBG( BG_A, "                 ", 14, 6);
		VDP_drawTextBG( BG_A, "                 ", 2, 24);
		VDP_drawTextBG( BG_A, "                 ", 2, 26);
		PAL_setPalette(PAL3, merchantSprite.palette->data, DMA);
		SYS_doVBlankProcess();
		XGM_stopPlay();
		XGM_startPlay(world_vgm);
		PAL_setPalette(PAL2, our_sprite.palette->data, DMA);
		SPR_setVisibility(player, VISIBLE);
		displayRoom();
}
void attack(){
	

	//VDP_clearTileMapRect(BG_B, 1, 21, 15, 2);
	
	battleMessage();
	VDP_drawTextBG(BG_A, "        ", 19, 5);
	VDP_drawTextBG(BG_A, "             ", 2, 22);
	VDP_drawTextBG(BG_A, "             ", 4, 21);
	//VDP_drawTextBG(BG_B, "       ", 3, 22);
	//deal damage to goblin
	s16 damage = ((random() % 10)*player_level)+player_attack;
	// damage formula
	damage  = (damage - (goblin_defense));
	if(damage < 0){
		damage = 0;
	}
	goblin_hp -= damage;
	VDP_drawTextBG( BG_A, "    ", 14, 6);

	sprintf(damageMessage, "%d", damage);
	VDP_drawTextBG( BG_A, "-" , 23, 19);
	VDP_drawTextBG(BG_A, damageMessage, 24, 19);
	waitMs(100);
	VDP_drawTextBG(BG_A, "         ", 23, 19);
	VDP_drawTextBG( BG_A, "-" , 23, 18);
	VDP_drawTextBG(BG_A, damageMessage, 24, 18);
	waitMs(100);
	VDP_drawTextBG(BG_A, "         ", 23, 18);
	VDP_drawTextBG( BG_A, "-" , 23, 19);
	VDP_drawTextBG(BG_A, damageMessage, 24, 19);
	waitMs(100);
	VDP_drawTextBG(BG_A, "         ", 23, 19);
	VDP_drawTextBG( BG_A, "-" , 23, 20);
	VDP_drawTextBG(BG_A, damageMessage, 24, 20);
	//waitMs(2000);
	isAnimating = TRUE;
	
	updateBattleAnimation();
	sprintf(gHP, "%d", goblin_hp);

	// for(int i = 0; i < 120; i++){
	// 	SYS_doVBlankProcess();
	// }
	//delayFrames(120);

	if(goblin_hp > 0){
	VDP_drawTextBG( BG_A, gHP, 14, 8);
	
	}
	
	
	
	
	if(goblin_hp > 0 ){
	goblinAttack();
	turn = !turn;

	}
	
	
	


}

void battleMessage(){
		VDP_drawTextBG(BG_A, "                        ", 3, 2);
		char message[40];
		int randIndex = random() % 13; // We have 13 messages in the array
		strncpy(message, attack_message[randIndex], 20);
		VDP_drawTextBG(BG_A, "You ", 3, 2);
		VDP_drawTextBG(BG_A, message, 7, 2);
	
	
}
void goblinAttack(){
	

	s16 damage = ((random() % 5)*player_level) + goblin_attack;
	damage  = (damage - (player_defense));
	if(damage < 0){
		damage = 0;
	}
	//if(isAnimating){
		

	
	player_hp -= damage;
	

	
	//VDP_drawTextBG(BG_A, "                        ", 3, 2);
	VDP_drawTextBG(BG_A, "         ", 19, 12);
	
	VDP_drawTextBG(BG_A, "attacks!", 19, 5);


	sprintf(damageMessage, "%d", damage);
	drawBox(1, 20, 15, 3);
	VDP_drawTextBG(BG_A, damageMessage, 4, 21);
	VDP_drawTextBG(BG_A, "damage!", 7, 21);
		VDP_drawTextBG( BG_A, "        ", 8, 26);
		sprintf(pHP, "%d", player_hp);
	VDP_drawTextBG( BG_A, pHP, 8, 26);
	if(player_hp <= 0){
		player_hp = 0;
		VDP_drawTextBG(BG_A, "You died!", 2, 24);
		VDP_drawTextBG(BG_A, "Game Over", 2, 26);
		delayFrames(300);
		//show gameOverScreen
		VDP_clearTileMap(BG_B, ind, 1, TRUE);
		VDP_clearTileMap(BG_A, ind, 1, TRUE);
		SPR_setVisibility(goblin_sprite, HIDDEN);
		//clear all lines of text
		for(int i = 0; i < 28; i++){
			VDP_clearTextLine(i);
		}

		PAL_setPalette(PAL2, gameOverScreen.palette->data, DMA);
		VDP_drawImageEx(BG_B, &gameOverScreen, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ind), 0, 0, FALSE, TRUE);
		SPR_setVisibility(goblin_sprite, HIDDEN);
		SPR_releaseSprite(goblin_sprite);
		SPR_update();
		delayFrames(600);

		SYS_hardReset();

		
	}
	
	//delayFrames(120);
	//turn = !turn;
	//}
}

void levelUp(){


	player_level++;
	player_hp_max += (player_level * 10);
	player_hp = player_hp_max;
	player_attack += 2;
	player_defense += 2;
	player_exp_needed = player_exp_needed * 2;
	//display level up message
	drawBox(3,24, 17, 3);
	VDP_drawTextBG(BG_A, "                 ", 1, 23);
	VDP_drawTextBG(BG_A, "                 ", 1, 24);
	VDP_drawTextBG(BG_A, "                 ", 1, 25);
	VDP_drawTextBG(BG_A, "                 ", 1, 26);
	VDP_drawTextBG(BG_A, "You leveled up!", 4, 25);
	delayFrames(120);

}

void itemDrop(){
	u16 rand = random() % 100;
	if(rand < 25){
		rand = random() % 6;
		addItem(rand, 1);
		VDP_clearTextLine(21);
		drawBox(9, 19, 11, 5);
		VDP_drawTextBG(BG_A, "You found ", 10, 20);
		switch (rand){
			case 0:
				VDP_drawTextBG(BG_A, "a skull", 10, 22);
				break;
			case 1:
				VDP_drawTextBG(BG_A, "some meat", 10, 22);
				break;
			case 2:
				VDP_drawTextBG(BG_A, "bones", 10, 22);
				break;
			case 3:
				VDP_drawTextBG(BG_A, "horn", 10, 22);
				break;
			case 4:
				VDP_drawTextBG(BG_A, "eyes", 10, 22);
				break;
			case 5:
				VDP_drawTextBG(BG_A, "fangs", 10, 22);
				break;



		//delayFrames(120);
	}

}
}

void drawBox(u16 x, u16 y, u16 width, u16 height) {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			// Fill center
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL0, 1, 0, 0, 1470), x+i, y+j);
			
			// Top edge
			if (j == 0) VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL0, 1, 0, 0, 1468), x+i, y);
			
			// Bottom edge - vertically flipped
			if (j == height-1) VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL0, 1, 1, 0, 1468), x+i, y+j);
			
			// Left edge
			if (i == 0) VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL0, 1, 0, 0, 1469), x, y+j);
			
			// Right edge - horizontally flipped
			if (i == width-1) VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL0, 1, 0, 1, 1469), x+i, y+j);
			
			// Corners
			if (j == 0 && i == 0) VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL0, 1, 0, 0, 1467), x, y);
			if (j == 0 && i == width-1) VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL0, 1, 0, 1, 1467), x+i, y);
			if (j == height-1 && i == 0) VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL0, 1, 1, 0, 1467), x, y+j);
			if (j == height-1 && i == width-1) VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL0, 1, 1, 1, 1467), x+i, y+j);
			//add drop shadow to bottom and right using "^"
			if (j == height-1) VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL0, 0, 0, 0, 1502), x+i+1, y+j+1);
			if (i == width-1) VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL0, 0, 0, 0, 1502), x+i+1, y+j+1);

			
		}
	}
}


void updateBattleAnimation() {
    if (isAnimating) {
        battleAnimationTimer++;
        
        // Flash every 5 frames
        if (battleAnimationTimer & 4) {
            SPR_setVisibility(goblin_sprite, HIDDEN);
        } else {
            SPR_setVisibility(goblin_sprite, VISIBLE);
        }
        
        // End after 60 frames (1 second)
        if (battleAnimationTimer >= 60) {
            isAnimating = FALSE;
            battleAnimationTimer = 0;
			if(bBattleOngoing){
				SPR_setVisibility(goblin_sprite, VISIBLE);
				VDP_drawTextBG(BG_A, "         ", 23, 20);
				VDP_drawTextBG(BG_A, "         ", 19, 5);
			}
			else{



				SPR_setVisibility(goblin_sprite, HIDDEN);
			}
            //VDP_drawTextBG(BG_A, "         ", 23, 20);
            turn = !turn;
        }
    }
}
