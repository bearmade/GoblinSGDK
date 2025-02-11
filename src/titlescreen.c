#include "../inc/titlescreen.h"
#include "../inc/globals.h"
#include <resources.h>
#include "../inc/player.h"
#include "../inc/battle.h"

bool showTitleScreen = TRUE;

void displayTitle(){
     bShowMenu = FALSE;
    XGM_startPlay(title_vgm);
    PAL_setPalette(PAL1, titleBase.palette->data, DMA);
    VDP_drawImageEx(BG_B, &titleBase, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, ind), 0, 0, FALSE, TRUE);
    PAL_setPalette(PAL3, titleLetters.palette->data, DMA);
    VDP_drawImageEx(BG_A, &titleLetters, TILE_ATTR_FULL(PAL3, FALSE, FALSE, FALSE, 1024), 0, 0, FALSE, TRUE);
    
    // Draw menu once before the loop
    PAL_setPalette(PAL0, palette_Font.data, DMA);
    drawBox(9, 23, 14, 5);
    VDP_drawTextBG(BG_A, " New Game", 11, 24);
    VDP_drawTextBG(BG_A, " Load Game", 11, 26);
    
    while(1) {
        // Only handle cursor and input logic here
        u16 value = JOY_readJoypad(JOY_1);
        
        VDP_drawTextBG(BG_A, " ", 10, ((selection == 0 ? 1 : 0)*2 + 24));
        VDP_drawTextBG(BG_A, "~", 10, ((selection*2) + 24));

        if(value & BUTTON_START) {
            // Your existing START button logic
            if(selection == 0) {
                worldSeed = random();
                setRandomSeed(worldSeed);
            } else {
                sramLoad();
                setRandomSeed(worldSeed);
            }
            showTitleScreen = FALSE;
            bPlayerCanMove = TRUE;
            bShowMenu = FALSE;
        }

        if(showTitleScreen == FALSE) {
             bShowMenu = FALSE;
            VDP_clearTileMap(BG_B, ind, 0, TRUE);
            VDP_clearTileMap(BG_A, ind, 0, TRUE);
            XGM_stopPlay();
            XGM_startPlay(world_vgm);
            break;
        }

        SYS_doVBlankProcess();
    }
}
