#include "../inc/debuginfo.h"
#include "../inc/level.h"
#include "../inc/player.h"
#include "../inc/camera.h"
#include "../inc/makemap.h"

int current_cam_x;
int current_cam_y;
void camera() {
const fix32 SCROLL_SPEED = FIX32(4);

if (isTransitioning) {
	switch(transitionDirection) {
		case 0: // Right
			transitionOffset += SCROLL_SPEED;
			if (fix32ToInt(transitionOffset) >= HORIZONTAL_RESOLUTION) {
				VDP_clearPlane(BG_A, TRUE);
				VDP_clearPlane(BG_B, TRUE);
				updateCurrentRoom();
				convertMapArrays();
				displayRoom();
				finishTransition();
			}
			VDP_setHorizontalScroll(BG_A, -fix32ToInt(transitionOffset));
			VDP_setHorizontalScroll(BG_B, -fix32ToInt(transitionOffset));
			break;
			
		case 1: // Left 
			transitionOffset -= SCROLL_SPEED;
			if (fix32ToInt(transitionOffset) <= -HORIZONTAL_RESOLUTION) {
				VDP_clearPlane(BG_A, TRUE);
				VDP_clearPlane(BG_B, TRUE);
				updateCurrentRoom();
				convertMapArrays();
				displayRoom();
				finishTransition();
			}
			VDP_setHorizontalScroll(BG_A, -fix32ToInt(transitionOffset));
			VDP_setHorizontalScroll(BG_B, -fix32ToInt(transitionOffset));
			break;
			
		case 2: // Down
			transitionOffset += SCROLL_SPEED;
			if (fix32ToInt(transitionOffset) >= VERTICAL_RESOLUTION) {
				VDP_clearPlane(BG_A, TRUE);
				VDP_clearPlane(BG_B, TRUE);
				updateCurrentRoom();
				convertMapArrays();
				displayRoom();
				finishTransition();
			}
			VDP_setVerticalScroll(BG_A, fix32ToInt(transitionOffset));
			VDP_setVerticalScroll(BG_B, fix32ToInt(transitionOffset));
			break;
			
		case 3: // Up
			transitionOffset -= SCROLL_SPEED;
			
			if (fix32ToInt(transitionOffset) <= -VERTICAL_RESOLUTION) {
				VDP_clearPlane(BG_A, TRUE);
				VDP_clearPlane(BG_B, TRUE);
				updateCurrentRoom();
				convertMapArrays();
				displayRoom();
				finishTransition();
			}

			VDP_setVerticalScroll(BG_A, fix32ToInt(transitionOffset));
			VDP_setVerticalScroll(BG_B, fix32ToInt(transitionOffset));
			break;
	}
	
	SPR_setPosition(player, fix32ToInt(playerPosX) - current_cam_x, fix32ToInt(playerPosY) - current_cam_y);
	return;
}

	

	if(playerPosX < FIX32(0)) {
		 // Left
		 
		currentWorldX--;	
		startTransition(1);	
		playerPosX = FIX32(MAP_WIDTH - PLAYER_WIDTH);
		
	}
	else if (playerPosX > FIX32(MAP_WIDTH - PLAYER_WIDTH)) {
		
		currentWorldX++;
		startTransition(0); // Right
		playerPosX = FIX32(0);
	}

	if(playerPosY < FIX32(0)) {

		currentWorldY--;
		startTransition(3); // Up
		playerPosY = FIX32(MAP_HEIGHT - PLAYER_HEIGHT);
	}
	else if (playerPosY > FIX32(MAP_HEIGHT - PLAYER_HEIGHT)) {

		currentWorldY++;
		startTransition(2); // Down
		playerPosY = FIX32(0);
	}

	int player_x_map_int = fix32ToInt(playerPosX);
	int player_y_map_int = fix32ToInt(playerPosY);

	int player_x_screen_pos = player_x_map_int - current_cam_x;
	int player_y_screen_pos = player_y_map_int - current_cam_y;

	int new_cam_x;
	int new_cam_y;

	if(player_x_screen_pos > HOW_FAR_RIGHT_BEFORE_CAMERA_MOVES) {
		new_cam_x = player_x_map_int - HOW_FAR_RIGHT_BEFORE_CAMERA_MOVES;
	}
	else if(player_x_screen_pos < HOW_FAR_LEFT_BEFORE_CAMERA_MOVES) {
		new_cam_x = player_x_map_int - HOW_FAR_LEFT_BEFORE_CAMERA_MOVES;
	}
	else new_cam_x = current_cam_x;

	if(player_y_screen_pos > HOW_FAR_BOTTOM_BEFORE_CAMERA_MOVES) {
		new_cam_y = player_y_map_int - HOW_FAR_BOTTOM_BEFORE_CAMERA_MOVES;
	}
	else if(player_y_screen_pos < HOW_FAR_TOP_BEFORE_CAMERA_MOVES) {
		new_cam_y = player_y_map_int - HOW_FAR_TOP_BEFORE_CAMERA_MOVES;
	}
	else new_cam_y = current_cam_y;

	if(new_cam_x < 0) {
		new_cam_x = 0;
	}
	else if(new_cam_x > (MAP_WIDTH - HORIZONTAL_RESOLUTION)) {
		new_cam_x = MAP_WIDTH - HORIZONTAL_RESOLUTION;
	}
	if(new_cam_y < 0) {
		new_cam_y = 0;
	}
	else if(new_cam_y > (MAP_HEIGHT - VERTICAL_RESOLUTION)) {
		new_cam_y = MAP_HEIGHT - VERTICAL_RESOLUTION;
	}

	if((current_cam_x != new_cam_x) || (current_cam_y != new_cam_y)) {
		current_cam_x = new_cam_x;
		current_cam_y = new_cam_y;

		VDP_setHorizontalScroll(BG_A, -new_cam_x);
		VDP_setVerticalScroll(BG_A, new_cam_y);
		VDP_setHorizontalScroll(BG_B, -new_cam_x);
		VDP_setVerticalScroll(BG_B, new_cam_y);
	}

	SPR_setPosition(player, fix32ToInt(playerPosX) - new_cam_x, fix32ToInt(playerPosY) - new_cam_y);
}
void startTransition(int direction) {
	SPR_setVisibility(player, HIDDEN);
	SPR_setVisibility(merchant, HIDDEN);
	isTransitioning = TRUE;
	    // Clear and pre-load next room at start of transition
    //VDP_clearPlane(BG_A, TRUE);
    //VDP_clearPlane(BG_B, TRUE);
	
    updateCurrentRoom();
	showMerchant();
    convertMapArrays();
    displayRoom();
	
    
    transitionDirection = direction;
    transitionOffset = FIX32(0);
    

}
void finishTransition() {
	SPR_setVisibility(player, VISIBLE);
	//SPR_setVisibility(merchant, VISIBLE);
	
    isTransitioning = FALSE;
    transitionOffset = FIX32(0);
    VDP_setHorizontalScroll(BG_A, 0);
    VDP_setHorizontalScroll(BG_B, 0);
    VDP_setVerticalScroll(BG_A, 0);
    VDP_setVerticalScroll(BG_B, 0);
	showMerchant();
}