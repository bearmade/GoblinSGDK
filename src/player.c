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



}

void randomEncounter(){
	randChance = random() % 1000;
	if(randChance <= 5){
		initBattle();
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
	VDP_drawTextBG( BG_B, "Goblin Encounter!", 4, 2);
	//draw text on window plane
	nameGenerator();
	VDP_drawText(Name, 4, 4);

	bBattleStarted = FALSE;
	//show battle menu
	
	VDP_drawTextBG( BG_B, ".Attack.", 6, 8);
	VDP_drawTextBG( BG_B, ".Run.", 6, 10);
	//sprintf(pointer, "%c", selection);
	
	


	}
	while(bBattleOngoing){
		
		//get player selection
		VDP_drawTextBG( BG_B, "~", 5, ((selection*2 )+ 8));
		//delayVBlank(5);
		//VDP_drawTextBG( BG_B, " ", 5, ((selection*2 )+ 8));
		//clear part of screen
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
