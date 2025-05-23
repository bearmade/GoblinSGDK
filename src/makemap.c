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

u8 LEVEL_TILES[14][16] = {{0}};
u8 WORLD_TILES[9][9][14][16] = {{{{0}}}};

u16 caveEntranceRow = 0;
u16 caveEntranceCol = 0;

//u8 WORLD_TILES2[8][8][14][16] = {{{{0}}}};
u8 WORLD_LAYOUT[9][9] = {{0}};
u8 WORLD_LAYOUT_CA[112][128] = {{0}};
u16 roomY = 0;
u16 roomX = 0;
u16 currentWorldX = 0;
u16 currentWorldY = 0;
u16 mw = 32;
u16 mh = 28;
u16 mapwidth = 16;
u16 mapheight = 14;
u16 mapIndex;
u16 result;
u16 xx, yy;
u16 buildingPosX;
u16 buildingPosY;
bool enemyDisplayed = FALSE;
bool merchantChance = 0;
fix32 merchantPosX = FIX32(100);
fix32 merchantPosY = FIX32(100);
u16 merchWorldY = 0;
u16 merchWorldX = 0;


u16 countNeighbors(u16 type, u16 y, u16 x){
    u16 neighbors = 0;

    if(LEVEL_TILES[y + 1][x] == type) neighbors++;
    if(LEVEL_TILES[y - 1][x] == type) neighbors++;
    if(LEVEL_TILES[y][x + 1] == type) neighbors++;
    if(LEVEL_TILES[y][x - 1] == type) neighbors++;
    if(LEVEL_TILES[y + 1][x + 1] == type) neighbors++;
    if(LEVEL_TILES[y - 1][x - 1] == type) neighbors++;
    if(LEVEL_TILES[y + 1][x - 1] == type) neighbors++;
    if(LEVEL_TILES[y - 1][x + 1] == type) neighbors++;

    return neighbors;

}
u16 countNeighborsEX(u16 type, u16 y, u16 x){
    u16 neighbors = 0;

    if(LEVEL_TILES[y + 1][x] == type) neighbors++;
    if(LEVEL_TILES[y - 1][x] == type) neighbors++;
    if(LEVEL_TILES[y][x + 1] == type) neighbors++;
    if(LEVEL_TILES[y][x - 1] == type) neighbors++;
    if(LEVEL_TILES[y + 1][x + 1] == type) neighbors++;
    if(LEVEL_TILES[y - 1][x - 1] == type) neighbors++;
    if(LEVEL_TILES[y + 1][x - 1] == type) neighbors++;
    if(LEVEL_TILES[y - 1][x + 1] == type) neighbors++;

    return neighbors;

}
u16 countNeighborsBigMap(u16 type, u16 y, u16 x){
    u16 neighbors = 0;

    if(WORLD_LAYOUT_CA[y + 1][x] == type) neighbors++;
    if(WORLD_LAYOUT_CA[y - 1][x] == type) neighbors++;
    if(WORLD_LAYOUT_CA[y][x + 1] == type) neighbors++;
    if(WORLD_LAYOUT_CA[y][x - 1] == type) neighbors++;
    if(WORLD_LAYOUT_CA[y + 1][x + 1] == type) neighbors++;
    if(WORLD_LAYOUT_CA[y - 1][x - 1] == type) neighbors++;
    if(WORLD_LAYOUT_CA[y + 1][x - 1] == type) neighbors++;
    if(WORLD_LAYOUT_CA[y - 1][x + 1] == type) neighbors++;

    return neighbors;

}
u16 randSet(u16 a, u16 b, u16 c, u16 d){
	u16 result;
	result = (random() % 4);
				if (result == 0)
					result = a;
				else if (result == 1)
					result = b;
				else if (result == 2)
					result = c;
				else // result == 3
					result = d;

	return result;
}
void initMap(u16 yy, u16 xx, u16 mapheight, u16 mapwidth){
    for (yy = 0; yy < mapheight; yy++) { 
        for (xx = 0; xx < mapwidth; xx++) { 
            LEVEL_TILES[yy][xx] = 0; 
        }
    }

}
void makeRocks(u16 yy, u16 xx, u16 mapheight, u16 mapwidth, u16 type){

    for (yy = 3; yy < mapheight - 1; yy++) { 
        for (xx = 3; xx < mapwidth - 1; xx++) { 
			if((random() % 100)>=95){
				LEVEL_TILES[yy][xx] = type;
				if((random() % 100)>=55){
				LEVEL_TILES[yy+1][xx] = type;
				}
				if((random() % 100)>=55){
				LEVEL_TILES[yy+1][xx+1] = type;
				}
				if((random() % 100)>=55){
				LEVEL_TILES[yy][xx+1] = type;
				}
				if((random() % 100)>=55){
				LEVEL_TILES[yy-1][xx] = type;
				}
				if((random() % 100)>=55){
				LEVEL_TILES[yy-1][xx-1] = type;
				}
				if((random() % 100)>=55){
				LEVEL_TILES[yy][xx-1] = type;
				}
				if((random() % 100)>=55){
				LEVEL_TILES[yy+1][xx-1] = type;
				}
				if((random() % 100)>=55){
				LEVEL_TILES[yy-1][xx+1] = type;
				}
			}
		}}
}
void makeWall(u16 yy, u16 xx, u16 mapheight, u16 mapwidth, u16 side, u8 type){
    for (yy = 0; yy < mapheight; yy++) { 
        for (xx = 0; xx < mapwidth; xx++) { 
            if(side == 0){
                if (xx == 0) {
                    LEVEL_TILES[yy][xx] = type;
                }
                if ((xx == 1)) {
                    if((random() % 100)>=65){
                        LEVEL_TILES[yy][xx] = type;
                    }
                }   
            }
            else if(side == 1){
                if (yy == 0) {
                    LEVEL_TILES[yy][xx] = type;
                }
                if ((yy == 1)) {
                    if((random() % 100)>=65){
                        LEVEL_TILES[yy][xx] = type;
                    }
                }
            }
            else if(side == 2){
                if (xx == mapwidth-1) {
                    LEVEL_TILES[yy][xx] = type;
                }
                if ((xx == mapwidth-2)) {
                    if((random() % 100)>=65){
                        LEVEL_TILES[yy][xx] = type;
                    }
                }
            }
            else if(side == 3){
                if (yy == mapheight-1) {
                    LEVEL_TILES[yy][xx] = type;
                }
                if ((yy == mapheight-2)) {
                    if((random() % 100)>=65){
                        LEVEL_TILES[yy][xx] = type;
                    }
                }
            }
        }
    }
}

void makeGrass(u16 yy, u16 xx, u16 mapheight, u16 mapwidth){
    for (yy = 0; yy < mapheight; yy++) { 
        for (xx = 0; xx < mapwidth; xx++) { 
            if (!(xx == 0) && !(yy == 0) && !(xx == mapwidth-1) && !(yy == mapheight-1)) {
                if((random() % 100)>=75){
				LEVEL_TILES[yy][xx] = 2;
			}
            } 
        }
    }    
}
void randWalk(u16 yy, u16 xx, u16 mapheight, u16 mapwidth, u16 steps) {
    
    u16 currentY = yy;
    u16 currentX = xx;   
    const int dx[] = { 0, 0, -1, 1 };
    const int dy[] = { -1, 1, 0, 0 };
    
    for (u16 step = 0; step < steps; step++) {
        
        if(randSet(0,0,1,1)==1){
            LEVEL_TILES[currentY][currentX] = 3;
        }
        else{
            LEVEL_TILES[currentY][currentX] = 8;
        }        
        u16 direction = random() % 4;        
        u16 nextY = currentY + dy[direction];
        u16 nextX = currentX + dx[direction];
        
        if (nextY > 0 && nextY < mapheight - 1 && nextX > 0 && nextX < mapwidth - 1) {
            
            currentY = nextY;
            currentX = nextX;
        }
        
    }
}
void randWorldWalk(u16 mapheight, u16 mapwidth, u16 steps) {
    
    u16 currentY = 3;
    u16 currentX = 3;   
    const int dx[] = { 0, 0, -1, 1 };
    const int dy[] = { -1, 1, 0, 0 };    
    for (u16 step = 0; step < steps; step++) {
        
        WORLD_LAYOUT[currentY][currentX] = 1;        
        u16 direction = random() % 4;        
        u16 nextY = currentY + dy[direction];
        u16 nextX = currentX + dx[direction];
        
        if (nextY > 0 && nextY < mapheight - 1 && nextX > 0 && nextX < mapwidth - 1) {
            
            currentY = nextY;
            currentX = nextX;
        }
        
    }
}
void makePlayerSpawn(u16 yy, u16 xx, u16 mapheight, u16 mapwidth){
    bool playerPlaced = FALSE;

    for (yy = 0; yy < mapheight; yy++) { 
        for (xx = 0; xx < mapwidth; xx++) { 
            if (!(xx == 0) && !(yy == 0) && !(xx == mapwidth-1) && !(yy == mapheight-1) && (playerPlaced == FALSE)) {
                if((random() % 100)>=95){
				LEVEL_TILES[yy][xx] = 10;
                playerPlaced = TRUE;
			}
            }       
        }
    }    
}
void makeExit(u16 yy, u16 xx, u16 mapheight, u16 mapwidth){
    bool exitPlaced = FALSE;

    for (yy = 2; yy < mapheight; yy++) { 
        for (xx = 0; xx < mapwidth; xx++) { 
            if (!(xx == 0) && !(yy == 0) && !(xx == mapwidth-1) && !(yy == mapheight-1) && (exitPlaced == FALSE)) {
                if((random() % 100)>=95){

				LEVEL_TILES[yy-2][xx-1] = 2;
                LEVEL_TILES[yy-2][xx] = 2;
                LEVEL_TILES[yy-2][xx+1] = 2;

                LEVEL_TILES[yy-1][xx-1] = 12;
                LEVEL_TILES[yy-1][xx] = 12;
                LEVEL_TILES[yy-1][xx+1] = 12;

                LEVEL_TILES[yy][xx - 1] = 12;
                LEVEL_TILES[yy][xx] = 9;
                LEVEL_TILES[yy][xx + 1] = 12;

                LEVEL_TILES[yy+1][xx-1] = 2;
                LEVEL_TILES[yy+1][xx] = 10;
                LEVEL_TILES[yy+1][xx+1] = 2;
                
                LEVEL_TILES[yy+2][xx-1] = 2;
                LEVEL_TILES[yy+2][xx] = 2;
                LEVEL_TILES[yy+2][xx+1] = 2;

                exitPlaced = TRUE;
			}
            } 
        }
    }    
}
void makeInnerWall(u16 yy, u16 xx, u16 mapheight, u16 mapwidth){

    for (yy = 0; yy < mapheight; yy++) { 
        for (xx = 0; xx < mapwidth; xx++) { 

            if(countNeighbors(1, yy, xx) >= 3 && !(LEVEL_TILES[yy][xx] == 1)){
                
                LEVEL_TILES[yy][xx] = 6;
            }
            
        }
    }
}
void makeTreeTops(u16 yy, u16 xx, u16 mapheight, u16 mapwidth){

    for (yy = 0; yy < mapheight; yy++) { 
        for (xx = 0; xx < mapwidth; xx++) { 

            if((LEVEL_TILES[yy][xx] == 8)){
                
                LEVEL_TILES[yy-1][xx] = 7;
            }   
        }
    }
}
void makeDoorways(){
    bool exitPlaced = FALSE;
    u16 result = 0;
 
    if ((exitPlaced == FALSE)) {
        result = randSet(1,2,3,4);
        if (result == 1){
            LEVEL_TILES[0][7] = 2;
            LEVEL_TILES[0][8] = 2;
            LEVEL_TILES[0][9] = 2;
            exitPlaced = TRUE;
        }
        if (result == 2){
            LEVEL_TILES[7][0] = 2;
            LEVEL_TILES[8][0] = 2;
            LEVEL_TILES[9][0] = 2;
            exitPlaced = TRUE;
        }
        if (result == 3){
            LEVEL_TILES[13][7] = 2;
            LEVEL_TILES[13][8] = 2;
            LEVEL_TILES[13][9] = 2;
            exitPlaced = TRUE;
        }
        if (result == 4){
            LEVEL_TILES[7][15] = 2;
            LEVEL_TILES[8][15] = 2;
            LEVEL_TILES[9][15] = 2;
            exitPlaced = TRUE;
        }    
    }       
}
// function to check for edge blocks
void makeRoom(u16 yy, u16 xx, u16 mapheight, u16 mapwidth, u16 type){

    initMap(yy, xx, mapheight, mapwidth);
    makeGrass(yy, xx, mapheight, mapwidth);    
    makeRocks(yy, xx, mapheight, mapwidth, 3);
    makeRocks(yy, xx, mapheight, mapwidth, 8); 
    makeGrass(yy, xx, mapheight, mapwidth);
    randWalk(7, 8, mapheight, mapwidth, 32);
     



    switch(type) {
        case 1:
            makeWall(yy, xx, mapheight, mapwidth, 0, 8);
            makeWall(yy, xx, mapheight, mapwidth, 1, 8);
            break;
        case 2:
            makeWall(yy, xx, mapheight, mapwidth, 1, 8);
            break;
        case 3:
            makeWall(yy, xx, mapheight, mapwidth, 1, 8);
            makeWall(yy, xx, mapheight, mapwidth, 2, 8);
            break;
        case 4:
            makeWall(yy, xx, mapheight, mapwidth, 0, 8);
            break;
        case 5:
            makeExit(yy, xx, mapheight, mapwidth);
            break;
        case 6:
            makeWall(yy, xx, mapheight, mapwidth, 2, 8);
            break;
        case 7:
            makeWall(yy, xx, mapheight, mapwidth, 0, 8);
            makeWall(yy, xx, mapheight, mapwidth, 3, 8);
            break;
        case 8:
            makeWall(yy, xx, mapheight, mapwidth, 3, 8);
            break;
        case 9:
            makeWall(yy, xx, mapheight, mapwidth, 3, 8);
            makeWall(yy, xx, mapheight, mapwidth, 2, 8);
            break;
        case 10:
            //merchantChance = 1; 
            makeGrass(yy, xx, mapheight, mapwidth);
           // findMerchantPosition();
                       
            break;
        case 11:
            break;
        default:
           
            break;
    }
    // Add doorways based on adjacent rooms
    makeDoorway(yy, xx, mapheight, mapwidth, 0); // Check left
    makeDoorway(yy, xx, mapheight, mapwidth, 1); // Check top
    makeDoorway(yy, xx, mapheight, mapwidth, 2); // Check right
    makeDoorway(yy, xx, mapheight, mapwidth, 3); // Check bottom



}
void findMerchantPosition() {
    bool validPosition = FALSE;
    u16 attempts = 0;
    const u16 MAX_ATTEMPTS = 100;
    
    while (!validPosition && attempts < MAX_ATTEMPTS) {
        // Get random positions but keep merchant away from edges
        u16 testY = 2 + (random() % (mapheight-4));
        u16 testX = 2 + (random() % (mapwidth-4));
        
        // Check for valid floor tiles (0=floor, 2=grass)
        // Also check surrounding tiles to ensure merchant isn't trapped
        if ((LEVEL_TILES[testY][testX] == 0 || LEVEL_TILES[testY][testX] == 2) &&
            (LEVEL_TILES[testY-1][testX] == 0 || LEVEL_TILES[testY-1][testX] == 2) &&
            (LEVEL_TILES[testY+1][testX] == 0 || LEVEL_TILES[testY+1][testX] == 2) &&
            (LEVEL_TILES[testY][testX-1] == 0 || LEVEL_TILES[testY][testX-1] == 2) &&
            (LEVEL_TILES[testY][testX+1] == 0 || LEVEL_TILES[testY][testX+1] == 2)) {
            
            // Convert to pixel coordinates (multiply by 16 for tile size)
            merchantPosX = FIX32(testX << 4);
            merchantPosY = FIX32(testY << 4);
            validPosition = TRUE;
        }
        attempts++;
    }
    
    // Fallback position if no valid spot found
    if (!validPosition) {
        merchantPosX = FIX32(128);
        merchantPosY = FIX32(128);
    }
}

void makeDoorway(u16 yy, u16 xx, u16 mapheight, u16 mapwidth, u16 side) {
    // Check if there's a room in the direction we're making the door
    bool hasAdjacentRoom = FALSE;
    
    switch(side) {
        case 0: // Left
            if(currentWorldX > 0 && WORLD_LAYOUT[currentWorldY][currentWorldX-1] == 1) {
                hasAdjacentRoom = TRUE;
                // Create left doorway
                LEVEL_TILES[mapheight/2-1][0] = 2;
                LEVEL_TILES[mapheight/2][0] = 2;
                LEVEL_TILES[mapheight/2+1][0] = 2;
            }
            break;
        case 1: // Top
            if(currentWorldY > 0 && WORLD_LAYOUT[currentWorldY-1][currentWorldX] == 1) {
                hasAdjacentRoom = TRUE;
                // Create top doorway
                LEVEL_TILES[0][mapwidth/2-1] = 2;
                LEVEL_TILES[0][mapwidth/2] = 2;
                LEVEL_TILES[0][mapwidth/2+1] = 2;
            }
            break;
        case 2: // Right 
            if(currentWorldX < 7 && WORLD_LAYOUT[currentWorldY][currentWorldX+1] == 1) {
                hasAdjacentRoom = TRUE;
                // Create right doorway
                LEVEL_TILES[mapheight/2-1][mapwidth-1] = 2;
                LEVEL_TILES[mapheight/2][mapwidth-1] = 2;
                LEVEL_TILES[mapheight/2+1][mapwidth-1] = 2;
            }
            break;
        case 3: // Bottom
            if(currentWorldY < 7 && WORLD_LAYOUT[currentWorldY+1][currentWorldX] == 1) {
                hasAdjacentRoom = TRUE;
                // Create bottom doorway
                LEVEL_TILES[mapheight-1][mapwidth/2-1] = 2;
                LEVEL_TILES[mapheight-1][mapwidth/2] = 2;
                LEVEL_TILES[mapheight-1][mapwidth/2+1] = 2;
            }
            break;
    }
}
void ruleTileWORLD(){
    for(u8 y = 0; y < 8; y++){
        for(u8 x = 0; x < 8; x++){
    }
    }
}
void makeWorldMap(){
    
    randWorldWalk(8,8,10);
    u16 randRoomY = (random()%8)+1;
    u16 randRoomX = (random()%8)+1;
                    merchWorldY = randRoomY -1;
                merchWorldX = randRoomX;
    u16 index = 1;
    for(roomY = 0; roomY < 9; roomY++){
        for(roomX = 0; roomX < 9; roomX++){
            ///
            if(roomY == 0 && roomX == 0){
                makeRoom(0, 0, 14, 16, 1);
            }            
            else if(roomY == 0 && roomX == 8){
                makeRoom(0, 0, 14, 16, 3);
            }
            else if(roomY == 0){
                makeRoom(0, 0, 14, 16, 2);
            }
            ///
            else if(roomY == 8 && roomX == 0){
                makeRoom(0, 0, 14, 16, 7);
            }
            else if(roomY == 8 && roomX == 8){
                makeRoom(0, 0, 14, 16, 9);
            }
            else if(roomY == 8){
                makeRoom(0, 0, 14, 16, 8);
            }
            else if(roomX == 8){
                makeRoom(0, 0, 14, 16, 6);
            }

            else if((roomX == 0)&&!(roomY == 8)){
                makeRoom(0, 0, 14, 16, 4);
            }
            else if(roomY == 3 && roomX == 3){
                makeRoom(0, 0, 14, 16, 5);
            }
            else if (roomY == randRoomY && roomX == randRoomX){
                makeRoom(0, 0, 14, 16, 10);
                merchWorldY = randRoomY - 1; 
                merchWorldX = randRoomX;
                //findMerchantPosition();

            }
            else{
                makeRoom(0, 0, 14, 16, 11);
            }

            for (int j = 0; j < 14; j++) { 
                for (int i = 0; i < 16; i++) { 
                    WORLD_TILES[roomY][roomX][j][i] = LEVEL_TILES[j][i]; 
                
            }
        }
        
    }

    }
}
void updateCurrentRoom(){
    for (int j = 0; j < 14; j++) { 
            for (int i = 0; i < 16; i++) { 
                LEVEL_TILES[j][i] = WORLD_TILES[currentWorldY][currentWorldX][j][i];
        }
    }


}
void convertMapArrays(){
    int index = 0;
    for (int j = 0; j < mapheight; j++) { 
        for (int i = 0; i < mapwidth; i++) { 
            LEVEL_COL[index] = LEVEL_TILES[j][i]; 
			LEVEL_COL2[index] = LEVEL_TILES[j][i];
             

            index++;
        }
    }
	index = 0;
    for (int j = 0; j < mapheight; j++) { 
        for (int i = 0; i < mapwidth; i++) { 
			if((LEVEL_COL2[index] == 3)||(LEVEL_COL2[index] == 8)){
					LEVEL_COL[index] = 1;				
			}
            index++;
        }
    }

}
void create16by16TileRand(bool vertFlip, u16 a, u16 b, u16 c, u16 d){
    
    if(vertFlip == TRUE){
        result = randSet(a,b,c,d);
        VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 0, randSet(0,0,1,1), randSet(0,0,1,1), result), xx, yy);
        result = randSet(a,b,c,d);
        VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 0, randSet(0,0,1,1), randSet(0,0,1,1), result), xx + 1, yy);
        result = randSet(a,b,c,d);
        VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 0, randSet(0,0,1,1), randSet(0,0,1,1), result), xx + 1, yy + 1);
        result = randSet(a,b,c,d);
        VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 0, randSet(0,0,1,1), randSet(0,0,1,1), result), xx, yy + 1);

    }
    else{
        result = randSet(a,b,c,d);
        VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 0, FALSE, randSet(0,0,1,1), result), xx, yy);
        result = randSet(a,b,c,d);
        VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 0, FALSE, randSet(0,0,1,1), result), xx + 1, yy);
        result = randSet(a,b,c,d);
        VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 0, FALSE, randSet(0,0,1,1), result), xx + 1, yy + 1);
        result = randSet(a,b,c,d);
        VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 0, FALSE, randSet(0,0,1,1), result), xx, yy + 1);

    }
    
  

}
void create16by16Tile(bool horFlip, bool topPrio, bool bottomPrio, bool topBG, bool bottomBG, u16 a, u16 b, u16 c, u16 d){
    
    if(horFlip==TRUE){
        VDP_setTileMapXY(topBG, TILE_ATTR_FULL(PAL1, topPrio, FALSE, horFlip, a), xx, yy);    
        VDP_setTileMapXY(topBG, TILE_ATTR_FULL(PAL1, topPrio, FALSE, horFlip, b), xx + 1, yy);
        VDP_setTileMapXY(bottomBG, TILE_ATTR_FULL(PAL1, bottomPrio, FALSE, horFlip, c), xx, yy + 1);    
        VDP_setTileMapXY(bottomBG, TILE_ATTR_FULL(PAL1, bottomPrio, FALSE, horFlip, d), xx + 1, yy + 1);

    }
    else{
        VDP_setTileMapXY(topBG, TILE_ATTR_FULL(PAL1, topPrio, FALSE, FALSE, a), xx, yy);    
        VDP_setTileMapXY(topBG, TILE_ATTR_FULL(PAL1, topPrio, FALSE, FALSE, b), xx + 1, yy);
        VDP_setTileMapXY(bottomBG, TILE_ATTR_FULL(PAL1, bottomPrio, FALSE, FALSE, c), xx, yy + 1);    
        VDP_setTileMapXY(bottomBG, TILE_ATTR_FULL(PAL1, bottomPrio, FALSE, FALSE, d), xx + 1, yy + 1);

    }
    
    
    
}
void create16by16TileIndexed(bool horFlip, bool topPrio, bool bottomPrio, bool topBG, bool bottomBG, u16 ind){
    
    if(horFlip==TRUE){
        VDP_setTileMapXY(topBG, TILE_ATTR_FULL(PAL1, topPrio, FALSE, horFlip, ind + 1), xx, yy);    
        VDP_setTileMapXY(topBG, TILE_ATTR_FULL(PAL1, topPrio, FALSE, horFlip, ind + 0), xx + 1, yy);
        VDP_setTileMapXY(bottomBG, TILE_ATTR_FULL(PAL1, bottomPrio, FALSE, horFlip, ind + 3), xx, yy + 1);    
        VDP_setTileMapXY(bottomBG, TILE_ATTR_FULL(PAL1, bottomPrio, FALSE, horFlip, ind + 2), xx + 1, yy + 1);

    }
    else{
        VDP_setTileMapXY(topBG, TILE_ATTR_FULL(PAL1, topPrio, FALSE, FALSE, ind), xx, yy);    
        VDP_setTileMapXY(topBG, TILE_ATTR_FULL(PAL1, topPrio, FALSE, FALSE, ind + 1), xx + 1, yy);
        VDP_setTileMapXY(bottomBG, TILE_ATTR_FULL(PAL1, bottomPrio, FALSE, FALSE, ind + 2), xx, yy + 1);    
        VDP_setTileMapXY(bottomBG, TILE_ATTR_FULL(PAL1, bottomPrio, FALSE, FALSE, ind + 3), xx + 1, yy + 1);

    }
    
    
    
}
void create32by16TileIndexed(bool horFlip, bool topPrio, bool bottomPrio, bool topBG, bool bottomBG, u16 ind){
    
    if(horFlip==TRUE){
        VDP_setTileMapXY(topBG, TILE_ATTR_FULL(PAL1, topPrio, FALSE, horFlip, ind+1), xx, yy-2);
        VDP_setTileMapXY(topBG, TILE_ATTR_FULL(PAL1, topPrio, FALSE, horFlip, ind), xx+1, yy-2);
        VDP_setTileMapXY(topBG, TILE_ATTR_FULL(PAL1, topPrio, FALSE, horFlip, ind+3), xx, yy -1);
        VDP_setTileMapXY(topBG, TILE_ATTR_FULL(PAL1, topPrio, FALSE, horFlip, ind+2), xx+1, yy -1);
        

        VDP_setTileMapXY(bottomBG, TILE_ATTR_FULL(PAL1, topPrio, FALSE, horFlip, ind+5), xx, yy);    
        VDP_setTileMapXY(bottomBG, TILE_ATTR_FULL(PAL1, topPrio, FALSE, horFlip, ind + 4), xx + 1, yy);
        VDP_setTileMapXY(bottomBG, TILE_ATTR_FULL(PAL1, bottomPrio, FALSE, horFlip, ind + 7), xx, yy + 1);    
        VDP_setTileMapXY(bottomBG, TILE_ATTR_FULL(PAL1, bottomPrio, FALSE, horFlip, ind + 6), xx + 1, yy + 1);        

    }
    else{
        VDP_setTileMapXY(topBG, TILE_ATTR_FULL(PAL1, topPrio, FALSE, FALSE, ind), xx, yy-2);
        VDP_setTileMapXY(topBG, TILE_ATTR_FULL(PAL1, topPrio, FALSE, FALSE, ind+1), xx+1, yy-2);
        VDP_setTileMapXY(topBG, TILE_ATTR_FULL(PAL1, topPrio, FALSE, FALSE, ind+2), xx, yy -1);
        VDP_setTileMapXY(topBG, TILE_ATTR_FULL(PAL1, topPrio, FALSE, FALSE, ind+3), xx+1, yy -1);
        

        VDP_setTileMapXY(bottomBG, TILE_ATTR_FULL(PAL1, topPrio, FALSE, FALSE, ind+4), xx, yy);    
        VDP_setTileMapXY(bottomBG, TILE_ATTR_FULL(PAL1, topPrio, FALSE, FALSE, ind + 5), xx + 1, yy);
        VDP_setTileMapXY(bottomBG, TILE_ATTR_FULL(PAL1, bottomPrio, FALSE, FALSE, ind + 6), xx, yy + 1);    
        VDP_setTileMapXY(bottomBG, TILE_ATTR_FULL(PAL1, bottomPrio, FALSE, FALSE, ind + 7), xx + 1, yy + 1);

    }
    
    
    
}
void displayRoom(){
   

    for (xx = 0; xx < mw; xx += 2) {
        for (yy = 0; yy < mh; yy += 2) {

            mapIndex = ((yy >> 1) * (mw >> 1)) + (xx >> 1);

            if (LEVEL_COL2[mapIndex] == 10) {
                create16by16TileRand(FALSE, 11,9,12,56);
                if(MapHasBeenMade == FALSE){
                    playerPosX = intToFix32(xx) << 3;
                    playerPosY = intToFix32(yy) << 3;

                }

            }

            if (LEVEL_COL2[mapIndex] == 0) {
				create16by16TileRand(FALSE, 10,10,12,12);
            }
            //lone stone
            if (LEVEL_COL2[mapIndex] == 1) {
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 1), xx, yy);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, TRUE, 1), xx + 1, yy);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 0, TRUE, TRUE, 1), xx + 1, yy + 1);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 0, TRUE, FALSE, 1), xx, yy + 1);
            }
            //cross stone
            if(LEVEL_COL2[mapIndex] == 17){
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 3), xx, yy);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, TRUE, 3), xx + 1, yy);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 0, TRUE, TRUE, 3), xx + 1, yy + 1);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 0, TRUE, FALSE, 3), xx, yy + 1);
            }
           
            if (LEVEL_COL2[mapIndex] == 5) {
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, randSet(0,0,1,1), 2), xx, yy);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, randSet(0,0,1,1), 2), xx + 1, yy);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 0, TRUE, randSet(0,0,1,1), 2), xx + 1, yy + 1);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 0, TRUE, randSet(0,0,1,1), 2), xx, yy + 1);
            }
            if (LEVEL_COL2[mapIndex] == 6) {
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, randSet(0,0,1,1), TRUE, 21), xx, yy);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, randSet(0,0,1,1), FALSE, 21), xx + 1, yy);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 0, randSet(0,0,1,1), FALSE, 21), xx + 1, yy + 1);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 0, randSet(0,0,1,1), TRUE, 21), xx, yy + 1);
            }
           
            if (LEVEL_COL2[mapIndex] == 8) {
                result = randSet(0,0,1,2);
                if (result == 1){
                    if (randSet(1,1,0,0) == 1){
                        create32by16TileIndexed(randSet(0,0,1,1), 1,0,0,1,44);

                }
                else {
                    create32by16TileIndexed(randSet(0,0,1,1), 1,0,0,1,98);


                }
                }
                else if (result == 0){
                    create32by16TileIndexed(randSet(0,0,1,1), 1,0,0,1,34);

                }
                else if (result == 2){

                    VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 34), xx, yy-4);
                    VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 35), xx+1, yy-4);
                    VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 37), xx+1, yy -3);
                    VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 36), xx, yy -3);

                    VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 57), xx, yy-2);
                    VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 58), xx+1, yy-2);
                    VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 60), xx+1, yy -1);
                    VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 59), xx, yy -1);

                    

                    VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 38), xx, yy);
                    VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 39), xx + 1, yy);
                    VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 41), xx + 1, yy + 1);
                    VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 40), xx, yy + 1);
                    
                }
                else if ( result == 3){
                    VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 106), xx, yy-2);
                    VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 107), xx+1, yy-2);
                    VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 109), xx+1, yy -1);
                    VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 108), xx, yy -1);

                    

                    VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 110), xx, yy);
                    VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 111), xx + 1, yy);
                    VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 114), xx + 1, yy + 1);
                    VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 113), xx, yy + 1);

                }
              
            }

            if (LEVEL_COL2[mapIndex] == 9) {
                
                
                
               //left
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 12), xx - 2, yy - 4);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 62), xx - 1, yy - 4);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 67), xx - 1, yy - 3);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 66), xx - 2, yy - 3);

                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 72), xx - 2, yy - 2);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 73), xx - 1, yy - 2);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 79), xx - 1, yy - 1);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 78), xx - 2, yy - 1);

                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 112), xx - 2, yy);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 84), xx - 1, yy);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 89), xx - 1, yy + 1);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 115), xx - 2, yy + 1);
                
                
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 120), xx - 2, yy + 2);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 121), xx - 1, yy + 2);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 123), xx - 1, yy + 3);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 122), xx - 2, yy + 3);
      


                //middle
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 63), xx, yy - 4);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 64), xx + 1, yy - 4);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 69), xx + 1, yy - 3);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 68), xx, yy - 3);

                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 74), xx, yy - 2);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 75), xx + 1, yy - 2);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 81), xx + 1, yy - 1);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 80), xx, yy - 1);

                VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 120), xx, yy);
                VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 121), xx + 1, yy);
                VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 123), xx + 1, yy + 1);
                VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 122), xx, yy + 1);
                
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 85), xx , yy + 2);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 86), xx + 1, yy + 2);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 91), xx + 1, yy + 3);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 90), xx , yy + 3);



                //right
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 65), xx + 2, yy - 4);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 12), xx + 3, yy - 4);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 71), xx + 3, yy - 3);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 70), xx + 2, yy - 3);

                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 76), xx + 2, yy - 2);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 77), xx + 3, yy - 2);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 83), xx + 3, yy - 1);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 82), xx + 2, yy - 1);

                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 87), xx + 2, yy);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 88), xx + 3, yy);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 93), xx + 3, yy + 1);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 92), xx + 2, yy + 1);
          
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 120), xx + 2, yy + 2);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 121), xx + 3, yy + 2);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 123), xx + 3, yy + 3);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 122), xx + 2, yy + 3);



            }
           


            if (LEVEL_COL2[mapIndex] == 12) {
				result = randSet(11,9,12,56);
                VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 0, FALSE, randSet(0,0,1,1), result), xx, yy);
				result = randSet(43,9,33,12);
                VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 0, FALSE, randSet(0,0,1,1), result), xx + 1, yy);
				result = randSet(11,9,31,12);
                VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 0, FALSE, randSet(0,0,1,1), result), xx + 1, yy + 1);
				result = randSet(12,32,11,9);
                VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 0, FALSE, randSet(0,0,1,1), result), xx, yy + 1);
            }



			if (LEVEL_COL2[mapIndex] == 2) {
				result = randSet(11,9,12,56);
                VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 0, FALSE, randSet(0,0,1,1), result), xx, yy);
				result = randSet(43,9,33,12);
                VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 0, FALSE, randSet(0,0,1,1), result), xx + 1, yy);
				result = randSet(11,9,31,12);
                VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 0, FALSE, randSet(0,0,1,1), result), xx + 1, yy + 1);
				result = randSet(12,32,11,9);
                VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 0, FALSE, randSet(0,0,1,1), result), xx, yy + 1);
            }
			if (LEVEL_COL2[mapIndex] == 3) {
				result = randSet(1,2,3,4);
				    switch (result) {
                        case 1:
                        
                            create16by16TileIndexed(randSet(0,0,1,1),1,0,1,1,27);                            
                            break;
                        case 2:
                            create16by16TileIndexed(randSet(0,0,1,1),1,0,1,1,17);    
                            break;
                        case 3:
                            create16by16TileIndexed(randSet(0,0,1,1),1,0,1,1,13); 
                            break;
                        case 4:
                            create16by16TileIndexed(randSet(0,0,1,1),1,0,1,1,13);                          
                            break;
                    }                     
            } 

                               //left
            // Example cave drawing: if a cave marker is detected, draw an 8x7 cave using tiles starting from index 124.
            // (You might choose a different condition; here we check for a special cave tile value, e.g. 15.)
            if (LEVEL_COL2[mapIndex] == 15) {
                // Choose an offset inside the room where the cave should appear.
                // (For example, cave tiles will be drawn starting at bg tile coordinate (4,4) in the room.)
                u16 caveOffsetX = caveEntranceCol + 1;
                u16 caveOffsetY = caveEntranceRow - 2;
                u16 tileIndexStart = 124;
                for (u16 cy = 0; cy < 7; cy++) {
                    for (u16 cx = 0; cx < 8; cx++) {
                        // Calculate the tile to draw from the cave tileset.
                        u16 caveTile = tileIndexStart + cx + (cy * 8);
                        // Draw the cave tile at the proper BG tile coordinate.
                        // (xx and yy here are the current drawing offsets for this room)
                        VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, caveTile),
                                         caveOffsetX + cx, caveOffsetY + cy);
                    
                }
            }




            }    
            if (LEVEL_COL2[mapIndex] == 16) {
            
             VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 180), xx, yy);
            VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 181), xx + 1, yy);
            VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 183), xx + 1, yy + 1);
            VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 182), xx, yy + 1);

    }
    

}
    }}
void makeMap(){

    
    VDP_loadTileSet(tileset1.tileset, 1, DMA);
    //PAL_fadeIn(16, 32, tileset1.palette->data, 600, 0);
    //PAL_setPalette(PAL1, tileset1.palette->data, DMA);
    
    makeWorldMap();
    

    currentWorldX = 3;
    currentWorldY = 3;

    updateCurrentRoom();

    convertMapArrays();

    displayRoom();
    MapHasBeenMade = TRUE;

    

    
}
void bigMapCA(){

    for(yy = 0; yy < 112; yy++){
        for(xx = 0; xx < 128; xx++){
            if(randSet(1,1,0,0)==1){
                WORLD_LAYOUT_CA[yy][xx] = 8;
            }        
    }
    }
    u16 neighbors = 0;
    for(yy = 0; yy < 112; yy++){
        for(xx = 0; xx < 128; xx++){
            neighbors = countNeighborsBigMap(8,yy,xx);
            if((neighbors > 3) || (neighbors < 3)){
                WORLD_LAYOUT_CA[yy][xx] = 0;
            }        
    }
    }
    makeRocksBigMap(yy,xx,112,128,3);


}
void makeRocksBigMap(u16 yy, u16 xx, u16 mapheight, u16 mapwidth, u16 type){

    for (yy = 3; yy < mapheight - 1; yy++) { 
        for (xx = 3; xx < mapwidth - 1; xx++) { 
			if((random() % 100)>=95){
				WORLD_LAYOUT_CA[yy][xx] = type;
				if((random() % 100)>=55){
				WORLD_LAYOUT_CA[yy+1][xx] = type;
				}
				if((random() % 100)>=55){
				WORLD_LAYOUT_CA[yy+1][xx+1] = type;
				}
				if((random() % 100)>=55){
				WORLD_LAYOUT_CA[yy][xx+1] = type;
				}
				if((random() % 100)>=55){
				WORLD_LAYOUT_CA[yy-1][xx] = type;
				}
				if((random() % 100)>=55){
				WORLD_LAYOUT_CA[yy-1][xx-1] = type;
				}
				if((random() % 100)>=55){
				WORLD_LAYOUT_CA[yy][xx-1] = type;
				}
				if((random() % 100)>=55){
				WORLD_LAYOUT_CA[yy+1][xx-1] = type;
				}
				if((random() % 100)>=55){
				WORLD_LAYOUT_CA[yy-1][xx+1] = type;
				}
			}

		}}


}
void showMerchant(){
        findMerchantPosition();
        PAL_setPalette(PAL3, merchantSprite.palette->data, DMA);
        merchant = SPR_addSprite(&merchantSprite, fix32ToInt(merchantPosX), fix32ToInt(merchantPosY), TILE_ATTR(PAL3, FALSE, FALSE, FALSE));
        SPR_setPosition(merchant, fix32ToInt(merchantPosX), fix32ToInt(merchantPosY));
        //SPR_setVisibility(merchant, VISIBLE);
       
       if (currentWorldX == merchWorldX && currentWorldY == merchWorldY ) {


		SPR_setVisibility(merchant, VISIBLE);
        
	} else {

		SPR_setVisibility(merchant, HIDDEN);
	}
        
        //SPR_setVisibility(merchant, HIDDEN);

}

void matchRoomEdges(u16 roomY, u16 roomX) {
    // Match left edge with right edge of previous room
    if (roomX > 0) {
        for (u16 y = 0; y < 14; y++) {
            // Copy last column of previous room to first column of current room
            WORLD_TILES[roomY][roomX][y][0] = WORLD_TILES[roomY][roomX-1][y][15];
            // Ensure walkable path
            if (WORLD_TILES[roomY][roomX][y][0] == 1) { // If wall
                WORLD_TILES[roomY][roomX][y][0] = 0;    // Make walkable
                WORLD_TILES[roomY][roomX-1][y][15] = 0; // Make previous tile walkable
            }
        }
    }

    // Match top edge with bottom edge of room above
    if (roomY > 0) {
        for (u16 x = 0; x < 16; x++) {
            // Copy bottom row of room above to top row of current room
            WORLD_TILES[roomY][roomX][0][x] = WORLD_TILES[roomY-1][roomX][13][x];
            // Ensure walkable path
            if (WORLD_TILES[roomY][roomX][0][x] == 1) { // If wall
                WORLD_TILES[roomY][roomX][0][x] = 0;    // Make walkable
                WORLD_TILES[roomY-1][roomX][13][x] = 0; // Make previous tile walkable
            }
        }
    }
}

void spawnCaveEntrances() {
    u16 spawned = 0;
    // Loop until five cave entrances have been spawned.
    while (spawned < 5) {
        // Select a random room from the 9x9 world.
        u16 rRoomY = random() % 9;
        u16 rRoomX = random() % 9;
        
        // Skip room (3,3)
        if (rRoomY == 3 && rRoomX == 3) {
            continue;
        }
        // mark room as a cave room
        
        
        // Check if a cave entrance has already been placed in this room.
        // (Using the center of the room as a reference; the center tiles are at row 6-7 and col 7-8 for a 14x16 room.)
        if (WORLD_LAYOUT[rRoomY][rRoomX] == 20) {
            continue;
        }
        
        // Pick a random cave entrance within a safe zone (rows 3 to 9, cols 3 to 9)
        u16 caveRow = 3 + (random() % 7); // random value 3-9
        u16 caveCol = 3 + (random() % 7); // random value 3-9

        caveEntranceRow = caveRow;
        caveEntranceCol = caveCol;

        // Clear a 3x3 area around the chosen cave entrance
        for (u16 j = caveRow - 3; j <= caveRow + 3; j++) {
            for (u16 i = caveCol - 3; i <= caveCol + 3; i++) {
                // Make sure we don't exceed our safe zone boundaries
                //if (j >= 3 && j <= 9 && i >= 3 && i <= 9) {
                    WORLD_TILES[rRoomY][rRoomX][j][i] = 0;
                //}
            }
        }
        // Place the cave entrance tile at the chosen location
        WORLD_TILES[rRoomY][rRoomX][caveRow-3][caveCol] = 15;

        WORLD_LAYOUT[rRoomY][rRoomX] = 20;
        spawned++;

    }
}



void drawMap() {
    // Clear previous map tiles
    VDP_clearPlane(BG_A, TRUE);
    VDP_clearPlane(BG_B, TRUE);
    
    // Draw the map tiles
    for (u16 y = 0; y < mh; y++) {
        for (u16 x = 0; x < mw; x++) {
            u16 tileValue = LEVEL_TILES[y][x];
            
            VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL0, 0, 0, 0, tileValue), x, y);
        }
    }
    
    // Update the screen
    //SYS_doVBlankProcess();
}
