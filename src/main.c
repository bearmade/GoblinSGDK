
#include <genesis.h>
#include <resources.h>
#include "../inc/level.h"
#include "../inc/debuginfo.h"
#include "../inc/player.h"
#include "../inc/camera.h"
#include "../inc/collision.h"
#include "../inc/globals.h"
#include "../inc/titlescreen.h"
#include "../inc/makemap.h"
#include "../inc/gamemanager.h"



void vblankCallback();
void inputHandler(u16 joy, u16 changed, u16 state);

//void initMap();
void showMenu();
void drawMap();


#define MAP_WIDTH 32
#define MAP_HEIGHT 20
#define SFX_SWOOSH 64

//u16 ind = TILE_USER_INDEX;
char map[MAP_WIDTH][MAP_HEIGHT];
u16 playerX, playerY;
u16 displayX, displayY;
u16 playerDir;
char tempMapA = ';';
char tempMapB = ';';
char tempMapC = ';';



char noS = 'n';
u16 sCount = 0;
u16 total_sCount = 0;
u16 floorCount = 0;

int main()
{
    //sramLoad();
    bPlayerCanMove = TRUE;
    bShowMenu = FALSE;
    XGM_setPCM(SFX_SWOOSH, sfx_swoosh, sizeof(sfx_swoosh));


    VDP_loadFontData(tileset_Font.tiles, 96, CPU);
    PAL_setPalette(PAL0,palette_Font.data, DMA);

    VDP_loadTileSet(tileset2.tileset, 1, DMA);
    PAL_setPalette(PAL1, tileset2.palette->data, DMA);
    
    //VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 2), 1, 1);
    
	VDP_setScreenWidth256();//256 pixels / 8 = 32 tiles    
	VDP_setScreenHeight224();//224 pixels / 8 = 28 tiles
    JOY_init();
    //JOY_setEventHandler(inputHandler);
    JOY_setEventHandler(joyEvent);
	displayTitle();
    
    //showMenu();
    delayCounter = 0;
    SYS_setVIntCallback(vblankCallback);
    char numString[2];
    //initMap();
    //drawMap();
    makeMap();
    PAL_setPalette(PAL0, fg2.palette->data, DMA);
	bigMapCA();
	SPR_init();
	displayPlayer();
    bShowMenu = FALSE;


    while(1)
    {
        //handleInput();
      	//if (attack_timer == 0) handleInput();
		//else if (attack_timer > 0 && attack_timer < attack_duration) attack_timer += 1;
		//else if (attack_timer == attack_duration) attack_timer = 0;

      handleInput();


        collision();
		camera();



		// char numberString[3];
		// sprintf(numberString, "%u", currentWorldX);
		// VDP_drawText(numberString, 24, 8);
		// sprintf(numberString, "%u", currentWorldY);
		// VDP_drawText(numberString, 26, 8);
		// sprintf(numberString, "%u", WORLD_LAYOUT[currentWorldY][currentWorldX]);
		// VDP_drawText(numberString, 28, 8);

      

        if(bIsMoving == TRUE){
            randomEncounter();
        }
        if(bBattleStarted == TRUE){
            displayBattle();
            SYS_doVBlankProcess();
        }
        else if(bBattleStarted == FALSE){
            if (bBattleOngoing == FALSE){
            		SPR_update();
                    SYS_doVBlankProcess();
            }
         
        }

    //SYS_doVBlankProcess();


    }
    return (0);
}
void inputHandler(u16 joy, u16 changed, u16 state){


        if (changed & state & BUTTON_START)
		{
       
            random_number = random() % 100;
			if(is_start_pressed){
                is_start_pressed = 0;
            }
            else {
                is_start_pressed = 1;
            }
		}
}









void vblankCallback() {
    if (delayCounter > 0) {
        delayCounter--;
    }
}
void showMenu() {
    
    VDP_drawText("Press STARTTTT to generate map", displayX, displayY + 3);
    
    while (1) {
        u16 value = JOY_readJoypad(JOY_1);
        
        if (value & BUTTON_START) {
			VDP_drawText("                            ", displayX, displayY + 3);
            break;  // Exit the menu loop when Start button is pressed
        }
        
        SYS_doVBlankProcess();
    }
}

// void initMap() {
//     for (u16 i = 0; i < MAP_WIDTH; i++) {
//         for (u16 j = 0; j < MAP_HEIGHT; j++) {

//             u16 random_number = random() % 100;
//             if (random_number > 75) {
//                 map[i][j] = '#';
//             }
//             else {
//                 map[i][j] = ' ';
//             }
//         }
//     }
// }   

void drawMap() {
    for (u16 i = 0; i < MAP_WIDTH; i++) {
        for (u16 j = 0; j < MAP_HEIGHT; j++) {
            if (map [i][j] == ' ') {
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 1), i, j);
            }
            else if (map[i][j] == '#') {
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 2), i, j);
            }
        
        }
    }

}
