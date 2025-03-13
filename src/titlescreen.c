#include "../inc/titlescreen.h"
#include "../inc/globals.h"
#include <resources.h>
#include "../inc/player.h"
#include "../inc/battle.h"
#include "../inc/gamemanager.h"
#include <string.h>




bool showTitleScreen = TRUE;
bool bMenuVisible = FALSE;
char player_name1[11];
char player_name2[11];
char player_name3[11];
int loadSelection = 0;
int hoffset = 0;

void displayLoadMenu();
void playerNameInput();
void all3playerNames();
void drawBoxScroll(u16 x, u16 y, u16 width, u16 height);



void displayTitle(){
     bShowMenu = FALSE;
    XGM_startPlay(title_vgm);
    //PAL_fadeAll(0, 63,   60, 1);
    
    


    VDP_clearPlane(BG_A, TRUE);
    VDP_clearPlane(BG_B, TRUE);
    drawBoxScroll(7, 9, 19, 3);
    VDP_drawTextBG(BG_A, "BEARMADE>PRESENTS", 8, 10);
    PAL_fadeIn(0, 63,  palette_Font.data, 60, 1);
    delayFrames(340);
   
    VDP_drawTextBG(BG_A, ">>>>>>>>>>>>>>>>>", 8, 10);
    PAL_setPalette(PAL1, black_palette, DMA);
    PAL_setPalette(PAL3, black_palette, DMA);
    

    
    //PAL_setPalette(PAL1, titleBase.palette->data, DMA);
    VDP_drawImageEx(BG_B, &titleBase, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, ind), 0, 0, FALSE, TRUE);
    //PAL_setPalette(PAL3, titleLetters.palette->data, DMA);
    VDP_drawImageEx(BG_A, &titleLetters, TILE_ATTR_FULL(PAL3, FALSE, FALSE, FALSE, 1024), 0, 0, FALSE, TRUE);
    
    // Draw menu once before the loop
    PAL_setPalette(PAL0, palette_Font.data, DMA);

    // First fade in just the base
    PAL_fadeIn(16, 32, titleBase.palette->data, 60, 0);  // Changed to 1 to wait for completion

    // Wait for a short delay before fading in the letters
    delayFrames(30);  // Optional delay between fades

    // Then fade in the title letters
    PAL_fadeIn(48, 63, titleLetters.palette->data, 60, 1);  // Changed to 1 to wait for completion
    delayFrames(60);
    drawBoxScroll(9, 23, 14, 5);
    VDP_drawTextBG(BG_A, ">New>Game", 11, 24);
    VDP_drawTextBG(BG_A, ">Load>Game", 11, 26);
    delayFrames(30);
    bMenuVisible = TRUE;
    JOY_setEventHandler(joyEvent);
    while(1) {
        hoffset--;
        // if(hoffset > 160) {
        //     hoffset = 0;
        // }
        
        VDP_setHorizontalScroll(BG_B, hoffset);
       
        u16 value = JOY_readJoypad(JOY_1);
        
        VDP_drawTextBG(BG_A, ">", 10, ((selection == 0 ? 1 : 0)*2 + 24));
        VDP_drawTextBG(BG_A, "~", 10, ((selection*2) + 24));
if (bMenuVisible)
{
    


        if((value & BUTTON_START) || (value & BUTTON_B) || (value & BUTTON_A) || (value & BUTTON_C)){
            
            if(selection == 0) {
                playerNameInput();
                worldSeed = random();
                setRandomSeed(worldSeed);
                cheatCode();
            } else {
                //sramLoad(0);
                //setRandomSeed(worldSeed);
                displayLoadMenu();
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
}

//show load menu

void displayLoadMenu(){
    PAL_setPalette(PAL1, titleBase.palette->data, DMA);
    VDP_drawImageEx(BG_B, &titleBase, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, ind), 0, 0, FALSE, TRUE);
    PAL_setPalette(PAL3, titleLetters.palette->data, DMA);
    VDP_drawImageEx(BG_A, &titleLetters, TILE_ATTR_FULL(PAL3, FALSE, FALSE, FALSE, 1024), 0, 0, FALSE, TRUE);
    //waitMs(1000);
    delayWithScrolling(10);
    // Draw menu once before the loop (shifted up by 3)
    PAL_setPalette(PAL0, palette_Font.data, DMA);
    drawBoxScroll(3, 20, 24, 8);
    all3playerNames();
    // draw each player name after slot number
    VDP_drawTextBG(BG_A, ">Slot>(", 5, 21);
    VDP_drawTextBG(BG_A, player_name1, 13, 21);
    VDP_drawTextBG(BG_A, ">Slot>)", 5, 23);
    VDP_drawTextBG(BG_A, player_name2, 13, 23);
    VDP_drawTextBG(BG_A, ">Slot>*", 5, 25);
    VDP_drawTextBG(BG_A, player_name3, 13, 25);    
    // Before entering the input loop, initialize lastSelection to the default selection.
    int lastSelection = loadSelection;

    // Initial draw of the marker at the starting selection.
    VDP_drawTextBG(BG_A, "~", 4, (loadSelection * 2) + 21);

    while(1) {
        hoffset--;
        VDP_setHorizontalScroll(BG_B, hoffset);

        u16 value = JOY_readJoypad(JOY_1);
        
        // Handle up/down input
        if(value & BUTTON_UP) {
            if(loadSelection > 0) {
                loadSelection--;
                //waitMs(100);
                delayWithScrolling(10);
                SYS_doVBlankProcess(); // simple debouncing
            }
        }
        if(value & BUTTON_DOWN) {
            if(loadSelection < 2) {
                loadSelection++;
                //waitMs(100);
                delayWithScrolling(10);
                SYS_doVBlankProcess();
            }
        }
        
        if (loadSelection != lastSelection) {
            // Clear old selection marker.
            VDP_drawTextBG(BG_A, ">", 4, (lastSelection * 2) + 21);
            // Draw new selection marker.
            VDP_drawTextBG(BG_A, "~", 4, (loadSelection * 2) + 21);
            lastSelection = loadSelection;
        }

        if((value & BUTTON_START) || (value & BUTTON_B) || 
           (value & BUTTON_A)){
            if(loadSelection == 0) {
                // Check if name is empty or only contains spaces/null bytes
                bool isEmpty = TRUE;
                for(int i = 0; i < 10; i++) {
                    if(player_name1[i] != ' ' && player_name1[i] != '\0') {
                        isEmpty = FALSE;
                        break;
                    }
                }
                
                if(isEmpty) {
                    playerNameInput();
                } else {
                    sramLoad(loadSelection);
                    setRandomSeed(worldSeed);
                }
            } else if(loadSelection == 1) {
                // Check if name is empty or only contains spaces/null bytes
                bool isEmpty = TRUE;
                for(int i = 0; i < 10; i++) {
                    if(player_name2[i] != '>' && player_name2[i] != '\0') {
                        isEmpty = FALSE;
                        break;
                    }
                }
                
                if(isEmpty) {
                    playerNameInput();
                } else {
                    sramLoad(loadSelection);
                    setRandomSeed(worldSeed);
                }
            } else if(loadSelection == 2) {
                // Check if name is empty or only contains spaces/null bytes
                bool isEmpty = TRUE;
                for(int i = 0; i < 10; i++) {
                    if(player_name3[i] != ' ' && player_name3[i] != '\0') {
                        isEmpty = FALSE;
                        break;
                    }
                }
                
                if(isEmpty) {
                    playerNameInput();
                } else {
                    sramLoad(loadSelection);
                    setRandomSeed(worldSeed);
                }
            }

            showTitleScreen = FALSE;
            bPlayerCanMove = TRUE;
            bShowMenu = FALSE;
            // Clear and start world music.
            VDP_clearTileMap(BG_B, ind, 0, TRUE);
            VDP_clearTileMap(BG_A, ind, 0, TRUE);
            XGM_stopPlay();
            XGM_startPlay(world_vgm);
            break;
        }
        if (value & BUTTON_C){
           // go back to title screen
           showTitleScreen = TRUE;
           bPlayerCanMove = FALSE;
           bShowMenu = TRUE;
           break;  // Added break to exit this loop when user presses C
        }

        SYS_doVBlankProcess();
    }
}

void playerNameInput(){
    // disable player movement while editing name
    //waitMs(1000);
    delayWithScrolling(10);
    // Allowed characters: include a space, uppercase and lowercase letters.
    const char allowed_chars[] = ">ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    const int num_allowed = sizeof(allowed_chars) - 1; // exclude null terminator
    
    // Set up the name input prompt and default name.
    //char player_name[11] = "AAAAAAAAAA"; // initial value of 10 letters
    // Instead of using player_name characters directly, track an index into allowed_chars
    int allowed_index[10];
    // Here 'A' is at index 1 in allowed_chars (since index 0 is space)
    for (int i = 0; i < 10; i++){
        allowed_index[i] = 1;
    }
    
    int currentLetter = 0;         // index that the player is editing
    
    VDP_clearTextLine(21);
    VDP_clearTextLine(22);
    VDP_clearTextLine(23);
    VDP_clearTextLine(24);
    VDP_clearTextLine(25);
    VDP_clearTextLine(26);
    VDP_clearTextLine(27);
    VDP_clearTextLine(28);
    // Draw the prompt and an editing box.
    PAL_setPalette(PAL0, palette_Font.data, DMA);
    drawBoxScroll(9, 22, 14, 6);
    VDP_drawTextBG(BG_A, "Enter>Name", 11, 23);
    VDP_drawTextBG(BG_A, player_name, 11, 25);
    // Create a marker string with spaces and mark the current letter.
    char marker[11] = ">>>>>>>>>>";
    marker[currentLetter] = '.';
    VDP_drawTextBG(BG_A, marker, 11, 26);
    
    while(1) {
        hoffset--;
        VDP_setHorizontalScroll(BG_B, hoffset);
        u16 value = JOY_readJoypad(JOY_1);
        
        // Allow moving the selection left/right.
        if(value & BUTTON_LEFT) {
            if(currentLetter > 0) { 
                currentLetter--; 
                //waitMs(200);
                delayWithScrolling(10);
            }
        }
        if(value & BUTTON_RIGHT) {
            if(currentLetter < 9) { 
                currentLetter++; 
                //waitMs(200);
                delayWithScrolling(10);
            }
        }
        
        // Cycle letter up/down using our allowed_chars array.
        if(value & BUTTON_UP) {
            // Increment the index, wrapping around
            allowed_index[currentLetter] = (allowed_index[currentLetter] + 1) % num_allowed;
            player_name[currentLetter] = allowed_chars[allowed_index[currentLetter]];
            //waitMs(200);
            delayWithScrolling(10);
        }
        if(value & BUTTON_DOWN) {
            // Decrement the index with wrap-around
            allowed_index[currentLetter] = (allowed_index[currentLetter] - 1 + num_allowed) % num_allowed;
            player_name[currentLetter] = allowed_chars[allowed_index[currentLetter]];
            //waitMs(200);
            delayWithScrolling(10);
        }
        
        // Redraw the current player name.
        VDP_drawTextBG(BG_A, ">>>>>>>>>>", 11, 25); // clear the previous text
        VDP_drawTextBG(BG_A, player_name, 11, 25);
        
        // Update the marker below the current letter.
        for(int i = 0; i < 10; i++){
            marker[i] = '>';
        }
        marker[currentLetter] = '.';
        VDP_drawTextBG(BG_A, marker, 11, 26);
        
        // Confirm input with START or A button
        if(value & BUTTON_START || value & BUTTON_A) {
            // Add check for SOUNDTEST name
            if(strcmp(player_name, "HEARMEOUT>") == 0){
                // Show sound test menu
                displaySoundTestMenu();
                // After returning from sound test menu, go back to title
                showTitleScreen = TRUE;
                displayTitle();
                return;
            }
            
            cheatCode();
            break;
        }
        cheatCode();
        SYS_doVBlankProcess();
    }
    
 
    
    showTitleScreen = FALSE;
    bPlayerCanMove = TRUE;
    bShowMenu = FALSE;
    
    // Clear the screen and switch to world music.
    VDP_clearTileMap(BG_B, ind, 0, TRUE);
    VDP_clearTileMap(BG_A, ind, 0, TRUE);
    XGM_stopPlay();
    XGM_startPlay(world_vgm);
}
void all3playerNames(){
    SRAM_enable();
    
    // Read player_name1
    for(u8 i = 0; i < sizeof(player_name1) - 1; i++){
        player_name1[i] = SRAM_readByte(38 + i);
    }
    player_name1[10] = '\0';  // Ensure null-termination
    
    // Read player_name2
    for(u8 i = 0; i < sizeof(player_name2) - 1; i++){
        player_name2[i] = SRAM_readByte(68 + 38 + i);
    }
    player_name2[10] = '\0';  // Ensure null-termination
    
    // Read player_name3
    for(u8 i = 0; i < sizeof(player_name3) - 1; i++){
        player_name3[i] = SRAM_readByte(136 + 38 + i);
    }
    player_name3[10] = '\0';  // Ensure null-termination
    
    SRAM_disable();
}

void cheatCode(){
    if(strcmp(player_name, "HELP>ME>>>") == 0){
        player_hp = 9999;
        player_hp_max = 9999;
        player_level = 1;
        player_attack = 99;
        player_defense = 99;

        player_gold = 9999;
    }
    if(strcmp(player_name, "GIMMEDCASH") == 0){

        player_gold = 10000;
    }
    if(strcmp(player_name, "MACHOMAN>>") == 0){

        player_attack = 100;
        player_defense = 100;
    }
    if(strcmp(player_name, "IWANTDEATH") == 0){
        player_hp = 1;
        
    }
    if(strcmp(player_name, "JOHN>CENA>") == 0){
        player_attack = 100;
        player_defense = 100;
    }

}







void displaySoundTestMenu() {
    // List of songs to test
    const char* songNames[] = {
        "Initial>Fantasy",
        "I>Killed>a>Goblin",
        "Goblin>Emergence",
        "Death>of>a>Goblin",
        "Goblin's>Ghost",
        "Goblin>Nest",
        "Victor>Goblin"

    };
    
    // Corresponding VGM resources
    const void* songResources[] = {
        title_vgm,
        world_vgm,
        battle_vgm,
        victory_vgm,
        world2_vgm,
        cave_vgm,
        gameOver_vgm
    };
    
    const int songCount = 7;
    int selection = 0;
    int lastSelection = -1;
    bool playing = FALSE;
    
    // Clear screens
    VDP_clearPlane(BG_A, TRUE);
    VDP_clearPlane(BG_B, TRUE);
    
    // Set up background
    PAL_setPalette(PAL1, titleBase.palette->data, DMA);
    VDP_drawImageEx(BG_B, &titleBase, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, ind), 0, 0, FALSE, TRUE);
    
    // Draw sound test menu
    PAL_setPalette(PAL0, palette_Font.data, DMA);
    drawBoxScroll(5, 10, 22, 17);
    VDP_drawTextBG(BG_A, "SOUND>TEST", 11, 11);
    VDP_drawTextBG(BG_A, "..........", 11, 12);
    
    // Draw initial list
    for(int i = 0; i < songCount; i++) {
        VDP_drawTextBG(BG_A, songNames[i], 8, 14 + i);
    }
    
    // Draw instructions
    VDP_drawTextBG(BG_A, "A:>Play>>C:>Exit", 8, 24);
    
    while(1) {
        hoffset--;
        VDP_setHorizontalScroll(BG_B, hoffset);
        u16 value = JOY_readJoypad(JOY_1);
        
        // Handle up/down navigation
        if(value & BUTTON_UP) {
            if(selection > 0) {
                selection--;
                //waitMs(200);
                delayWithScrolling(10);
            }
        }
        if(value & BUTTON_DOWN) {
            if(selection < songCount - 1) {
                selection++;
                //waitMs(200);
                delayWithScrolling(10);
            }
        }
        
        // Update selection marker if needed
        if(selection != lastSelection) {
            // Clear old selection
            if(lastSelection >= 0) {
                VDP_drawTextBG(BG_A, ">>", 6, 14 + lastSelection);
            }
            
            // Draw new selection
            VDP_drawTextBG(BG_A, "~", 6, 14 + selection);
            lastSelection = selection;
        }
        
        // Play selected music
        if((value & BUTTON_A) || (value & BUTTON_B) || (value & BUTTON_START)) {
            XGM_stopPlay();
            XGM_startPlay(songResources[selection]);
            playing = TRUE;
            //waitMs(200);
            delayWithScrolling(10);
        }
        
        // Return to title screen
        if(value & BUTTON_C) {
            SYS_hardReset();
            XGM_stopPlay();
            showTitleScreen = TRUE;
            displayTitle();
            break;
        }
        
        SYS_doVBlankProcess();
    }
}
void drawBoxScroll(u16 x, u16 y, u16 width, u16 height) {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			// Fill center
			VDP_setTileMapXY(3, TILE_ATTR_FULL(PAL0, 1, 0, 0, 1470), x+i, y+j);
			
			// Top edge
			if (j == 0) VDP_setTileMapXY(3, TILE_ATTR_FULL(PAL0, 1, 0, 0, 1468), x+i, y);
			
			// Bottom edge - vertically flipped
			if (j == height-1) VDP_setTileMapXY(3, TILE_ATTR_FULL(PAL0, 1, 1, 0, 1468), x+i, y+j);
			
			// Left edge
			if (i == 0) VDP_setTileMapXY(3, TILE_ATTR_FULL(PAL0, 1, 0, 0, 1469), x, y+j);
			
			// Right edge - horizontally flipped
			if (i == width-1) VDP_setTileMapXY(3, TILE_ATTR_FULL(PAL0, 1, 0, 1, 1469), x+i, y+j);
			
			// Corners
			if (j == 0 && i == 0) VDP_setTileMapXY(3, TILE_ATTR_FULL(PAL0, 1, 0, 0, 1467), x, y);
			if (j == 0 && i == width-1) VDP_setTileMapXY(3, TILE_ATTR_FULL(PAL0, 1, 0, 1, 1467), x+i, y);
			if (j == height-1 && i == 0) VDP_setTileMapXY(3, TILE_ATTR_FULL(PAL0, 1, 1, 0, 1467), x, y+j);
			if (j == height-1 && i == width-1) VDP_setTileMapXY(3, TILE_ATTR_FULL(PAL0, 1, 1, 1, 1467), x+i, y+j);
			//add drop shadow to bottom and right using "^"
			if (j == height-1) VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL0, 0, 0, 0, 1502), x+i+1, y+j+1);
			if (i == width-1) VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL0, 0, 0, 0, 1502), x+i+1, y+j+1);

			
		}
	}
}
void delayWithScrolling(u16 frames) {
    u16 frameCount = 0;
    while(frameCount < frames) {
        hoffset--;
        VDP_setHorizontalScroll(BG_B, hoffset);
        SYS_doVBlankProcess();
        frameCount++;
    }
}