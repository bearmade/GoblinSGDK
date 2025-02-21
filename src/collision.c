#include "../inc/debuginfo.h"
#include "../inc/level.h"
#include "../inc/player.h"
#include "../inc/camera.h"
#include "../inc/collision.h"
#include "../inc/makemap.h"
#include "../inc/gamemanager.h"
#include "../inc/inventory.h"
#include "../inc/battle.h"
#include "../inc/globals.h"

// Add offset values to shrink collision box
#define MERCHANT_COL_OFFSET_X 14  
#define MERCHANT_COL_OFFSET_Y 12  




void collision(){

	s16 player_coll_coord_left;
	s16 player_coll_coord_right;
	s16 player_coll_coord_up = fix32ToInt(playerPosY) + PLAYER_COL_UP;
	s16 player_coll_coord_down = fix32ToInt(playerPosY) + PLAYER_COL_DOWN;

	s16 xtile_coord_player_left;
	s16 xtile_coord_player_right;
	s16 ytile_coord_player_up = player_coll_coord_up >> 4;
	s16 ytile_coord_player_down = player_coll_coord_down >> 4;

	u16 arrayIndex_topleft_col;
	u16 arrayIndex_topright_col;
	u16 arrayIndex_bottomleft_col;
	u16 arrayIndex_bottomright_col;

	u8 tile_type_coll_topleft;
	u8 tile_type_coll_topright;
	u8 tile_type_coll_bottomleft;
	u8 tile_type_coll_bottomright;

	s16 blocked_coord;

	s16 playerX = fix32ToInt(playerPosX);
	s16 playerY = fix32ToInt(playerPosY);
	s16 merchantX = fix32ToInt(merchantPosX); // You'll need these position variables
	s16 merchantY = fix32ToInt(merchantPosY); // for your merchant sprite
	bool merchantInteraction ;
			// Box collision detection
    

			if (playerX < (merchantX + MERCHANT_COL_OFFSET_X) + (MERCHANT_WIDTH - MERCHANT_COL_OFFSET_X*2) &&
				playerX + PLAYER_WIDTH > merchantX + MERCHANT_COL_OFFSET_X &&
				playerY < (merchantY + MERCHANT_COL_OFFSET_Y) + (MERCHANT_HEIGHT - MERCHANT_COL_OFFSET_Y*2) &&
				playerY + PLAYER_HEIGHT > merchantY + MERCHANT_COL_OFFSET_Y &&
				currentWorldX == merchWorldX && currentWorldY == merchWorldY) {
		
				// Set interaction flag
				merchantInteraction = TRUE;
		
		
				// Prevent walking through merchant by pushing player back
				if(player_move_right && playerX < merchantX) {
					playerPosX -= playerSpeed;
				}
				if(player_move_left && playerX > merchantX) {
					playerPosX += playerSpeed;
				}
				if(player_move_down && playerY < merchantY) {
					playerPosY -= playerSpeed;
				}
				if(player_move_up && playerY > merchantY) {
					playerPosY += playerSpeed;
				}
		
			
				//check joy button a press
				if(JOY_readJoypad(JOY_1) & BUTTON_A){

					if(bShowMerchMenu == FALSE){
						waitMs(1000);
						bShowMerchMenu = TRUE;
						bPlayerCanMove = FALSE;
						showMerchMenu();
						SPR_setVisibility(merchant, HIDDEN);
					}

				}
				
			} else {
				bShowMerchMenu = FALSE;
				merchantInteraction = FALSE;
				resetMerchantInteractions();
			}
	if(player_move_left){
		playerPosX -= playerSpeed;
		player_coll_coord_left = fix32ToInt(playerPosX) + PLAYER_COL_LEFT;
		xtile_coord_player_left = player_coll_coord_left >> 4;
		arrayIndex_topleft_col = xtile_coord_player_left + (ytile_coord_player_up * LENGTH_OF_LEVEL_ARRAY);
		tile_type_coll_topleft = LEVEL_COL[arrayIndex_topleft_col];
		arrayIndex_bottomleft_col = xtile_coord_player_left + (ytile_coord_player_down * LENGTH_OF_LEVEL_ARRAY);
		tile_type_coll_bottomleft = LEVEL_COL[arrayIndex_bottomleft_col];

		if((tile_type_coll_topleft == SOLID_TILE)||(tile_type_coll_bottomleft == SOLID_TILE)){
			blocked_coord = (xtile_coord_player_left << 4) + 16 - PLAYER_COL_LEFT;
			playerPosX = intToFix32(blocked_coord);
		}
		if((tile_type_coll_topleft == BLOCKER_TILE)||(tile_type_coll_bottomleft == BLOCKER_TILE)){
			blocked_coord = (xtile_coord_player_left << 4) + 16 - PLAYER_COL_LEFT;
			playerPosX = intToFix32(blocked_coord);
		}
		if((tile_type_coll_topleft == EXIT_TILE)||(tile_type_coll_bottomleft == EXIT_TILE)){
			//VDP_clearTileMapRect(BG_B, (fix32ToInt(playerPosX) >> 3),(fix32ToInt(playerPosY) >> 3),2,2);
			// VDP_clearPlane(BG_A, TRUE);
			// VDP_clearPlane(BG_B, TRUE);
			// SPR_setVisibility(player, HIDDEN);
			//sramSave(0);

			
			showPlayerHouse();
			 //player_hp = player_hp_max;
			//playerPosX = FIX32(100);
			//playerPosY = FIX32(100);
			//makeMap();
			//roomCount++;

		}
		if ((tile_type_coll_topleft == CAVE_ENTRANCE_TILE)||(tile_type_coll_bottomleft == CAVE_ENTRANCE_TILE)){
			VDP_clearPlane(BG_A, TRUE);
			VDP_clearPlane(BG_B, TRUE);
			SPR_setVisibility(player, HIDDEN);
			//sramSave(0);

		}
		
	}
	else if(player_move_right){
		playerPosX += playerSpeed;
		player_coll_coord_right = fix32ToInt(playerPosX) + PLAYER_COL_RIGHT;
		xtile_coord_player_right = player_coll_coord_right >> 4;
		arrayIndex_topright_col = xtile_coord_player_right + (ytile_coord_player_up * LENGTH_OF_LEVEL_ARRAY);
		tile_type_coll_topright = LEVEL_COL[arrayIndex_topright_col];
		arrayIndex_bottomright_col = xtile_coord_player_right + (ytile_coord_player_down * LENGTH_OF_LEVEL_ARRAY);
		tile_type_coll_bottomright = LEVEL_COL[arrayIndex_bottomright_col];

		if((tile_type_coll_topright == SOLID_TILE)||(tile_type_coll_bottomright == SOLID_TILE)){
			blocked_coord = (xtile_coord_player_right << 4) - PLAYER_COL_RIGHT;
			playerPosX = intToFix32(blocked_coord);
			playerPosX -= FIX32(0.1);
		}
		if((tile_type_coll_topright == BLOCKER_TILE)||(tile_type_coll_bottomright == BLOCKER_TILE)){
			blocked_coord = (xtile_coord_player_right << 4) - PLAYER_COL_RIGHT;
			playerPosX = intToFix32(blocked_coord);
			playerPosX -= FIX32(0.1);
		}
		if((tile_type_coll_topright == EXIT_TILE)||(tile_type_coll_bottomright == EXIT_TILE)){
			//VDP_clearTileMapRect(BG_B, (fix32ToInt(playerPosX) >> 3),(fix32ToInt(playerPosY) >> 3),2,2);
			// VDP_clearPlane(BG_A, TRUE);
			// VDP_clearPlane(BG_B, TRUE);
			//SPR_setVisibility(player, HIDDEN);
			//sramSave(0);
			showPlayerHouse();
			//player_hp = player_hp_max;
		}
		if((tile_type_coll_topright == CAVE_ENTRANCE_TILE)||(tile_type_coll_bottomright == CAVE_ENTRANCE_TILE)){
			//VDP_clearTileMapRect(BG_B, (fix32ToInt(playerPosX) >> 3),(fix32ToInt(playerPosY) >> 3),2,2);
			VDP_clearPlane(BG_A, TRUE);
			VDP_clearPlane(BG_B, TRUE);
			SPR_setVisibility(player, HIDDEN);
			//sramSave(0);

	}}
	if (player_move_up){
		playerPosY -= playerSpeed;
		player_coll_coord_up = fix32ToInt(playerPosY) + PLAYER_COL_UP;
		ytile_coord_player_up = player_coll_coord_up >> 4;

		player_coll_coord_left = fix32ToInt(playerPosX) + PLAYER_COL_LEFT;
		xtile_coord_player_left = player_coll_coord_left >> 4;
		arrayIndex_topleft_col = xtile_coord_player_left + (ytile_coord_player_up * LENGTH_OF_LEVEL_ARRAY);
		tile_type_coll_topleft = LEVEL_COL[arrayIndex_topleft_col];

		player_coll_coord_right = fix32ToInt(playerPosX) + PLAYER_COL_RIGHT;
		xtile_coord_player_right = player_coll_coord_right >> 4;
		arrayIndex_topright_col = xtile_coord_player_right + (ytile_coord_player_up * LENGTH_OF_LEVEL_ARRAY);
		tile_type_coll_topright = LEVEL_COL[arrayIndex_topright_col];

		if((tile_type_coll_topright == SOLID_TILE)||(tile_type_coll_topleft == SOLID_TILE)){
			blocked_coord = (ytile_coord_player_up << 4) + 16 - PLAYER_COL_UP;
			playerPosY = intToFix32(blocked_coord);
		}
		if((tile_type_coll_topright == BLOCKER_TILE)||(tile_type_coll_topleft == BLOCKER_TILE)){
			blocked_coord = (ytile_coord_player_up << 4) + 16 - PLAYER_COL_UP;
			playerPosY = intToFix32(blocked_coord);
		}
		if((tile_type_coll_topright == EXIT_TILE)||(tile_type_coll_topleft == EXIT_TILE)){
			//VDP_clearTileMapRect(BG_B, (fix32ToInt(playerPosX) >> 3),(fix32ToInt(playerPosY) >> 3),2,2);
			// VDP_clearPlane(BG_A, TRUE);
			// VDP_clearPlane(BG_B, TRUE);
			// SPR_setVisibility(player, HIDDEN);
			showPlayerHouse();

			 //player_hp = player_hp_max;
			//sramSave(0);
		}
		if((tile_type_coll_topright == CAVE_ENTRANCE_TILE)||(tile_type_coll_topleft == CAVE_ENTRANCE_TILE)){
			VDP_clearPlane(BG_A, TRUE);
			VDP_clearPlane(BG_B, TRUE);
			SPR_setVisibility(player, HIDDEN);
			//sramSave(0);

		}

	}
	else if (player_move_down){
		playerPosY += playerSpeed;
		player_coll_coord_down = fix32ToInt(playerPosY) + PLAYER_COL_DOWN;
		ytile_coord_player_down = player_coll_coord_down >> 4;

		player_coll_coord_left = fix32ToInt(playerPosX) + PLAYER_COL_LEFT;
		xtile_coord_player_left = player_coll_coord_left >> 4;
		arrayIndex_bottomleft_col = xtile_coord_player_left + (ytile_coord_player_down * LENGTH_OF_LEVEL_ARRAY);
		tile_type_coll_bottomleft = LEVEL_COL[arrayIndex_bottomleft_col];

		player_coll_coord_right = fix32ToInt(playerPosX) + PLAYER_COL_RIGHT;
		xtile_coord_player_right = player_coll_coord_right >> 4;
		arrayIndex_bottomright_col = xtile_coord_player_right + (ytile_coord_player_down * LENGTH_OF_LEVEL_ARRAY);
		tile_type_coll_bottomright = LEVEL_COL[arrayIndex_bottomright_col];

		if((tile_type_coll_bottomright == SOLID_TILE)||(tile_type_coll_bottomleft == SOLID_TILE)){
			blocked_coord = (ytile_coord_player_down << 4) - PLAYER_COL_DOWN;
			playerPosY = intToFix32(blocked_coord);
			playerPosY -= FIX32(0.1);
		}
		if((tile_type_coll_bottomright == BLOCKER_TILE)||(tile_type_coll_bottomleft == BLOCKER_TILE)){
			blocked_coord = (ytile_coord_player_down << 4) - PLAYER_COL_DOWN;
			playerPosY = intToFix32(blocked_coord);
			playerPosY -= FIX32(0.1);
		}
		if((tile_type_coll_bottomright == EXIT_TILE)||(tile_type_coll_bottomleft == EXIT_TILE)){
			//VDP_clearTileMapRect(BG_B, (fix32ToInt(playerPosX) >> 3),(fix32ToInt(playerPosY) >> 3),2,2);
			// VDP_clearPlane(BG_A, TRUE);
			// VDP_clearPlane(BG_B, TRUE);
			// 	 SPR_setVisibility(player, HIDDEN);
			//VDP_clearPlane(BG_A, TRUE);
			//VDP_clearPlane(BG_B, TRUE);
			//SPR_setVisibility(player, HIDDEN);
			// check for key press
			showPlayerHouse();
			// player_hp = player_hp_max;
			//sramSave(0);
		}
		if((tile_type_coll_bottomright == CAVE_ENTRANCE_TILE)||(tile_type_coll_bottomleft == CAVE_ENTRANCE_TILE)){
			VDP_clearPlane(BG_A, TRUE);
			VDP_clearPlane(BG_B, TRUE);
			SPR_setVisibility(player, HIDDEN);
			//sramSave(0);

		}
	}

}



