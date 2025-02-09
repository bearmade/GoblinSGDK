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

	if((changed & state & BUTTON_START)){
		showTitleScreen = FALSE;
		if(bBattleOngoing){
		//delayVBlank(120);
		if(bBattleMessageDone){
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
					//XGM_startPlayPCM(SFX_SWOOSH, 15, SOUND_PCM_CH2);
					attack();
					
				}
				else{
								
				}
				
			}
		}
		}
		else if(!bBattleOngoing && !bBattleMessageDone && !bIsMoving){
			//bIsMoving = FALSE;
			bPlayerCanMove = FALSE;
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
		VDP_drawTextBG(BG_B, "Kills: ", 3, 16);
		sprintf(goblinsKilledChar, "%d", goblinsKilled);
		VDP_drawTextBG(BG_B, goblinsKilledChar, 16, 16);
		displayMiniMap();
		VDP_drawTextBG(BG_B, "Skulls", 3, 18);
		sprintf(skullAmount, "%d", skulls);
		VDP_drawTextBG(BG_B, skullAmount, 16, 18);
		VDP_drawTextBG(BG_B, "Meat: ", 3, 20);
		sprintf(meatAmount, "%d", meat);
		VDP_drawTextBG(BG_B, meatAmount, 16, 20);
		VDP_drawTextBG(BG_B, "Bones: ", 3, 22);
		sprintf(bonesAmount, "%d", bones);
		VDP_drawTextBG(BG_B, bonesAmount, 16, 22);
		VDP_drawTextBG(BG_B, "Skin: ", 3, 24);
		sprintf(skinAmount, "%d", skin);
		VDP_drawTextBG(BG_B, skinAmount, 16, 24);
		VDP_drawTextBG(BG_B, "Eyes: ", 19, 18);
		sprintf(eyesAmount, "%d", eyes);
		VDP_drawTextBG(BG_B, eyesAmount, 29, 18);
		VDP_drawTextBG(BG_B, "Fangs: ", 19, 20);
		sprintf(fangAmount, "%d", fang);
		VDP_drawTextBG(BG_B, fangAmount, 29, 20);
		VDP_drawTextBG(BG_B, "Horns: ", 19, 22);
		sprintf(hornAmount, "%d", horn);
		VDP_drawTextBG(BG_B, hornAmount, 29, 22);
		VDP_drawTextBG(BG_B, "Tails: ", 19, 24);
		sprintf(tailAmount, "%d", tail);
		VDP_drawTextBG(BG_B, tailAmount, 29, 24);



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

void showMerchMenu(){
	u16 randomItemForSale = random() % 10;
	u16 randomItemPrice = random() % 10;
	u16 itemPrice = 0;
	char itemPriceString[4];

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
		VDP_drawTextBG(BG_B, "Purchase", 4, 4);
		//randomItemForSale = 0;
		switch (randomItemForSale) {
			case 0:
				VDP_drawTextBG(BG_B, "Skull", 4, 6);
				itemPrice = skull_base + (random() % skull_base);
				sprintf(itemPriceString, "%d", itemPrice);
				VDP_drawTextBG(BG_B, itemPriceString, 12, 6);
				VDP_drawTextBG(BG_B, " Gold?", 15, 6);
				break;
			case 1:
				VDP_drawTextBG(BG_B, "Meat", 4, 6);
				itemPrice = meat_base + (random() % meat_base);
				sprintf(itemPriceString, "%d", itemPrice);
				VDP_drawTextBG(BG_B, itemPriceString, 12, 6);
				VDP_drawTextBG(BG_B, " Gold?", 15, 6);
				break;
			case 2:
				VDP_drawTextBG(BG_B, "Bones", 4, 6);
				itemPrice = bones_base + (random() % bones_base);
				sprintf(itemPriceString, "%d", itemPrice);
				VDP_drawTextBG(BG_B, itemPriceString, 12, 6);
				VDP_drawTextBG(BG_B, " Gold?", 15, 6);
				break;
			case 3:
				VDP_drawTextBG(BG_B, "Skin", 4, 6);
				itemPrice = skin_base + (random() % skin_base);
				sprintf(itemPriceString, "%d", itemPrice);
				VDP_drawTextBG(BG_B, itemPriceString, 12, 6);
				VDP_drawTextBG(BG_B, " Gold?", 15, 6);	
				break;
			case 4:
				VDP_drawTextBG(BG_B, "Eyes", 4, 6);
				itemPrice = eyes_base + (random() % eyes_base);
				sprintf(itemPriceString, "%d", itemPrice);
				VDP_drawTextBG(BG_B, itemPriceString, 12, 6);
				VDP_drawTextBG(BG_B, " Gold?", 15, 6);
				break;
			case 5:
				VDP_drawTextBG(BG_B, "Fangs", 4, 6);
				itemPrice = fang_base + (random() % fang_base);
				sprintf(itemPriceString, "%d", itemPrice);
				VDP_drawTextBG(BG_B, itemPriceString, 12, 6);
				VDP_drawTextBG(BG_B, " Gold?", 15, 6);
				break;
			case 6:
				VDP_drawTextBG(BG_B, "Horn", 4, 6);
				itemPrice = horn_base + (random() % horn_base);
				sprintf(itemPriceString, "%d", itemPrice);
				VDP_drawTextBG(BG_B, itemPriceString, 12, 6);
				VDP_drawTextBG(BG_B, " Gold?", 15, 6);
				break;
			case  7:
				VDP_drawTextBG(BG_B, "Tail", 4, 6);
				itemPrice = tail_base + (random() % tail_base);
				sprintf(itemPriceString, "%d", itemPrice);
				VDP_drawTextBG(BG_B, itemPriceString, 12, 6);
				VDP_drawTextBG(BG_B, " Gold?", 15, 6);
				break;
			case 8:
				VDP_drawTextBG(BG_B, "ATK UP", 4, 6);
				itemPrice = player_attack * (50 + random() % 50);
				sprintf(itemPriceString, "%d", itemPrice);
				VDP_drawTextBG(BG_B, itemPriceString, 12, 6);
				VDP_drawTextBG(BG_B, " Gold?", 15, 6);
				break;
			case 9:
				VDP_drawTextBG(BG_B, "DEF UP", 4, 6);
				itemPrice = player_defense * (50 + random() % 50);
				sprintf(itemPriceString, "%d", itemPrice);
				VDP_drawTextBG(BG_B, itemPriceString, 12, 6);
				VDP_drawTextBG(BG_B, " Gold?", 15, 6);
			break;

		}

	}
	else{
		// When closing menu, set new random merchant position
		merchWorldX = random() % 8;
		merchWorldY = random() % 8;
		while(WORLD_LAYOUT[merchWorldY][merchWorldX] != 1) {
			// Keep trying until we find a valid room
			merchWorldX = random() % 8;
			merchWorldY = random() % 8;
		}
		//show player sprite
		SPR_setVisibility(player, VISIBLE);
		//redraw screen
		VDP_loadTileSet(tileset1.tileset, 1, DMA);
		PAL_setPalette(PAL1, tileset1.palette->data, DMA);
		PAL_setPalette(PAL0, fg2.palette->data, DMA);
		//clear all battle messages
		VDP_drawTextBG(BG_B, "      ", 4, 2);
		VDP_drawTextBG(BG_B, "      ", 4, 4);
		VDP_drawTextBG(BG_B, "      ", 8, 4);
		VDP_drawTextBG(BG_B, "      ", 4, 6);
		displayRoom();
	}
}

void displayMiniMap(){
	//display mini map using font tiles
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			if ( i == 3 && j == 3){
				VDP_drawTextBG(BG_B, "$", 22 + j, 6 + i);
			}
			else if (i  == currentWorldY && j == currentWorldX){
				VDP_drawTextBG(BG_B, "%", 22 + j, 6 + i);
			}
			else if (i == merchWorldY && j == merchWorldX){
				VDP_drawTextBG(BG_B, "&", 22 + j, 6 + i);
			}
			else{
				VDP_drawTextBG(BG_B, "#", 22 + j, 6 + i);
			}
		}
	}
}




