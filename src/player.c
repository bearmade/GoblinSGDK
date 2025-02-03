#include "../inc/debuginfo.h"
#include "../inc/level.h"
#include "../inc/camera.h"
#include "../inc/player.h"
#include "../inc/globals.h"
#include <resources.h>
#include "../inc/titlescreen.h"
#include "../inc/makemap.h"
#include "../inc/gamemanager.h"

Sprite* player;
Sprite* merchant;
u16 playerDir = 0;
fix32 playerPosX = FIX32(100);
fix32 playerPosY = FIX32(100);
fix32 playerSpeed = FIX32(0.85);
bool player_move_left = FALSE;
bool player_move_right = FALSE;
bool player_move_up = FALSE;
bool player_move_down = FALSE;
bool bIsMoving = FALSE;
int attack_timer = 0;
int attack_duration = 24;
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

fix32 tempPlayerPosX;
fix32 tempPlayerPosY;


#define SFX_SWOOSH 64



void displayPlayer(){

	PAL_setPalette(PAL2, our_sprite.palette->data, DMA);
	player = SPR_addSprite(&our_sprite, fix32ToInt(playerPosX), fix32ToInt(playerPosY), TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
//showMerchant();
//showMerchant();
//PAL_setPalette(PAL3, merchantSprite.palette->data, DMA);
//	merchant = SPR_addSprite(&merchantSprite,fix32ToInt(playerPosX), fix32ToInt(playerPosY), TILE_ATTR(PAL3, FALSE, FALSE, FALSE));


}
void joyEvent(u16 joy, u16 changed, u16 state){
	// if((changed & state & BUTTON_A) && (attack_timer == 0)){

	// 	player_move_down = FALSE;
	// 	player_move_up = FALSE;
	// 	player_move_left = FALSE;
	// 	player_move_right = FALSE;

	// 	if(playerDir == 0){
	// 		SPR_setAnim(player, ANIM_ATTACK_DOWN);
	// 		XGM_startPlayPCM(SFX_SWOOSH, 15, SOUND_PCM_CH2);
	// 		attack_timer += 1;
	// 	}
	// 	if(playerDir == 1){
	// 		SPR_setAnim(player, ANIM_ATTACK_RIGHT);
	// 		XGM_startPlayPCM(SFX_SWOOSH, 15, SOUND_PCM_CH2);
	// 		attack_timer += 1;

	// 	}
	// 	if(playerDir == 2){
	// 		SPR_setAnim(player, ANIM_ATTACK_LEFT);
	// 		XGM_startPlayPCM(SFX_SWOOSH, 15, SOUND_PCM_CH2);
	// 		attack_timer += 1;
	// 	}
	// 	if(playerDir == 3){
	// 		SPR_setAnim(player, ANIM_ATTACK_UP);
	// 		XGM_startPlayPCM(SFX_SWOOSH, 15, SOUND_PCM_CH2);
	// 		attack_timer += 1;
	// 	}
	// }
	if((changed & state & BUTTON_START)){
		showTitleScreen = FALSE;
		if(bBattleOngoing){
		//delayVBlank(120);
		
			if(selection == 1){
				int rand = random() % 100;
				char droppedGold[5];
				if(rand > 40){
					VDP_drawTextBG(BG_B, "You run like a scared child", 2, 2);
					delayFrames(120); 
					if (player_gold >= 10){
						rand = random() % 10;
						player_gold -= rand;
						VDP_drawTextBG(BG_B, "You dropped ", 2, 16);
						sprintf(droppedGold, "%d", rand);
						VDP_drawTextBG(BG_B, droppedGold, 15, 16);
						VDP_drawTextBG(BG_B, " gold", 17, 16);
						delayFrames(120);
						VDP_drawTextBG(BG_B, "                    ", 2, 16);

					}

					bBattleOngoing = FALSE;
					endBattle();
				}
				else{
				    VDP_drawTextBG(BG_B, "You failed to Run", 3, 2);
					delayFrames(120);
					goblinAttack();
					sprintf(pHP, "%d", player_hp);
					VDP_drawTextBG(BG_B, "    ", 8, 26);  // Clear old HP
					VDP_drawTextBG(BG_B, pHP, 8, 26);      // Draw new HP
					delayFrames(120);
					VDP_drawTextBG(BG_B, "        ", 20, 6);
					VDP_drawTextBG(BG_B, "             ", 2, 22);
					turn = !turn;
				}


			}
			else{
				
				if(turn){
					XGM_startPlayPCM(SFX_SWOOSH, 15, SOUND_PCM_CH2);
					attack();
					
				}
				else{
					
					//goblinAttack();
					
				}
				
			}
		}

		else{

			showStats();
		}

		
	}
			if(changed & state & BUTTON_B){
				if(bShowMerchMenu){
					bShowMerchMenu = FALSE;
					showMerchMenu();
					bPlayerCanMove = TRUE;
					SPR_setVisibility(merchant, VISIBLE);
					displayRoom();
				
			}else{
				if(!bBattleOngoing){
			 sramSave();
			 VDP_drawTextBG(BG_B, "Saved", 10, 10);
			 delayFrames(120);
			 VDP_drawTextBG(BG_B, "     ", 10, 10);
			 displayRoom();
		}}}
		if(changed & state & BUTTON_A){
			if(bShowMerchMenu){
				bPlayerCanMove = FALSE;
				showMerchMenu();
				//hide merchant
				SPR_setVisibility(merchant, HIDDEN);
			}
		}
	if((changed & state & BUTTON_DOWN)){
		selection = !selection;
	}
	if((changed & state & BUTTON_UP)){
		selection = !selection;
	}
	// if((changed & state & BUTTON_X)){
	// 	MapHasBeenMade = FALSE;
	// 	VDP_clearPlane(BG_A, TRUE);
	// 	VDP_clearPlane(BG_B, TRUE);
	// 	playerPosX = FIX32(100);
	// 	playerPosY = FIX32(100);
	// 	makeMap();

	// }
}

void handleInput(){
	u16 value = JOY_readJoypad( JOY_1);

if(bPlayerCanMove && !bShowMenu && !bInsideHouse){


	if(!(value & BUTTON_DOWN) && !(value & BUTTON_UP) && !(value & BUTTON_LEFT) && !(value & BUTTON_RIGHT) && (playerDir == 0)) SPR_setAnim(player, ANIM_IDLE_DOWN);
	if(!(value & BUTTON_DOWN) && !(value & BUTTON_UP) && !(value & BUTTON_LEFT) && !(value & BUTTON_RIGHT) && (playerDir == 1)) SPR_setAnim(player, ANIM_IDLE_RIGHT);
	if(!(value & BUTTON_DOWN) && !(value & BUTTON_UP) && !(value & BUTTON_LEFT) && !(value & BUTTON_RIGHT) && (playerDir == 2)) SPR_setAnim(player, ANIM_IDLE_LEFT);
	if(!(value & BUTTON_DOWN) && !(value & BUTTON_UP) && !(value & BUTTON_LEFT) && !(value & BUTTON_RIGHT) && (playerDir == 3)) SPR_setAnim(player, ANIM_IDLE_UP);

	if( value & BUTTON_LEFT){
		playerDir = 2;
		player_move_right = FALSE;
		player_move_left = TRUE;
		bIsMoving = TRUE;
		
		SPR_setAnim(player, ANIM_WALK_LEFT);
	}
	else if( value & BUTTON_RIGHT){
		playerDir = 1;
		player_move_right = TRUE;
		player_move_left = FALSE;
		bIsMoving = TRUE;
		

		SPR_setAnim(player, ANIM_WALK_RIGHT);
	}
	else {
		player_move_left = FALSE;
		
	
		player_move_right = FALSE;
		bIsMoving = FALSE;
	}

	if( value & BUTTON_DOWN){
		playerDir = 0;
		player_move_down = TRUE;
		player_move_up = FALSE;
		bIsMoving = TRUE;
		selection = !selection;
		

		SPR_setAnim(player, ANIM_WALK_DOWN);
	}
	else if( (value & BUTTON_UP)){
		playerDir = 3;
		player_move_down = FALSE;
		player_move_up = TRUE;
		bIsMoving = TRUE;
		selection = !selection;
		

		SPR_setAnim(player, ANIM_WALK_UP);
	}
	else {
		player_move_down = FALSE;
		player_move_up = FALSE;
		bIsMoving = FALSE;
	}

	

}


}

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
	switch (goblinType){
		case 0:
		goblinOffset = 0;
		break;
		case 1:
		goblinOffset = 37;
		break;
		case 2:
		goblinOffset = 37*2;
		break;
		case 3:
		goblinOffset = 37*3;
		break;
		case 4:
		goblinOffset = 37*4;
		break;
		case 5:
		goblinOffset = 37*5;
		break;
		case 6:
		goblinOffset = 37*6;
		break;

		}



}

void randomEncounter(){
	randChance = random() % 1000;
	if(randChance <= 5 && !isTransitioning){
		initBattle();
		turn = TRUE;
	}
}

void displayBattle(){
	
	//clear  screen

	//VDP_clearTileMap(BG_B, ind, 0, TRUE);

	
	if (bBattleStarted == TRUE){
	
	VDP_clearTileMap(BG_A, ind, 0, TRUE);
	bBattleOngoing = TRUE;
	//load font tiles
	VDP_loadFontData(tileset_Font.tiles, 96, CPU);
	//set font palette
	PAL_setPalette(PAL0, palette_Font.data, DMA);
	VDP_drawTextBG( BG_B, "Goblin Encounter!", 8, 0);
	delayFrames(120);
	VDP_drawTextBG( BG_B, "                 ", 8, 0);
	//draw text on window plane
	nameGenerator();
	VDP_drawText(Name, 5, 4);

	bBattleStarted = FALSE;
	//show battle menu
	
	VDP_drawTextBG( BG_B, ".Attack.", 22, 24);
	VDP_drawTextBG( BG_B, ".Run.", 22, 26);
	//sprintf(pointer, "%c", selection);
	
	
VDP_loadTileSet(goblin.tileset, 1, DMA);
    PAL_setPalette(PAL1, goblin.palette->data, DMA);


	}
	while(bBattleOngoing){
	
	
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
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 1 + goblinOffset), 20, 14);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 2+ goblinOffset), 21, 14);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 3+ goblinOffset), 22, 14);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 4+ goblinOffset), 23, 14);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 5+ goblinOffset), 24, 14);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 6+ goblinOffset), 25, 14);

			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 8+ goblinOffset), 20, 15);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 9+ goblinOffset), 21, 15);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 10+ goblinOffset), 22, 15);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 11+ goblinOffset), 23, 15);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 12+ goblinOffset), 24, 15);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 13+ goblinOffset), 25, 15);

			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 14+ goblinOffset), 20, 16);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 15+ goblinOffset), 21, 16);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 16+ goblinOffset), 22, 16);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 17+ goblinOffset), 23, 16);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 18+ goblinOffset), 24, 16);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 19+ goblinOffset), 25, 16);

			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 20+ goblinOffset), 20, 17);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 21+ goblinOffset), 21, 17);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 22+ goblinOffset), 22, 17);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 23+ goblinOffset), 23, 17);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 24+ goblinOffset), 24, 17);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 25+ goblinOffset), 25, 17);

			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 26+ goblinOffset), 20, 18);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 27+ goblinOffset), 21, 18);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 28+ goblinOffset), 22, 18);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 29+ goblinOffset), 23, 18);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 30+ goblinOffset), 24, 18);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 31+ goblinOffset), 25, 18);

			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 32+ goblinOffset), 20, 19);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 33+ goblinOffset), 21, 19);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 34+ goblinOffset), 22, 19);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 35+ goblinOffset), 23, 19);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 36+ goblinOffset), 24, 19);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 37+ goblinOffset), 25, 19);






			VDP_drawTextBG( BG_B, "HP: ", 10, 6);
			sprintf(gHP, "%d", goblin_hp);
			VDP_drawTextBG( BG_B, gHP, 14, 6);

		}
		else{
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

		}
		//get player selection
		VDP_drawTextBG( BG_B, " ", 21, ((selection == 0 ? 1 : 0)*2 + 24)); // Clear old cursor
		VDP_drawTextBG( BG_B, "~", 21, ((selection*2) + 24)); // Draw new cursor
		
		if(goblin_hp <= 0){
			goblinsKilled++;
			player_gold += goldDrop;
			
			player_exp += experience_gained;
			VDP_clearTileMap(BG_B, ind, 1, TRUE);
			if (player_exp >= player_exp_needed){
				levelUp();
				delayFrames(120);
			}

			delayFrames(120);
			bBattleOngoing = FALSE;
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

	displayRoom();
    
}

void attack(){
	
	XGM_startPlayPCM(SFX_SWOOSH, 15, SOUND_PCM_CH2);

	
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
	if(goblin_hp > 0){
	VDP_drawTextBG( BG_B, gHP, 14, 6);}
	XGM_startPlayPCM(SFX_SWOOSH, 15, SOUND_PCM_CH2);
	delayFrames(120);
	//turn = !turn;
	if(goblin_hp > 0){
	goblinAttack();
	turn = !turn;
	}
	
	



}

void battleMessage(){
	XGM_startPlayPCM(SFX_SWOOSH, 15, SOUND_PCM_CH2);
		char message[40];
		int randIndex = random() % 13; // We have 13 messages in the array
		strncpy(message, attack_message[randIndex], 20);
		VDP_drawTextBG(BG_B, "You ", 3, 2);
		VDP_drawTextBG(BG_B, message, 7, 2);
	
}
void goblinAttack(){
XGM_startPlayPCM(SFX_SWOOSH, 15, SOUND_PCM_CH2);
	s16 damage = ((random() % 10)*player_level) + goblin_attack;
	damage  = (damage - (player_defense));
	if(damage < 0){
		damage = 0;
	}

	player_hp -= damage;

	VDP_drawTextBG(BG_B, "                        ", 3, 2);
	VDP_drawTextBG(BG_B, "         ", 19, 12);
	VDP_drawTextBG(BG_B, "attacks!", 20, 6);
	VDP_drawTextBG( BG_B, "    ", 8, 26);
	VDP_drawTextBG( BG_B, pHP, 8, 26);

	sprintf(damageMessage, "%d", damage);
	VDP_drawTextBG(BG_B, damageMessage, 2, 22);
	VDP_drawTextBG(BG_B, "damage!", 5, 22);
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

void showStats(){
	bShowMenu = !bShowMenu;
	if(bShowMenu){
// Hide the player sprite
        SPR_setVisibility(player, HIDDEN);
	    SPR_setVisibility(merchant, HIDDEN);
		// tempPlayerPosX = playerPosX;
		// tempPlayerPosY = playerPosY;
		// playerPosX = 100;
		// playerPosY = 16;
		//clear screen
		VDP_clearTileMap(BG_B, ind, 0, TRUE);
		//load font tiles
		VDP_loadFontData(tileset_Font.tiles, 96, CPU);
		//set font palette
		PAL_setPalette(PAL0, palette_Font.data, DMA);

	//show player stats
	VDP_drawTextBG(BG_B, "Player", 3, 2);
	VDP_drawTextBG(BG_B, "HP: ", 3, 4);
	sprintf(pHP, "%d", player_hp);
	VDP_drawTextBG(BG_B, pHP, 8, 4);
	VDP_drawTextBG(BG_B, "/", 12, 4);
	sprintf(pHPMax, "%d", player_hp_max);
	VDP_drawTextBG(BG_B, pHPMax, 14, 4);
	VDP_drawTextBG(BG_B, "LVL: ", 3, 6);
	sprintf(pLevel, "%d", player_level);
	VDP_drawTextBG(BG_B, pLevel, 16, 6);
	VDP_drawTextBG(BG_B, "ATK: ", 3, 8);
	sprintf(pAttack, "%d", player_attack);
	VDP_drawTextBG(BG_B, pAttack, 16, 8);
	VDP_drawTextBG(BG_B, "DEF: ", 3, 10);
	sprintf(pDefense, "%d", player_defense);
	VDP_drawTextBG(BG_B, pDefense, 16, 10);
	VDP_drawTextBG(BG_B, "EXP: ", 3, 12);
	sprintf(pExp, "%d", player_exp);
	VDP_drawTextBG(BG_B, pExp, 16, 12);
	VDP_drawTextBG(BG_B, "Gold: ", 3, 14);
	sprintf(pGold, "%d", player_gold);
	VDP_drawTextBG(BG_B, pGold, 16, 14);
	VDP_drawTextBG(BG_B, "Goblin Kills: ", 3, 16);
	sprintf(goblinsKilledChar, "%d", goblinsKilled);
	VDP_drawTextBG(BG_B, goblinsKilledChar, 16, 16);
	displayMiniMap();

	}
	else{
		   // Show the player sprite
		   
		   
        SPR_setVisibility(player, VISIBLE);
		       if (currentWorldX == merchWorldX && currentWorldY == merchWorldY) {

		SPR_setVisibility(merchant, VISIBLE);
	} else {

		SPR_setVisibility(merchant, HIDDEN);
	}

		//redraw screen
	// 		playerPosX = tempPlayerPosX;
	// playerPosY = tempPlayerPosY;
VDP_clearPlane(BG_B, TRUE);
	VDP_clearPlane(BG_A, TRUE);
	VDP_loadTileSet(tileset1.tileset, 1, DMA);
    PAL_setPalette(PAL1, tileset1.palette->data, DMA);
	PAL_setPalette(PAL0, fg2.palette->data, DMA);
	PAL_setPalette(PAL3,palette_Font.data, DMA);
	// clear all battle messages
	VDP_drawTextBG(BG_B, "      ", 4, 2);
	VDP_drawTextBG(BG_B, "      ", 4, 4);
	VDP_drawTextBG(BG_B, "      ", 8, 4);
	VDP_drawTextBG(BG_B, "      ", 4, 6);
	VDP_drawTextBG(BG_B, "      ", 8, 6);
	VDP_drawTextBG(BG_B, "      ", 4, 8);
	VDP_drawTextBG(BG_B, "      ", 8, 8);
	VDP_drawTextBG(BG_B, "      ", 4, 10);
	VDP_drawTextBG(BG_B, "      ", 8, 10);
	VDP_drawTextBG(BG_B, "      ", 4, 12);
	VDP_drawTextBG(BG_B, "      ", 8, 12);

	SYS_doVBlankProcess();

	PAL_setPalette(PAL3, merchantSprite.palette->data, DMA);
	displayRoom();
	//hide player stats


	}

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

void sramSave(){
	//save player stats
	SRAM_enable();
	SRAM_writeWord(0, player_hp);
	SRAM_writeWord(2, player_hp_max);
	SRAM_writeWord(4, player_level);
	SRAM_writeWord(6, player_attack);
	SRAM_writeWord(8, player_defense);
	SRAM_writeWord(10, player_exp);
	SRAM_writeWord(12, player_exp_needed);
	SRAM_writeWord(14, player_gold);
	SRAM_writeWord(16, goblinsKilled);
	SRAM_writeLong(18, worldSeed);
	SRAM_disable();
	//SRAM_writeWord(18, player_posX);
	//SRAM_writeWord(20, player_posY);

	


}

void sramLoad(){

	//load player stats
	SRAM_enable();
	player_hp = SRAM_readWord(0);
	player_hp_max = SRAM_readWord(2);
	player_level = SRAM_readWord(4);
	player_attack = SRAM_readWord(6);
	player_defense = SRAM_readWord(8);
	player_exp = SRAM_readWord(10);
	player_exp_needed = SRAM_readWord(12);
	player_gold = SRAM_readWord(14);
	goblinsKilled = SRAM_readWord(16);
	worldSeed = SRAM_readLong(18);
	SRAM_disable();
	//player_posX = SRAM_readWord(18);
	//player_posY = SRAM_readWord(20);

}
void showMerchMenu(){
	//bShowMerchMenu = !bShowMerchMenu;
	if(bShowMerchMenu){
		//hide player sprite
		
		SPR_setVisibility(player, HIDDEN);
		//clear screen
		VDP_clearTileMap(BG_B, ind, 0, TRUE);
		//load font tiles
		VDP_loadFontData(tileset_Font.tiles, 96, CPU);
		//set font palette
		PAL_setPalette(PAL0, palette_Font.data, DMA);
		//show merchant menu
		VDP_drawTextBG(BG_B, "Merchant", 4, 2);
		


	}
	else{
		//show player sprite
		SPR_setVisibility(player, VISIBLE);
		//redraw screen
		
		VDP_loadTileSet(tileset1.tileset, 1, DMA);
		PAL_setPalette(PAL1, tileset1.palette->data, DMA);
		PAL_setPalette(PAL0, fg2.palette->data, DMA);
		//PAL_setPalette(PAL3,palette_Font.data, DMA);
		//clear all battle messages
		VDP_drawTextBG(BG_B, "      ", 4, 2);
		VDP_drawTextBG(BG_B, "      ", 4, 4);
		VDP_drawTextBG(BG_B, "      ", 8, 4);
		VDP_drawTextBG(BG_B, "      ", 4, 6);
		displayRoom();
		//`VDP_drawTextBG(BG_B, "      ",
	}
}
void displayMiniMap(){
	//display mini map using font tiles
	
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			if (i  == currentWorldY && j == currentWorldX){
				VDP_drawTextBG(BG_B, "%", 22 + j, 6 + i);
			}
			else if (i == merchWorldY && j == merchWorldX){
				VDP_drawTextBG(BG_B, "&", 22 + j, 6 + i);
			}
			else{
				VDP_drawTextBG(BG_B, "#", 22 + j, 6 + i);
			}
			// if(WORLD_LAYOUT[i][j] == 1){
			// 	VDP_drawTextBG(BG_B, " ", 20 + j, 20 + i);
			// }
			// else{
			// 	VDP_drawTextBG(BG_B, ".", 20 + j, 20 + i);
			// }
		}
	}

}

