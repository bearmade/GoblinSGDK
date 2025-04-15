
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
#include "../inc/dungeonGenerator.h"





u32 lastTime = 0;

int main()
{

    VDP_setScreenWidth256();//256 pixels / 8 = 32 tiles    
	VDP_setScreenHeight224();//224 pixels / 8 = 28 tiles
    //show sega logo 
    showSegaLogo();

    bMenuVisible = FALSE;
    bPlayerCanMove = TRUE;
    bShowMenu = FALSE;
    char numString[10];
    //display free ram 
//     u32 freemem = MEM_getFree();
//     VDP_drawTextBG(BG_B, "Free Mem: ", 10, 10);
//    sprintf(numString, "%d", freemem);
//    VDP_drawTextBG(BG_B, numString, 10, 12);
//     VDP_drawTextBG(BG_B, "Bytes", 10, 14);
    waitMs(2000);
    MAX_MERCHANT_INTERACTIONS = (random() % 10) + 5;

    VDP_loadFontData(tileset_Font.tiles, 96, CPU);
    
    PAL_setPalette(PAL1,palette_Font.data, DMA);


    //JOY_setEventHandler(joyEvent);
    bShowMenu = FALSE;
    
	displayTitle();
    


    VDP_loadTileSet(tileset2.tileset, 1, DMA);
    PAL_setPalette(PAL1, black_palette, DMA);
    

    delayCounter = 0;
    SYS_setVIntCallback(vblankCallback);
    //char numString[2];

    makeMap();
    //spawnCaveEntrances();


	
    bigMapCA();
	SPR_init();

	

    bShowMenu = FALSE;
    displayPlayer();
    PAL_fadeIn(32, 47,  our_sprite.palette->data, 60, 0);
    delayFrames(60);
    SPR_update();
    PAL_fadeIn(0, 15, fg2.palette->data, 60, 0);
    PAL_fadeIn(16, 31, tileset1.palette->data, 60, 0);
    //u16 myDungeon[DUNGEON_HEIGHT][DUNGEON_WIDTH];
    while(1)
    {
//         freemem = MEM_getFree();
//         VDP_drawTextBG(BG_B, "Free Mem: ", 10, 10);
//    sprintf(numString, "%d", freemem);
//    VDP_drawTextBG(BG_B, numString, 10, 12);
//     VDP_drawTextBG(BG_B, "Bytes", 10, 14);
updateCaves();
u32 currentTime = getTick();
u32 elapsed = currentTime - lastTime;
lastTime = currentTime;

updatePlayerHouseCooldown(elapsed);

      handleInput();
        collision();

        // if (isAnimating) {
        //     SPR_update();
        //     updateBattleAnimation(); 
        // }
        checkGameOverInput();


		camera();
        if(bShowMerchMenu){
            bIsMoving = FALSE;
            
            handleMerchantMenuInput();
        }
        if(bInsideHouse){
            bIsMoving = FALSE;
            canFight = FALSE;
            bPlayerCanMove = FALSE;
            SPR_setVisibility(player, HIDDEN);
        }


        if(bIsMoving == TRUE && canFight == TRUE){ //&& (!currentWorldX == 3 && !currentWorldY == 3)){
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
    if (inCave){
        VDP_setHilightShadow(0);
        PAL_setPalette(PAL0, fg1.palette->data, DMA);
    }
    else{
        VDP_setHilightShadow(1);
    }
    if (bBattleOngoing == TRUE){
        SPR_setVisibility(player, HIDDEN);
        SPR_update();
        updateBattleAnimation(); 
    }
    else{
        SPR_setVisibility(player, VISIBLE);
    }



    return (0);
}
