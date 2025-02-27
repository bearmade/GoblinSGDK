#include "../inc/player.h"
#include "../inc/dungeonGenerator.h"
#include "../inc/makemap.h"
#include "../inc/level.h"
#include "../inc/globals.h"
#include "../inc/camera.h"
#include "../inc/collision.h"
#include <genesis.h>


// Keep track of current location (world or cave)
static bool inCave = FALSE;
// Store current cave level
static u16 caveLevel = 1;
// Save world position when entering cave for use when exiting
static u16 savedWorldX = 0;
static u16 savedWorldY = 0;
// Backup of the LEVEL_TILES for restoring when exiting cave
static u8 BACKUP_TILES[14][16] = {{0}};

// Generates cave wall pattern
void makeCaveWalls(u16 yy, u16 xx, u16 mapheight, u16 mapwidth) {
    // Initialize with random cave walls
    for (yy = 0; yy < mapheight; yy++) { 
        for (xx = 0; xx < mapwidth; xx++) { 
            if (random() % 100 >= 55) {
                LEVEL_TILES[yy][xx] = 1; // Wall tile
            } else {
                LEVEL_TILES[yy][xx] = 0; // Floor tile
            }
        }
    }
    
    // Cellular automata to smooth the caves (run multiple passes)
    for (u16 passes = 0; passes < 5; passes++) {
        u8 tempTiles[14][16] = {{0}};
        
        for (yy = 1; yy < mapheight - 1; yy++) {
            for (xx = 1; xx < mapwidth - 1; xx++) {
                u16 neighbors = countNeighbors(1, yy, xx);
                
                // Apply cellular automata rules
                if (LEVEL_TILES[yy][xx] == 1) {
                    // Wall remains if it has enough wall neighbors
                    tempTiles[yy][xx] = (neighbors >= 4) ? 1 : 0;
                } else {
                    // Floor becomes wall if too many adjacent walls
                    tempTiles[yy][xx] = (neighbors >= 5) ? 1 : 0;
                }
            }
        }
        
        // Copy temporary grid back to main grid
        for (yy = 1; yy < mapheight - 1; yy++) {
            for (xx = 1; xx < mapwidth - 1; xx++) {
                LEVEL_TILES[yy][xx] = tempTiles[yy][xx];
            }
        }
    }
    
    // Ensure cave border is solid
    for (yy = 0; yy < mapheight; yy++) {
        LEVEL_TILES[yy][0] = 1;
        LEVEL_TILES[yy][mapwidth-1] = 1;
    }
    for (xx = 0; xx < mapwidth; xx++) {
        LEVEL_TILES[0][xx] = 1;
        LEVEL_TILES[mapheight-1][xx] = 1;
    }
}

// Place cave features like treasures and monsters
void placeCaveFeatures(u16 mapheight, u16 mapwidth) {
    u16 numFeatures = 3 + (random() % 3); // 3-5 features
    
    for (u16 i = 0; i < numFeatures; i++) {
        u16 featureType = random() % 3; // 0=crystals, 1=water pools, 2=treasure
        u16 attempts = 0;
        
        while (attempts < 20) { // Try up to 20 times to place the feature
            u16 posY = 2 + (random() % (mapheight - 4));
            u16 posX = 2 + (random() % (mapwidth - 4));
            
            // Check if area is open (no walls or other features)
            if (LEVEL_TILES[posY][posX] == 0) {
                bool canPlace = TRUE;
                
                // Check 3x3 area around feature
                for (s16 dy = -1; dy <= 1; dy++) {
                    for (s16 dx = -1; dx <= 1; dx++) {
                        if (LEVEL_TILES[posY+dy][posX+dx] != 0) {
                            canPlace = FALSE;
                            break;
                        }
                    }
                    if (!canPlace) break;
                }
                
                if (canPlace) {
                    switch(featureType) {
                        case 0: // Crystal formations (using tile 3)
                            LEVEL_TILES[posY][posX] = 3;
                            break;
                        case 1: // Water pools (using tile 2)
                            LEVEL_TILES[posY][posX] = 2;
                            LEVEL_TILES[posY][posX+1] = 2;
                            LEVEL_TILES[posY+1][posX] = 2;
                            LEVEL_TILES[posY+1][posX+1] = 2;
                            break;
                        case 2: // Treasure (using tile 6)
                            LEVEL_TILES[posY][posX] = 6;
                            break;
                    }
                    break; // Feature placed successfully
                }
            }
            attempts++;
        }
    }
}

// Place entrance/exit for the cave
void placeCaveExit(u16 mapheight, u16 mapwidth) {
    bool exitPlaced = FALSE;
    u16 attempts = 0;
    
    while (!exitPlaced && attempts < 50) {
        u16 posY = 2 + (random() % (mapheight - 4));
        u16 posX = 2 + (random() % (mapwidth - 4));
        
        if (LEVEL_TILES[posY][posX] == 0) {
            // Check if there's enough open space around the exit
            if (LEVEL_TILES[posY-1][posX] == 0 && LEVEL_TILES[posY+1][posX] == 0 &&
                LEVEL_TILES[posY][posX-1] == 0 && LEVEL_TILES[posY][posX+1] == 0) {
                
                // Place exit ladder (using tile 9)
                LEVEL_TILES[posY][posX] = 16;
                
                // Place a bit of open space around exit
                for (s16 dy = -1; dy <= 1; dy++) {
                    for (s16 dx = -1; dx <= 1; dx++) {
                        if (LEVEL_TILES[posY+dy][posX+dx] == 1) {
                            LEVEL_TILES[posY+dy][posX+dx] = 0;
                        }
                    }
                }
                
                exitPlaced = TRUE;
            }
        }
        attempts++;
    }
    
    // Fallback: If we couldn't place an exit, force one in the middle
    if (!exitPlaced) {
        u16 midY = mapheight / 2;
        u16 midX = mapwidth / 2;
        LEVEL_TILES[midY][midX] = 16; // Exit tile
        
        // Ensure there's space around the exit
        for (s16 dy = -1; dy <= 1; dy++) {
            for (s16 dx = -1; dx <= 1; dx++) {
                LEVEL_TILES[midY+dy][midX+dx] = 0;
            }
        }
    }
}

// Generate cave dungeon level
void generateCaveLevel(u16 level) {
    u16 mapheight = 14;
    u16 mapwidth = 16;
    
    // Set RNG seed based on world seed and cave level for deterministic generation
    //setRandomSeed(worldSeed + caveLevel);
    
    // Initialize cave map
    for (u16 y = 0; y < mapheight; y++) {
        for (u16 x = 0; x < mapwidth; x++) {
            LEVEL_TILES[y][x] = 0;
        }
    }
    
    // Generate cave layout
    makeCaveWalls(0, 0, mapheight, mapwidth);
    
    // Add player spawn point
    u16 startY = mapheight / 2;
    u16 startX = mapwidth / 2;
    
    // Make sure player spawn is clear
    for (s16 dy = -2; dy <= 2; dy++) {
        for (s16 dx = -2; dx <= 2; dx++) {
            if (startY+dy >= 0 && startY+dy < mapheight && startX+dx >= 0 && startX+dx < mapwidth) {
                LEVEL_TILES[startY+dy][startX+dx] = 0;
            }
        }
    }
    
    // Place player (using tile 10)
    LEVEL_TILES[startY][startX] = 10;
    
    // Add cave features
    placeCaveFeatures(mapheight, mapwidth);
    
    // Add cave exit
    placeCaveExit(mapheight, mapwidth);
    
    // Ensure there's a valid path from player to exit
    ensurePathToExit(mapheight, mapwidth);
    makeWall(yy, xx, mapheight, mapwidth, 0, 1);
    makeWall(yy, xx, mapheight, mapwidth, 1, 1);
    makeWall(yy, xx, mapheight, mapwidth, 2, 1);
    makeWall(yy, xx, mapheight, mapwidth, 3, 1);
    //ruleTile(mapheight, mapwidth, 1);
    // Convert map arrays
    convertMapArrays();
}

// Save current world state before entering cave
void backupWorldState() {
    for (u16 y = 0; y < 14; y++) {
        for (u16 x = 0; x < 16; x++) {
            BACKUP_TILES[y][x] = LEVEL_TILES[y][x];
        }
    }
    
    // Save current world position
    savedWorldX = currentWorldX;
    savedWorldY = currentWorldY;
}

// Restore world state when exiting cave
void restoreWorldState() {
    for (u16 y = 0; y < 14; y++) {
        for (u16 x = 0; x < 16; x++) {
            LEVEL_TILES[y][x] = BACKUP_TILES[y][x];
        }
    }
    
    // Restore world position
    currentWorldX = savedWorldX;
    currentWorldY = savedWorldY;
    
    // Restore the original room data
    updateCurrentRoom();
    
    // Convert map arrays
    convertMapArrays();
}

// Enter a cave at a specified level
void enterCave(u16 level) {
    if (!inCave) {
        // Backup the current world state
        backupWorldState();
        
        // Set cave state
        inCave = TRUE;
        caveLevel = level;
        
        // Generate cave level
        generateCaveLevel(caveLevel);
        
        // Set player position to the cleared spawn point
        playerPosX = FIX32(8 + (mapwidth / 2) * 16);  // Center X (in pixels)
        playerPosY = FIX32(8 + (mapheight / 2) * 16); // Center Y (in pixels)
        
        VDP_clearPlane(BG_A, TRUE);
        VDP_clearPlane(BG_B, TRUE);
        
        // Display the cave
        displayRoom();
        waitMs(100);
        SYS_doVBlankProcess();
        
        // Clear screen warble effect if it was active
        // VDP_setHorizontalScrollLine(BG_A, 0, NULL, 224, DMA);
        // VDP_setHorizontalScrollLine(BG_B, 0, NULL, 224, DMA);
    }
}
// Exit the current cave back to the world
void exitCave() {
    if (inCave) {
        // Restore the world state
        restoreWorldState();
        
        // Reset cave state
        inCave = FALSE;
        VDP_clearPlane(BG_A, TRUE);
        VDP_clearPlane(BG_B, TRUE);
        playerPosX = FIX32((caveEntranceCol - 2) * 16);
        playerPosY = FIX32((caveEntranceRow -1) * 16);
        
        // Update sprite position
        SPR_setPosition(player, fix32ToInt(playerPosX), fix32ToInt(playerPosY));
        //set player animation to idle down
        SPR_setAnim(player, ANIM_IDLE_DOWN);
        //set player facing dir to down
        playerDir = 0;
        SPR_update();
        
        // Display the restored world
        displayRoom();
        waitMs(1000);
    }
}

// Check if player is on a cave entrance tile
bool isOnCaveEntrance() {
    // Convert player position to tile coordinates
    u16 playerTileX = (fix32ToInt(playerPosX) + 8) >> 4;
    u16 playerTileY = (fix32ToInt(playerPosY) + 8) >> 4;    
    // Check for cave entrance tile (15)
    for (s16 dy = -1; dy <= 1; dy++) {
        for (s16 dx = -1; dx <= 1; dx++) {
            u16 checkY = playerTileY + dy;
            u16 checkX = playerTileX + dx;
            
            if (checkY < 14 && checkX < 16 && LEVEL_TILES[checkY][checkX] == 15) {
                return TRUE;
            }
        }
    }
    
    return FALSE;
}

// Check if player is on a cave exit tile
bool isOnCaveExit() {
    // Convert player position to tile coordinates
    u16 playerTileX = (fix32ToInt(playerPosX) + 8) >> 4;
    u16 playerTileY = (fix32ToInt(playerPosY) + 8) >> 4;  
    
    
    //for (s16 dy = -1; dy <= 1; dy++) {
      //  for (s16 dx = -1; dx <= 1; dx++) {
            u16 checkY = playerTileY;// + dy;
            u16 checkX = playerTileX;// + dx;
            
            if (checkY < 14 && checkX < 16 && LEVEL_TILES[checkY][checkX] == 16) {
                return TRUE;
            }
        //}
    //}
    
    return FALSE;
}

// Update cave system (check for cave entrance/exit)
void updateCaves() {
    if (!inCave && isOnCaveEntrance()) {
        // Player is standing on a cave entrance
        //if (JOY_readJoypad(JOY_1) ) {
            // Enter a randomly generated cave
            enterCave(caveLevel);   
        //}
    }
    else if (inCave && isOnCaveExit()) {
        // Player is standing on a cave exit
        //if (JOY_readJoypad(JOY_1) & BUTTON_C) {
            // Exit the cave
            //caveLevel++;
            exitCave();
        //}
    }
}

// Return whether player is in a cave
bool isInCave() {
    return inCave;
}

// Ensure there's a valid path from player to exit
void ensurePathToExit(u16 mapheight, u16 mapwidth) {
    // Find player position (starting point)
    u16 playerY = 0, playerX = 0;
    // Find exit position (destination)
    u16 exitY = 0, exitX = 0;
    
    // Locate player and exit positions in the map
    for (u16 y = 0; y < mapheight; y++) {
        for (u16 x = 0; x < mapwidth; x++) {
            if (LEVEL_TILES[y][x] == 10) { // Player tile
                playerY = y;
                playerX = x;
            } else if (LEVEL_TILES[y][x] == 16) { // Exit tile
                exitY = y;
                exitX = x;
            }
        }
    }
    
    // Simple path creation using a direct approach
    u16 currentY = playerY;
    u16 currentX = playerX;
    
    // Create a direct path in X direction first
    while (currentX != exitX) {
        if (currentX < exitX) {
            currentX++;
        } else {
            currentX--;
        }
        // Clear any walls in the path
        if (LEVEL_TILES[currentY][currentX] == 1) {
            LEVEL_TILES[currentY][currentX] = 0;
        }
    }
    
    // Then create a direct path in Y direction
    while (currentY != exitY) {
        if (currentY < exitY) {
            currentY++;
        } else {
            currentY--;
        }
        // Clear any walls in the path
        if (LEVEL_TILES[currentY][currentX] == 1) {
            LEVEL_TILES[currentY][currentX] = 0;
        }
    }
    
    // Ensure the path is at least 2 tiles wide for better navigation
    for (u16 y = 0; y < mapheight; y++) {
        for (u16 x = 0; x < mapwidth; x++) {
            if (LEVEL_TILES[y][x] == 0) { // If this is a path tile
                // Check adjacent tiles and clear walls if necessary
                for (s16 dy = -1; dy <= 1; dy++) {
                    for (s16 dx = -1; dx <= 1; dx++) {
                        // Only check orthogonal adjacency (not diagonals)
                        if ((dy == 0 || dx == 0) && !(dy == 0 && dx == 0)) {
                            if (y+dy >= 0 && y+dy < mapheight && x+dx >= 0 && x+dx < mapwidth) {
                                // Randomly clear some adjacent walls for a more natural path
                                if (LEVEL_TILES[y+dy][x+dx] == 1 && random() % 100 < 40) {
                                    LEVEL_TILES[y+dy][x+dx] = 0;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void ruleTile(u16 mapheight, u16 mapwidth, u8 type){
    for( u16 y = 0; y < mapheight; y++) {
        for( u16 x = 0; x < mapwidth; x++) {
            if(LEVEL_TILES[y][x] == type){
                if(LEVEL_TILES[y-1][x] == type){
                    LEVEL_TILES[y][x] = 17;
                }

            }
        }
    }



}
