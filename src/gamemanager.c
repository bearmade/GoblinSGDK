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
#include "../inc/inventory.h"
#include <string.h>

#define SAVE_SLOT_SIZE 68  // Reserve 40 bytes per slot; adjust as needed
#define PLAYER_HOUSE_COOLDOWN_MS 500

u16 roomCount = 0;
bool MapHasBeenMade = FALSE;
char player_name[11] = "AAAAAAAAAA";
u32 playerHouseCooldown = 0;

void sramSave(u8 slot){
    // Ensure slot is within range (0 to 2)
    if(slot > 2){
        return;
    }
    
    u16 base = slot * SAVE_SLOT_SIZE;
    
    SRAM_enable();
    SRAM_writeWord(base + 0, player_hp);
    SRAM_writeWord(base + 2, player_hp_max);
    SRAM_writeWord(base + 4, player_level);
    SRAM_writeWord(base + 6, player_attack);
    SRAM_writeWord(base + 8, player_defense);
    SRAM_writeWord(base + 10, player_exp);
    SRAM_writeWord(base + 12, player_exp_needed);
    SRAM_writeWord(base + 14, player_gold);
    SRAM_writeWord(base + 16, goblinsKilled);
    SRAM_writeWord(base + 18, skulls);
    SRAM_writeWord(base + 20, meat);
    SRAM_writeWord(base + 22, bones);
    SRAM_writeWord(base + 24, skin);
    SRAM_writeWord(base + 26, tail);
    SRAM_writeWord(base + 28, horn);
    SRAM_writeWord(base + 30, eyes);
    SRAM_writeWord(base + 32, fang);
    SRAM_writeLong(base + 34, worldSeed);
    // Write player_name byte-by-byte; using sizeof(player_name) which is 11
    for(u8 i = 0; i < sizeof(player_name); i++){
        SRAM_writeByte(base + 38 + i, player_name[i]);
    }
    SRAM_disable();
}

void sramLoad(u8 slot){
    if(slot > 2){
        return;
    }
    
    u16 base = slot * SAVE_SLOT_SIZE;
    
    SRAM_enable();
    player_hp         = SRAM_readWord(base + 0);
    player_hp_max     = SRAM_readWord(base + 2);
    player_level      = SRAM_readWord(base + 4);
    player_attack     = SRAM_readWord(base + 6);
    player_defense    = SRAM_readWord(base + 8);
    player_exp        = SRAM_readWord(base + 10);
    player_exp_needed = SRAM_readWord(base + 12);
    player_gold       = SRAM_readWord(base + 14);
    goblinsKilled     = SRAM_readWord(base + 16);
    skulls            = SRAM_readWord(base + 18);
    meat              = SRAM_readWord(base + 20);
    bones             = SRAM_readWord(base + 22);
    skin              = SRAM_readWord(base + 24);
    tail              = SRAM_readWord(base + 26);
    horn              = SRAM_readWord(base + 28);
    eyes              = SRAM_readWord(base + 30);
    fang              = SRAM_readWord(base + 32);
    worldSeed         = SRAM_readLong(base + 34);
    // Read player_name byte-by-byte
    for(u8 i = 0; i < sizeof(player_name); i++){
        player_name[i] = SRAM_readByte(base + 38 + i);
    }
    SRAM_disable();
}

void showPlayerHouse(){
	// Only show the player house if the cooldown timer is not active.
	if(playerHouseCooldown != 0) {
		// Optionally, you could display a message like "House unavailable" or simply return.
		return;
	}
	bInsideHouse = TRUE;

	
	// When entering the player house, stop movement etc.
	bPlayerCanMove = FALSE;
	bIsMoving = FALSE;
	canFight = FALSE;
	SPR_setVisibility(player, HIDDEN);
	SPR_update();
	VDP_clearPlane(BG_A, TRUE);
	VDP_clearPlane(BG_B, TRUE);
	PAL_setPalette(PAL0, house.palette->data, DMA);
    VDP_drawImageEx(BG_B, &house, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, ind), 0, 0, FALSE, TRUE);


	VDP_loadFontData(tileset_Font.tiles, 96, CPU);
	PAL_setPalette(PAL1, palette_Font.data, DMA);

	drawBox(9, 4, 20, 12);


	VDP_drawTextBG(BG_A, "Player House", 10, 5);
	VDP_drawTextBG(BG_A, "Press DOWN to Exit", 10, 7);
	VDP_drawTextBG(BG_A, "Hold A to Rest", 10, 8);
	//VDP_drawTextBG(BG_A, "Press C to Save", 15, 9);
	VDP_drawTextBG(BG_A, "Health:", 10, 11);
	sprintf(pHP, "%d", player_hp);
	VDP_drawTextBG(BG_A, pHP, 10, 12);
	while(bInsideHouse){
		SPR_setVisibility(player, HIDDEN);
		SYS_doVBlankProcess();
	// Rest mechanics: while the B button is pressed, "rest" and update health.
	while(JOY_readJoypad(JOY_1) & BUTTON_A){
		VDP_drawTextBG(BG_A, "Resting...", 10, 10);
		VDP_drawTextBG(BG_A, "Health:", 10, 11);
		sprintf(pHP, "%d", player_hp);
		
		if (player_hp < player_hp_max){
			player_hp++;
			VDP_drawTextBG(BG_A, pHP, 10, 12);
		} else {
			VDP_drawTextBG(BG_A, pHP, 10, 12);
			VDP_drawTextBG(BG_A, "Fully Restored", 10, 13);
		}
		waitMs(750);
	}
	if (JOY_readJoypad(JOY_1) & BUTTON_DOWN){
		break;
	}
}
	// Assume that when the player stops resting, they are leaving the house.
	// So we set the cooldown timer.
	playerHouseCooldown = PLAYER_HOUSE_COOLDOWN_MS;
	

	bPlayerCanMove = TRUE;
	SPR_setVisibility(player, VISIBLE);
	 bIsMoving = TRUE;
	 canFight = TRUE;
	bInsideHouse = FALSE;
	VDP_clearPlane(BG_A, TRUE);
	VDP_clearPlane(BG_B, TRUE);
	//load tileset
	VDP_loadTileSet(tileset1.tileset, 1, DMA);
	PAL_setPalette(PAL1, tileset1.palette->data, DMA);
	PAL_setPalette(PAL0, fg2.palette->data, DMA);
	PAL_setPalette(PAL3,palette_Font.data, DMA);
	PAL_setPalette(PAL2, our_sprite.palette->data, DMA);
	SYS_doVBlankProcess();
	PAL_setPalette(PAL3, merchantSprite.palette->data, DMA);
	displayRoom();
}

void updatePlayerHouseCooldown(u32 elapsedMs) {
    if(playerHouseCooldown > elapsedMs){
        playerHouseCooldown -= elapsedMs;
    } else {
        playerHouseCooldown = 0;
    }
}