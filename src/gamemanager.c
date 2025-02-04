#include "../inc/level.h"
#include "../inc/debuginfo.h"
#include "../inc/player.h"
#include "../inc/camera.h"
#include "../inc/collision.h"
#include "../inc/globals.h"
#include "../inc/titlescreen.h"
#include "../inc/makemap.h"
#include <resources.h>
#include "../inc/gamemanager.h"
#include "../inc/battle.h"

u16 roomCount = 0;
bool MapHasBeenMade = FALSE;

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
