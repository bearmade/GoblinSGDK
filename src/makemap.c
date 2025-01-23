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
u8 WORLD_TILES[8][8][14][16] = {{{{0}}}};
u8 WORLD_LAYOUT[8][8] = {{0}};
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
void makeWall(u16 yy, u16 xx, u16 mapheight, u16 mapwidth, u16 side){
    for (yy = 0; yy < mapheight; yy++) { 
        for (xx = 0; xx < mapwidth; xx++) { 
			if(side == 0){
                if (xx == 0) {
                LEVEL_TILES[yy][xx] = 8;
                
            }
                if ((xx == 1)) {
                if((random() % 100)>=65){
				LEVEL_TILES[yy][xx] = 8;
			}
            }   
            }
            if(side == 1){
                if (yy == 0) {
                LEVEL_TILES[yy][xx] = 8;
            }
                if ((yy == 1)) {
                if((random() % 100)>=65){
				LEVEL_TILES[yy][xx] = 8;
			}
            }
            }
            if(side == 2){
                if (xx == mapwidth-1) {
                LEVEL_TILES[yy][xx] = 8;
            }
                if ((xx == mapwidth-2)) {
                if((random() % 100)>=65){
				LEVEL_TILES[yy][xx] = 8;
			}
            }
            }
            if(side == 3){
                if (yy == mapheight-1) {
                LEVEL_TILES[yy][xx] = 8;
            }
                if ((yy == mapheight-2)) {
                if((random() % 100)>=65){
				LEVEL_TILES[yy][xx] = 8;
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

                LEVEL_TILES[yy][xx - 1] = 2;
                LEVEL_TILES[yy][xx] = 9;
                LEVEL_TILES[yy][xx + 1] = 2;

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
void makeRoom(u16 yy, u16 xx, u16 mapheight, u16 mapwidth, u16 type){

    initMap(yy, xx, mapheight, mapwidth);
    makeGrass(yy, xx, mapheight, mapwidth);    
    makeRocks(yy, xx, mapheight, mapwidth, 3);
    makeRocks(yy, xx, mapheight, mapwidth, 8); 
    makeGrass(yy, xx, mapheight, mapwidth);
    randWalk(7, 8, mapheight, mapwidth, 32);

    switch(type) {
        case 1:
            makeWall(yy, xx, mapheight, mapwidth, 0);
            makeWall(yy, xx, mapheight, mapwidth, 1);
            break;
        case 2:
            makeWall(yy, xx, mapheight, mapwidth, 1);
            break;
        case 3:
            makeWall(yy, xx, mapheight, mapwidth, 1);
            makeWall(yy, xx, mapheight, mapwidth, 2);
            break;
        case 4:
            makeWall(yy, xx, mapheight, mapwidth, 0);
            break;
        case 5:
            makeExit(yy, xx, mapheight, mapwidth);
            break;
        case 6:
            makeWall(yy, xx, mapheight, mapwidth, 2);
            break;
        case 7:
            makeWall(yy, xx, mapheight, mapwidth, 0);
            makeWall(yy, xx, mapheight, mapwidth, 3);
            break;
        case 8:
            makeWall(yy, xx, mapheight, mapwidth, 3);
            break;
        case 9:
            makeWall(yy, xx, mapheight, mapwidth, 3);
            makeWall(yy, xx, mapheight, mapwidth, 2);
            break;
        case 10:
            
            break;
        default:
           
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
    
    randWorldWalk(7,7,10);
    
    u16 index = 1;
    for(roomY = 0; roomY < 8; roomY++){
        for(roomX = 0; roomX < 8; roomX++){
            ///
            if(roomY == 0 && roomX == 0){
                makeRoom(0, 0, 14, 16, 1);
            }            
            else if(roomY == 0 && roomX == 7){
                makeRoom(0, 0, 14, 16, 3);
            }
            else if(roomY == 0){
                makeRoom(0, 0, 14, 16, 2);
            }
            ///
            else if(roomY == 7 && roomX == 0){
                makeRoom(0, 0, 14, 16, 7);
            }
            else if(roomY == 7 && roomX == 7){
                makeRoom(0, 0, 14, 16, 9);
            }
            else if(roomY == 7){
                makeRoom(0, 0, 14, 16, 8);
            }
            else if(roomX == 7){
                makeRoom(0, 0, 14, 16, 6);
            }

            else if((roomX == 0)&&!(roomY == 7)){
                makeRoom(0, 0, 14, 16, 4);
            }
            else if(roomY == 3 && roomX == 3){
                makeRoom(0, 0, 14, 16, 5);
            }
            else{
                makeRoom(0, 0, 14, 16, 10);
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
				create16by16TileRand(TRUE, 10,10,12,12);
            }

            if (LEVEL_COL2[mapIndex] == 1) {
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 1), xx, yy);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, TRUE, 1), xx + 1, yy);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 0, TRUE, TRUE, 1), xx + 1, yy + 1);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 0, TRUE, FALSE, 1), xx, yy + 1);
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

                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 12), xx - 2, yy);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 84), xx - 1, yy);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 89), xx - 1, yy + 1);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 12), xx - 2, yy + 1);

                //middle
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 63), xx, yy - 4);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 64), xx + 1, yy - 4);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 69), xx + 1, yy - 3);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 68), xx, yy - 3);

                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 74), xx, yy - 2);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 75), xx + 1, yy - 2);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 81), xx + 1, yy - 1);
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, FALSE, FALSE, 80), xx, yy - 1);

                VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 85), xx, yy);
                VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 86), xx + 1, yy);
                VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 91), xx + 1, yy + 1);
                VDP_setTileMapXY(BG_B, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 90), xx, yy + 1);

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

        }
    }

}
void makeMap(){

    
    VDP_loadTileSet(tileset1.tileset, 1, DMA);
    PAL_setPalette(PAL1, tileset1.palette->data, DMA);
    
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
