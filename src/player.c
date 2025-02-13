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
fix32 tempPlayerPosX;
fix32 tempPlayerPosY;

void displayPlayer(){

	PAL_setPalette(PAL2, our_sprite.palette->data, DMA);
	player = SPR_addSprite(&our_sprite, fix32ToInt(playerPosX), fix32ToInt(playerPosY), TILE_ATTR(PAL2, FALSE, FALSE, FALSE));


}
void joyEvent(u16 joy, u16 changed, u16 state){

	if((changed & state & BUTTON_A)){
		showTitleScreen = FALSE;
		if(bBattleOngoing){
		//delayVBlank(120);
		if(bBattleMessageDone){
			if(selection == 1){
				int rand = random() % 100;
				char droppedGold[5];
				if(rand > 40){
					drawBox(1, 1, 29, 3);
					VDP_drawTextBG(BG_A, "You run like a scared child", 2, 2);
					delayFrames(120); 
					if (player_gold >= 10){
						rand = random() % 10;
						player_gold -= rand;
						drawBox(1, 15, 23, 3);
						VDP_drawTextBG(BG_A, "You dropped ", 2, 16);
						sprintf(droppedGold, "%d", rand);
						VDP_drawTextBG(BG_A, droppedGold, 15, 16);
						VDP_drawTextBG(BG_A, " gold", 17, 16);
						delayFrames(120);
						VDP_drawTextBG(BG_A, "                    ", 2, 16);

					}

					bBattleOngoing = FALSE;
					bBattleMessageDone = FALSE;
					endBattle();
				}
				else{
				    VDP_drawTextBG(BG_A, "You failed to Run", 3, 2);
					delayFrames(120);
					goblinAttack();
					sprintf(pHP, "%d", player_hp);
					VDP_drawTextBG(BG_A, "    ", 8, 26);  // Clear old HP
					VDP_drawTextBG(BG_A, pHP, 8, 26);      // Draw new HP
					delayFrames(120);
					VDP_drawTextBG(BG_A, "        ", 20, 6);
					VDP_drawTextBG(BG_A, "             ", 2, 22);
					turn = !turn;
				}


			}
			else{
				
				if(turn){
					//XGM_startPlayPCM(SFX_SWOOSH, 15, SOUND_PCM_CH2);
					attack();
					
				}
				else{
								
				}
				
			}
		}
		}
		// else if(!bBattleOngoing && !bBattleMessageDone && !bIsMoving){
		// 	//bIsMoving = FALSE;
		// 	bPlayerCanMove = FALSE;
		// 	showStats();
		// }		
	}
			if(changed & state & BUTTON_C){
				if(bShowMerchMenu){
					bShowMerchMenu = FALSE;
					showMerchMenu();
					bPlayerCanMove = TRUE;
					SPR_setVisibility(merchant, VISIBLE);
					for( int i = 0; i < 24; i++){
						VDP_clearTextLine(i);
					}
					

					displayRoom();
				
			}else{
				if(!bBattleOngoing){
			 sramSave();
			 VDP_drawTextBG(BG_B, "Saved", 10, 10);
			 delayFrames(120);
			 VDP_drawTextBG(BG_B, "     ", 10, 10);
			 displayRoom();
		}}}
		// if(changed & state & BUTTON_A){
		// 	if(bShowMerchMenu){
		// 		bPlayerCanMove = FALSE;
		// 		showMerchMenu();
		// 		//hide merchant
		// 		SPR_setVisibility(merchant, HIDDEN);
		// 	}
		// }
	if((changed & state & BUTTON_DOWN)){
		selection = !selection;
	}
	if((changed & state & BUTTON_UP)){
		selection = !selection;
	}
	if ((changed & state & BUTTON_START)){
		if(!bBattleOngoing && !bBattleMessageDone && !bIsMoving && !showTitleScreen){
			//bIsMoving = FALSE;
			bPlayerCanMove = FALSE;
			showStats();
		}	
	}
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

void showStats(){
	bShowMenu = !bShowMenu;
	//bIsMoving = FALSE;
	bPlayerCanMove = FALSE;
	if(bShowMenu){
		// Hide the player sprite
		SPR_setVisibility(player, HIDDEN);
		SPR_setVisibility(merchant, HIDDEN);
		VDP_clearTileMap(BG_B, ind, 0, TRUE);
		//load font tiles
		VDP_loadFontData(tileset_Font.tiles, 96, CPU);
		drawBox(0, 0, 11, 11);
		//set font palette
		PAL_setPalette(PAL0, palette_Font.data, DMA);
		PAL_setPalette(PAL2, face.palette->data, DMA);
		VDP_drawImageEx(BG_B, &face, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ind), 1, 1, FALSE, TRUE);
		drawBox(0, 11, 11, 11);
		displayMiniMap(1, 12);
drawBox(11, 0, 21, 15);
		VDP_drawTextBG(BG_A, "HP: ", 12, 1);
		sprintf(pHP, "%d", player_hp);
		VDP_drawTextBG(BG_A, pHP, 17, 1);
		VDP_drawTextBG(BG_A, "/", 22, 1);
		sprintf(pHPMax, "%d", player_hp_max);
		VDP_drawTextBG(BG_A, pHPMax, 25, 1);
		VDP_drawTextBG(BG_A, "LVL: ", 12, 3);
		sprintf(pLevel, "%d", player_level);
		VDP_drawTextBG(BG_A, pLevel, 27, 3);
		VDP_drawTextBG(BG_A, "ATK: ", 12, 5);
		sprintf(pAttack, "%d", player_attack);
		VDP_drawTextBG(BG_A, pAttack, 27, 5);
		VDP_drawTextBG(BG_A, "DEF: ", 12, 7);
		sprintf(pDefense, "%d", player_defense);
		VDP_drawTextBG(BG_A, pDefense, 27, 7);
		VDP_drawTextBG(BG_A, "EXP: ", 12, 9);
		sprintf(pExp, "%d", player_exp);
		VDP_drawTextBG(BG_A, pExp, 27, 9);
		VDP_drawTextBG(BG_A, "Gold: ", 12, 11);
		sprintf(pGold, "%d", player_gold);
		VDP_drawTextBG(BG_A, pGold, 27, 11);
		VDP_drawTextBG(BG_A, "Kills: ", 12, 13);
		sprintf(goblinsKilledChar, "%d", goblinsKilled);
		VDP_drawTextBG(BG_A, goblinsKilledChar, 27, 13);
		
		drawBox(11, 15, 21, 7);

		VDP_drawTextBG(BG_A, "Skull:", 12, 16);
		sprintf(skullAmount, "%d", skulls);
		VDP_drawTextBG(BG_A, skullAmount, 18, 16);
		VDP_drawTextBG(BG_A, "Meat:", 12, 18);
		sprintf(meatAmount, "%d", meat);
		VDP_drawTextBG(BG_A, meatAmount, 18, 18);
		VDP_drawTextBG(BG_A, "Bone:", 12, 20);
		sprintf(bonesAmount, "%d", bones);
		VDP_drawTextBG(BG_A, bonesAmount, 18, 20);
		// VDP_drawTextBG(BG_B, "Skin:", 12, 22);
		// sprintf(skinAmount, "%d", skin);
		// VDP_drawTextBG(BG_B, skinAmount, 20, 22);
		VDP_drawTextBG(BG_A, "Eye:", 21, 16);
		sprintf(eyesAmount, "%d", eyes);
		VDP_drawTextBG(BG_A, eyesAmount, 27, 16);
		VDP_drawTextBG(BG_A, "Fang:", 21, 18);
		sprintf(fangAmount, "%d", fang);
		VDP_drawTextBG(BG_A, fangAmount, 27, 18);
		VDP_drawTextBG(BG_A, "Horn:", 21, 20);
		sprintf(hornAmount, "%d", horn);
		VDP_drawTextBG(BG_A, hornAmount, 27, 20);
		// VDP_drawTextBG(BG_B, "Tails:", 23, 22);
		// sprintf(tailAmount, "%d", tail);
		// VDP_drawTextBG(BG_B, tailAmount, 29, 22);
		drawBox(0, 22, 32, 6);


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
	VDP_clearPlane(BG_B, TRUE);
	VDP_clearPlane(BG_A, TRUE);
	VDP_loadTileSet(tileset1.tileset, 1, DMA);
    PAL_setPalette(PAL1, tileset1.palette->data, DMA);
	PAL_setPalette(PAL0, fg2.palette->data, DMA);
	PAL_setPalette(PAL3,palette_Font.data, DMA);
	PAL_setPalette(PAL2, our_sprite.palette->data, DMA);
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
	bPlayerCanMove = TRUE;
	displayRoom();
	}
}



void displayMiniMap( int x, int y){
	//display mini map using font tiles
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 9; j++){
			if ( i == 3 && j == 3){
				VDP_drawTextBG(BG_B, "$", x + j, y + i);
			}
			else if (i  == currentWorldY && j == currentWorldX){
				VDP_drawTextBG(BG_B, "%", x + j, y + i);
			}
			else if (i == merchWorldY && j == merchWorldX){
				VDP_drawTextBG(BG_B, "&", x + j, y + i);
			}
			else{
				VDP_drawTextBG(BG_B, "#", x + j, y + i);
			}
		}
	}
}




