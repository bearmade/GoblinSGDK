#include "../inc/debuginfo.h"
#include "../inc/level.h"
#include "../inc/player.h"
#include "../inc/globals.h"
#include <resources.h>
#include "../inc/titlescreen.h"
#include "../inc/makemap.h"
#include "../inc/gamemanager.h"

Sprite* player;
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

char attack_message[8][20] = {
	"throw your shoe at  ",
	"clumsily trip into  ",
	"toss pocket lint at ",
	"yell very loudly at ",
	"perform nose boop on",
	"throw a rock towards",
	"attempt to kick     ",
	"do a barrel roll at " 
};
char damageMessage[8];

//player stats
s16 player_hp = 100;
u16 player_attack = 5;
u16 player_defense = 5;
u16 player_gold = 0;
u16 player_level = 1;
u16 player_exp = 0;
u16 player_exp_needed = 10;

//goblin stats
s16 goblin_hp = 0;
u16 goblin_attack = 0;
u16 goblin_defense = 0;

char pHP[5];
char pATK[5];
char gHP[5];
char gATK[5];


#define SFX_SWOOSH 64



void displayPlayer(){

	PAL_setPalette(PAL2, our_sprite.palette->data, DMA);
	player = SPR_addSprite(&our_sprite, fix32ToInt(playerPosX), fix32ToInt(playerPosY), TILE_ATTR(PAL2, FALSE, FALSE, FALSE));


}
void joyEvent(u16 joy, u16 changed, u16 state){
	if((changed & state & BUTTON_A) && (attack_timer == 0)){

		player_move_down = FALSE;
		player_move_up = FALSE;
		player_move_left = FALSE;
		player_move_right = FALSE;

		if(playerDir == 0){
			SPR_setAnim(player, ANIM_ATTACK_DOWN);
			XGM_startPlayPCM(SFX_SWOOSH, 15, SOUND_PCM_CH2);
			attack_timer += 1;
		}
		if(playerDir == 1){
			SPR_setAnim(player, ANIM_ATTACK_RIGHT);
			XGM_startPlayPCM(SFX_SWOOSH, 15, SOUND_PCM_CH2);
			attack_timer += 1;

		}
		if(playerDir == 2){
			SPR_setAnim(player, ANIM_ATTACK_LEFT);
			XGM_startPlayPCM(SFX_SWOOSH, 15, SOUND_PCM_CH2);
			attack_timer += 1;
		}
		if(playerDir == 3){
			SPR_setAnim(player, ANIM_ATTACK_UP);
			XGM_startPlayPCM(SFX_SWOOSH, 15, SOUND_PCM_CH2);
			attack_timer += 1;
		}
	}
	if((changed & state & BUTTON_START)){
		showTitleScreen = FALSE;
		if(bBattleOngoing){
		//delayVBlank(120);
		
			if(selection == 1){
				bBattleOngoing = FALSE;
				endBattle();
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

if(bPlayerCanMove){


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
	goblin_hp = (player_level * 5)+ random() % 25;



}

void randomEncounter(){
	randChance = random() % 1000;
	if(randChance <= 5){
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
	VDP_drawText(Name, 10, 4);

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


	    //show goblin stats
		if (goblin_hp > 0){
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 1), 20, 14);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 2), 21, 14);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 3), 22, 14);
			
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 5), 20, 15);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 6), 21, 15);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 7), 22, 15);

			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 8), 20, 16);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 9), 21, 16);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 10), 22, 16);

			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 11), 20, 17);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 12), 21, 17);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 13), 22, 17);

			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 14), 20, 18);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 15), 21, 18);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 16), 22, 18);

			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 17), 20, 19);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 18), 21, 19);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 19), 22, 19);

			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 20), 20, 20);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 21), 21, 20);
			VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 22), 22, 20);





			VDP_drawTextBG( BG_B, "HP: ", 10, 6);
			sprintf(gHP, "%d", goblin_hp);
			VDP_drawTextBG( BG_B, gHP, 14, 6);
		}
		else{
			VDP_clearTileMap(BG_B, ind, 1, TRUE);
			VDP_drawTextBG(BG_B, "        ", 19, 8);
			VDP_drawTextBG( BG_B, "is DEAD", 10, 6);

		}
		//get player selection
		VDP_drawTextBG( BG_B, " ", 21, ((selection == 0 ? 1 : 0)*2 + 24)); // Clear old cursor
		VDP_drawTextBG( BG_B, "~", 21, ((selection*2) + 24)); // Draw new cursor
		
		if(goblin_hp <= 0){
			VDP_clearTileMap(BG_B, ind, 1, TRUE);
			delayFrames(120);
			bBattleOngoing = FALSE;
			endBattle();
		}

		
		
		SYS_doVBlankProcess();


		
			}
	
	


}
void nameGenerator(){

    char firstName1[5][2] = {"Sn", "Sk", "Kr", "Br", "Gr"};
    char firstName2[5][2] = {"ar", "um", "ay", "ee", "oo"};
    char firstName3[5][3] = {"po ", "py ", "bs ", "ble", "gle"};
    char middleName[5][3] = {"Tum", "Bum", "Rub", "Hum", "Gru"};
    char lastName1[5][4] = {"thum", "bopo", "arum", "atum", "abum"};
    char lastName2[5][4] = {"lo  ", "bles", "gles", "po  ", "py  "};
    u16 rand = random() % 5;
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
	VDP_drawTextBG( BG_B, "                 ", 3, 2);
	VDP_drawTextBG( BG_B, "                 ", 7, 2);
	VDP_drawTextBG( BG_B, "                 ", 14, 6);
	VDP_drawTextBG( BG_B, "                 ", 2, 24);
	VDP_drawTextBG( BG_B, "                 ", 2, 26);
	SYS_doVBlankProcess();

	displayRoom();
    
}

void attack(){
	
	XGM_startPlayPCM(SFX_SWOOSH, 15, SOUND_PCM_CH2);

	
	battleMessage();
	VDP_drawTextBG(BG_B, "        ", 20, 8);
	VDP_drawTextBG(BG_B, "       ", 6, 20);
	VDP_drawTextBG(BG_B, "       ", 3, 22);
	//deal damage to goblin
	u16 damage = random() % 10;
	goblin_hp -= damage;
	VDP_drawTextBG( BG_B, "    ", 14, 6);

	sprintf(damageMessage, "%d", damage);
	VDP_drawTextBG( BG_B, "-" , 19, 8);
	VDP_drawTextBG(BG_B, damageMessage, 20, 8);
	sprintf(gHP, "%d", goblin_hp);
	if(goblin_hp >= 0){
	VDP_drawTextBG( BG_B, gHP, 14, 6);}
	XGM_startPlayPCM(SFX_SWOOSH, 15, SOUND_PCM_CH2);
	delayFrames(120);
	//turn = !turn;
	if(goblin_hp >= 0){
	goblinAttack();
	turn = !turn;
	}
	
	



}

void battleMessage(){
	XGM_startPlayPCM(SFX_SWOOSH, 15, SOUND_PCM_CH2);
		char message[40];
		int randIndex = random() % 8; // We have 8 messages in the array
		strncpy(message, attack_message[randIndex], 20);
		VDP_drawTextBG(BG_B, "You ", 3, 2);
		VDP_drawTextBG(BG_B, message, 7, 2);
	
}
void goblinAttack(){
XGM_startPlayPCM(SFX_SWOOSH, 15, SOUND_PCM_CH2);
	u16 damage = random() % 10;
	player_hp -= damage;
	VDP_drawTextBG(BG_B, "                        ", 3, 2);
	VDP_drawTextBG(BG_B, "         ", 19, 8);
	VDP_drawTextBG(BG_B, "attacks!", 20, 8);
	VDP_drawTextBG( BG_B, "    ", 8, 26);

	sprintf(damageMessage, "%d", damage);
	VDP_drawTextBG(BG_B, damageMessage, 6, 20);
	VDP_drawTextBG(BG_B, "damage!", 3, 22);
	//delayFrames(120);
	turn = !turn;

}