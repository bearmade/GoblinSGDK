#include "../inc/titlescreen.h"
#include "../inc/globals.h"
#include <resources.h>
#include "../inc/player.h"
#include "../inc/battle.h"
#include "../inc/gamemanager.h"
#include <string.h>




bool showTitleScreen = TRUE;
char player_name1[11];
char player_name2[11];
char player_name3[11];
int loadSelection = 0;

void displayLoadMenu();
void playerNameInput();
void all3playerNames();

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

        if((value & BUTTON_START) || (value & BUTTON_B) || (value & BUTTON_A) || (value & BUTTON_C)){
            // Your existing START button logic
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

//show load menu

void displayLoadMenu(){
    PAL_setPalette(PAL1, titleBase.palette->data, DMA);
    VDP_drawImageEx(BG_B, &titleBase, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, ind), 0, 0, FALSE, TRUE);
    PAL_setPalette(PAL3, titleLetters.palette->data, DMA);
    VDP_drawImageEx(BG_A, &titleLetters, TILE_ATTR_FULL(PAL3, FALSE, FALSE, FALSE, 1024), 0, 0, FALSE, TRUE);
    waitMs(1000);
    // Draw menu once before the loop (shifted up by 3)
    PAL_setPalette(PAL0, palette_Font.data, DMA);
    drawBox(3, 20, 24, 8);
    all3playerNames();
    // draw each player name after slot number
    VDP_drawTextBG(BG_A, " Slot 1", 5, 21);
    VDP_drawTextBG(BG_A, player_name1, 13, 21);
    VDP_drawTextBG(BG_A, " Slot 2", 5, 23);
    VDP_drawTextBG(BG_A, player_name2, 13, 23);
    VDP_drawTextBG(BG_A, " Slot 3", 5, 25);
    VDP_drawTextBG(BG_A, player_name3, 13, 25);    
    // Before entering the input loop, initialize lastSelection to the default selection.
    int lastSelection = loadSelection;

    // Initial draw of the marker at the starting selection.
    VDP_drawTextBG(BG_A, "~", 4, (loadSelection * 2) + 21);

    while(1) {
        u16 value = JOY_readJoypad(JOY_1);
        
        // Handle up/down input
        if(value & BUTTON_UP) {
            if(loadSelection > 0) {
                loadSelection--;
                waitMs(100);
                SYS_doVBlankProcess(); // simple debouncing
            }
        }
        if(value & BUTTON_DOWN) {
            if(loadSelection < 2) {
                loadSelection++;
                waitMs(100);
                SYS_doVBlankProcess();
            }
        }
        
        if (loadSelection != lastSelection) {
            // Clear old selection marker.
            VDP_drawTextBG(BG_A, " ", 4, (lastSelection * 2) + 21);
            // Draw new selection marker.
            VDP_drawTextBG(BG_A, "~", 4, (loadSelection * 2) + 21);
            lastSelection = loadSelection;
        }

        if((value & BUTTON_START) || (value & BUTTON_B) || 
           (value & BUTTON_A) || (value & BUTTON_C)){
            // if(loadSelection == 0) {
            //     sramLoad(0);
            // } else if(loadSelection == 1) {
            //     sramLoad(1);
            // } else if(loadSelection == 2) {
            //     sramLoad(2);
            // }
            sramLoad(loadSelection);
            setRandomSeed(worldSeed);
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

        SYS_doVBlankProcess();
    }
}

void playerNameInput(){
    // disable player movement while editing name
    waitMs(1000);
    
    // Allowed characters: include a space, uppercase and lowercase letters.
    const char allowed_chars[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
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
    drawBox(9, 23, 14, 5);
    VDP_drawTextBG(BG_A, "Enter Name", 11, 24);
    VDP_drawTextBG(BG_A, player_name, 11, 26);
    // Create a marker string with spaces and mark the current letter.
    char marker[11] = "          ";
    marker[currentLetter] = '.';
    VDP_drawTextBG(BG_A, marker, 11, 27);
    
    while(1) {
        u16 value = JOY_readJoypad(JOY_1);
        
        // Allow moving the selection left/right.
        if(value & BUTTON_LEFT) {
            if(currentLetter > 0) { 
                currentLetter--; 
                waitMs(200);
            }
        }
        if(value & BUTTON_RIGHT) {
            if(currentLetter < 9) { 
                currentLetter++; 
                waitMs(200);
            }
        }
        
        // Cycle letter up/down using our allowed_chars array.
        if(value & BUTTON_UP) {
            // Increment the index, wrapping around
            allowed_index[currentLetter] = (allowed_index[currentLetter] + 1) % num_allowed;
            player_name[currentLetter] = allowed_chars[allowed_index[currentLetter]];
            waitMs(200);
        }
        if(value & BUTTON_DOWN) {
            // Decrement the index with wrap-around
            allowed_index[currentLetter] = (allowed_index[currentLetter] - 1 + num_allowed) % num_allowed;
            player_name[currentLetter] = allowed_chars[allowed_index[currentLetter]];
            waitMs(200);
        }
        
        // Redraw the current player name.
        VDP_drawTextBG(BG_A, "          ", 11, 26); // clear the previous text
        VDP_drawTextBG(BG_A, player_name, 11, 26);
        
        // Update the marker below the current letter.
        for(int i = 0; i < 10; i++){
            marker[i] = ' ';
        }
        marker[currentLetter] = '.';
        VDP_drawTextBG(BG_A, marker, 11, 27);
        
        // Confirm input with START or A button
        if(value & BUTTON_START || value & BUTTON_A) {
            cheatCode();
            break;
        }
        cheatCode();
        SYS_doVBlankProcess();
    }
    
    // Here you could copy player_name into a global variable if needed.
    
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
    for(u8 i = 0; i < sizeof(player_name1); i++){
        player_name1[i] = SRAM_readByte(38 + i);
    }
    for(u8 i = 0; i < sizeof(player_name2); i++){
        player_name2[i] = SRAM_readByte(68 + 38 + i);
    }
    for(u8 i = 0; i < sizeof(player_name3); i++){
        player_name3[i] = SRAM_readByte(136 + 38 + i);
    }
    SRAM_disable();
}

void cheatCode(){
    if(strcmp(player_name, "HELP ME   ") == 0){
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
    if(strcmp(player_name, "MACHOMAN  ") == 0){

        player_attack = 100;
        player_defense = 100;
    }
}

