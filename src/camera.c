#include "../inc/debuginfo.h"
#include "../inc/level.h"
#include "../inc/player.h"
#include "../inc/camera.h"
#include "../inc/makemap.h"

int current_cam_x;
int current_cam_y;


void camera(){

	if(playerPosX < FIX32(0)){
		currentWorldX = currentWorldX - 1;
		VDP_clearPlane(BG_A, TRUE);
		VDP_clearPlane(BG_B, TRUE);
		updateCurrentRoom();
    	convertMapArrays();
    	displayRoom();
		playerPosX = FIX32(MAP_WIDTH - PLAYER_WIDTH);
	}
	else if (playerPosX > FIX32(MAP_WIDTH - PLAYER_WIDTH)){
		currentWorldX = currentWorldX + 1;
		VDP_clearPlane(BG_A, TRUE);
		VDP_clearPlane(BG_B, TRUE);
		updateCurrentRoom();
    	convertMapArrays();
    	displayRoom();
		playerPosX = FIX32(0);

	}
	if(playerPosY < FIX32(0)){
		currentWorldY = currentWorldY - 1;
		VDP_clearPlane(BG_A, TRUE);
		VDP_clearPlane(BG_B, TRUE);
		updateCurrentRoom();
    	convertMapArrays();
    	displayRoom();
		playerPosY = FIX32(MAP_HEIGHT - PLAYER_HEIGHT);

	}
	else if (playerPosY > FIX32(MAP_HEIGHT - PLAYER_HEIGHT)){
		currentWorldY = currentWorldY + 1;
		VDP_clearPlane(BG_A, TRUE);
		VDP_clearPlane(BG_B, TRUE);
		updateCurrentRoom();
    	convertMapArrays();
    	displayRoom();

		playerPosY = FIX32(0);
	}

	int player_x_map_int = fix32ToInt(playerPosX);
	int player_y_map_int = fix32ToInt(playerPosY);

	int player_x_screen_pos = player_x_map_int - current_cam_x;
	int player_y_screen_pos = player_y_map_int - current_cam_y;

	int new_cam_x;
	int new_cam_y;

	if(player_x_screen_pos > HOW_FAR_RIGHT_BEFORE_CAMERA_MOVES){
		new_cam_x = player_x_map_int - HOW_FAR_RIGHT_BEFORE_CAMERA_MOVES;
	}
	else if(player_x_screen_pos < HOW_FAR_LEFT_BEFORE_CAMERA_MOVES){
		new_cam_x = player_x_map_int - HOW_FAR_LEFT_BEFORE_CAMERA_MOVES;
	}
	else new_cam_x = current_cam_x;

	if(player_y_screen_pos > HOW_FAR_BOTTOM_BEFORE_CAMERA_MOVES){
		new_cam_y = player_y_map_int - HOW_FAR_BOTTOM_BEFORE_CAMERA_MOVES;
	}
	else if(player_y_screen_pos < HOW_FAR_TOP_BEFORE_CAMERA_MOVES){
		new_cam_y = player_y_map_int - HOW_FAR_TOP_BEFORE_CAMERA_MOVES;
	}
	else new_cam_y = current_cam_y;

	if(new_cam_x < 0){
		new_cam_x = 0;
	}
	else if(new_cam_x > (MAP_WIDTH - HORIZONTAL_RESOLUTION)){
		new_cam_x = MAP_WIDTH - HORIZONTAL_RESOLUTION;
	}
	if(new_cam_y < 0){
		new_cam_y = 0;
	}
	else if(new_cam_y > (MAP_HEIGHT - VERTICAL_RESOLUTION)){
		new_cam_y = MAP_HEIGHT - VERTICAL_RESOLUTION;
	}

	if((current_cam_x != new_cam_x) ||(current_cam_y != new_cam_y)){
		current_cam_x = new_cam_x;
		current_cam_y = new_cam_y;

		//MAP_scrollTo(level_1_map, new_cam_x, new_cam_y);
		//MAP_scrollTo(level_2_map, new_cam_x, new_cam_y);
		VDP_setHorizontalScroll(BG_A, -new_cam_x);
		VDP_setVerticalScroll(BG_A, new_cam_y);
		VDP_setHorizontalScroll(BG_B, -new_cam_x);
		VDP_setVerticalScroll(BG_B, new_cam_y);


	}
	//SPR_setPosition(ant, fix32ToInt(bugPosX) - new_cam_x, fix32ToInt(bugPosY) - new_cam_y);
	SPR_setPosition(player, fix32ToInt(playerPosX) - new_cam_x, fix32ToInt(playerPosY) - new_cam_y);

}
