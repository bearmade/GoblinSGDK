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

int attack_timer = 0;
int attack_duration = 24;
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

	if(!(value & BUTTON_DOWN) && !(value & BUTTON_UP) && !(value & BUTTON_LEFT) && !(value & BUTTON_RIGHT) && (playerDir == 0)) SPR_setAnim(player, ANIM_IDLE_DOWN);
	if(!(value & BUTTON_DOWN) && !(value & BUTTON_UP) && !(value & BUTTON_LEFT) && !(value & BUTTON_RIGHT) && (playerDir == 1)) SPR_setAnim(player, ANIM_IDLE_RIGHT);
	if(!(value & BUTTON_DOWN) && !(value & BUTTON_UP) && !(value & BUTTON_LEFT) && !(value & BUTTON_RIGHT) && (playerDir == 2)) SPR_setAnim(player, ANIM_IDLE_LEFT);
	if(!(value & BUTTON_DOWN) && !(value & BUTTON_UP) && !(value & BUTTON_LEFT) && !(value & BUTTON_RIGHT) && (playerDir == 3)) SPR_setAnim(player, ANIM_IDLE_UP);

	if( value & BUTTON_LEFT){
		playerDir = 2;
		player_move_right = FALSE;
		player_move_left = TRUE;
		SPR_setAnim(player, ANIM_WALK_LEFT);
	}
	else if( value & BUTTON_RIGHT){
		playerDir = 1;
		player_move_right = TRUE;
		player_move_left = FALSE;
		SPR_setAnim(player, ANIM_WALK_RIGHT);
	}
	else {
		player_move_left = FALSE;
		player_move_right = FALSE;
	}

	if( value & BUTTON_DOWN){
		playerDir = 0;
		player_move_down = TRUE;
		player_move_up = FALSE;
		SPR_setAnim(player, ANIM_WALK_DOWN);
	}
	else if( (value & BUTTON_UP)){
		playerDir = 3;
		player_move_down = FALSE;
		player_move_up = TRUE;
		SPR_setAnim(player, ANIM_WALK_UP);
	}
	else {
		player_move_down = FALSE;
		player_move_up = FALSE;
	}

	



}
