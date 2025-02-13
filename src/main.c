
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
#include "../inc/battle.h"
#include "../inc/inventory.h"


int main()
{

    bPlayerCanMove = TRUE;
    bShowMenu = FALSE;
    char numString[10];
    //display free ram 
//     u32 freemem = MEM_getFree();
//     VDP_drawTextBG(BG_B, "Free Mem: ", 10, 10);
//    sprintf(numString, "%d", freemem);
//    VDP_drawTextBG(BG_B, numString, 10, 12);
//     VDP_drawTextBG(BG_B, "Bytes", 10, 14);
// waitMs(2000);
    

    VDP_loadFontData(tileset_Font.tiles, 96, CPU);
    PAL_setPalette(PAL1,palette_Font.data, DMA);

	VDP_setScreenWidth256();//256 pixels / 8 = 32 tiles    
	VDP_setScreenHeight224();//224 pixels / 8 = 28 tiles
    JOY_setEventHandler(joyEvent);
    bShowMenu = FALSE;
	displayTitle();



    VDP_loadTileSet(tileset2.tileset, 1, DMA);
    PAL_setPalette(PAL1, tileset2.palette->data, DMA);
    

    delayCounter = 0;
    SYS_setVIntCallback(vblankCallback);
    //char numString[2];
    makeMap();
    PAL_setPalette(PAL0, fg2.palette->data, DMA);
	bigMapCA();
	SPR_init();
	displayPlayer();

    bShowMenu = FALSE;


    while(1)
    {
//         freemem = MEM_getFree();
//         VDP_drawTextBG(BG_B, "Free Mem: ", 10, 10);
//    sprintf(numString, "%d", freemem);
//    VDP_drawTextBG(BG_B, numString, 10, 12);
//     VDP_drawTextBG(BG_B, "Bytes", 10, 14);

      handleInput();
        collision();

		camera();
        if(bShowMerchMenu){
            bIsMoving = FALSE;
            handleMerchantMenuInput();
        }


        if(bIsMoving == TRUE){
            randomEncounter();
        }
        if(bBattleStarted == TRUE){

            displayBattle();
            SYS_doVBlankProcess();
        }
        else if(bBattleStarted == FALSE){
            if (bBattleOngoing == FALSE){
                //XGM_stopPlay();
                //XGM_startPlay(world_vgm);
            		SPR_update();
                    SYS_doVBlankProcess();
            }
         
        }
      
    }
    if (bBattleOngoing == TRUE){
        SPR_setVisibility(player, HIDDEN);
    }
    else{
        SPR_setVisibility(player, VISIBLE);
    }
    return (0);
}





