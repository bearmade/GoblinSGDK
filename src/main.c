
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

#define SFX_SEGA 65


u32 lastTime = 0;
void showSegaLogo() {
    // Load the logo image and palette
    PAL_setPalette(PAL2, segaLogo.palette->data, DMA);
    VDP_drawImageEx(BG_B, &segaLogo, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, 0), 0, 0, FALSE, TRUE);
    
    // Create an array to hold the scroll offsets for each line
    s16 scrollOffsets[224];
    
    // Initialize all lines to be off-screen (to the right)
    for (int i = 0; i < 224; i++) {
        scrollOffsets[i] = 256; // Start off-screen
    }
    
    // Apply initial scroll position
    VDP_setHorizontalScrollLine(BG_B, 0, scrollOffsets, 224, DMA);
    
    // Play the Sega sound
    XGM_setPCM(SFX_SEGA, sfx_sega, sizeof(sfx_sega));
    XGM_startPlayPCM(SFX_SEGA, 15, SOUND_PCM_CH2);
    
    // Cascade effect: gradually move each line into place
    for (int frame = 0; frame < 120; frame++) { // 120 frames for the animation
        // Update scrollOffsets based on current frame
        for (int line = 0; line < 224; line++) {
            // Create a delay effect where each line starts moving at a different time
            int lineDelay = line / 2; // Adjust this divisor to control cascade speed
            
            if (frame >= lineDelay) {
                // Calculate new position for this line
                int progress = frame - lineDelay;
                if (progress < 40) { // 40 frames for each line to move into place
                    scrollOffsets[line] = 256 - (progress * 16); // Gradually move from right to left
                } else {
                    scrollOffsets[line] = 0; // Final position
                }
            }
        }
        
        // Apply the updated scrolling
        VDP_setHorizontalScrollLine(BG_B, 0, scrollOffsets, 224, DMA);
        
        // Wait for next frame
        SYS_doVBlankProcess();
    }
    
    // Reset scrolling after the animation is complete
    VDP_setHorizontalScrollLine(BG_B, 0, NULL, 0, CPU);
    
    // Wait for the specified time
    delayFrames(240); 
    PAL_fadeOut(0, 63, 60, 1);
}
int main()
{

    VDP_setScreenWidth256();//256 pixels / 8 = 32 tiles    
	VDP_setScreenHeight224();//224 pixels / 8 = 28 tiles
    //show sega logo 
    showSegaLogo();


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


    JOY_setEventHandler(joyEvent);
    bShowMenu = FALSE;
    
	displayTitle();
    


    VDP_loadTileSet(tileset2.tileset, 1, DMA);
    PAL_setPalette(PAL1, tileset2.palette->data, DMA);
    

    delayCounter = 0;
    SYS_setVIntCallback(vblankCallback);
    //char numString[2];

    makeMap();
    spawnCaveEntrances();


    PAL_setPalette(PAL0, fg2.palette->data, DMA);
	bigMapCA();
	SPR_init();
	displayPlayer();

    bShowMenu = FALSE;

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





