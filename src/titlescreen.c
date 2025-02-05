#include "../inc/titlescreen.h"
#include "../inc/globals.h"
#include <resources.h>
#include "../inc/player.h"
#include "../inc/battle.h"

bool showTitleScreen = TRUE;

void displayTitle(){

    XGM_startPlay(title_vgm);
    //VDP_clearPlane(BG_A, TRUE);
            PAL_setPalette(PAL1, title.palette->data, DMA);
	    VDP_drawImageEx(BG_B, &title, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, ind), 0, 0, FALSE, TRUE);
       SYS_doVBlankProcess();

    while (1) {

        u16 value = JOY_readJoypad( JOY_1);
    	//  if((value & BUTTON_DOWN)){
	    // 	    selection = !selection;
	    //     }
	    //     if((value & BUTTON_UP)){
		//         selection = !selection;
	    //     }
        if(value & BUTTON_START){
            if(selection == 0){
                worldSeed = random();
                setRandomSeed(worldSeed);
                showTitleScreen = FALSE;
                bPlayerCanMove = TRUE;
                bShowMenu = FALSE;
                //break;
            }
            else{
                sramLoad();
                setRandomSeed(worldSeed);
                 showTitleScreen = FALSE;
                bPlayerCanMove = TRUE;
                bShowMenu = FALSE;
                //break;
            }
        }
        		//get player selection
		VDP_drawTextBG( BG_A, " ", 11, ((selection == 0 ? 1 : 0)*2 + 24)); // Clear old cursor
		VDP_drawTextBG( BG_A, "~", 11, ((selection*2) + 24)); // Draw new cursor

        //delayFrames(3);
        SYS_doVBlankProcess();
           
        //u16 value = JOY_readJoypad(JOY_1);

        //VDP_loadFontData(tileset_Font.tiles, 96, CPU);
	    PAL_setPalette(PAL2,palette_Font.data, DMA);
        VDP_drawTextBG( BG_A, "Bear Made Games", 10, 20);
        VDP_drawTextBG( BG_A, "     2025     ", 10, 22);
        VDP_drawTextBG( BG_A, "   ", 12, 24);
        //show load game or new game options
        VDP_drawTextBG( BG_A, " New Game", 12, 24);
        VDP_drawTextBG( BG_A, " Load Game", 12, 26);


        if (showTitleScreen == FALSE) {
            
			//VDP_drawText("Press START to generate map", 5, 5);
            VDP_clearTileMap(BG_B,ind,0, TRUE);
            //XGM_stopPlay();
                XGM_stopPlay();
    XGM_startPlay(world_vgm);
            
            break;  // Exit the menu loop when Start button is pressed
        }

        SYS_doVBlankProcess();
    }

}
