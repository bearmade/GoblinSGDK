
#include "../inc/player.h"
#include "../inc/dungeonGenerator.h"
#include "../inc/makemap.h"   
#include "../inc/level.h"      


#define FLOOR_TILE  1  
#define WALL_TILE   6  
#define EMPTY_TILE  0  
#define DUNGEON_HEIGHT 32
#define DUNGEON_WIDTH  40



// Carve out a starting room in the center of dungeonLayout.
static void carveStartingRoom(u16 mapHeight, u16 mapWidth, u16 dungeonLayout[][mapWidth]) {
    const u16 roomWidth = 3;
    const u16 roomHeight = 3;
    u16 startY = (mapHeight - roomHeight) / 2;
    u16 startX = (mapWidth - roomWidth) / 2;
    
    for(u16 y = startY; y < startY + roomHeight; y++){
        for(u16 x = startX; x < startX + roomWidth; x++){
            dungeonLayout[y][x] = FLOOR_TILE;
        }
    }
}

// Outline rooms with walls by checking neighbors.
static void outlineWalls(u16 mapHeight, u16 mapWidth, u16 dungeonLayout[][mapWidth]) {
   
    u16 temp[mapHeight][mapWidth];
    for(u16 y = 0; y < mapHeight; y++){
        for(u16 x = 0; x < mapWidth; x++){
            temp[y][x] = dungeonLayout[y][x];
        }
    }
    
  
    for(u16 y = 1; y < mapHeight - 1; y++){
        for(u16 x = 1; x < mapWidth - 1; x++){
            if(temp[y][x] == FLOOR_TILE){
                if(temp[y-1][x] == EMPTY_TILE) dungeonLayout[y-1][x] = WALL_TILE;
                if(temp[y+1][x] == EMPTY_TILE) dungeonLayout[y+1][x] = WALL_TILE;
                if(temp[y][x-1] == EMPTY_TILE) dungeonLayout[y][x-1] = WALL_TILE;
                if(temp[y][x+1] == EMPTY_TILE) dungeonLayout[y][x+1] = WALL_TILE;
            }
        }
    }
}

// A version of random walk that operates on our dungeonLayout array.
static void dungeonRandomWalk(u16 mapHeight, u16 mapWidth, u16 steps, u16 dungeonLayout[][mapWidth]) {

    u16 currentY = mapHeight / 2;
    u16 currentX = mapWidth / 2;   
    
    // Directions: up, down, left, right.
    const int dx[] = { 0, 0, -1, 1 };
    const int dy[] = { -1, 1, 0, 0 };    
    
    for (u16 step = 0; step < steps; step++) {
        // Carve out the current tile.
        dungeonLayout[currentY][currentX] = FLOOR_TILE;
        u16 direction = random() % 4;
        u16 nextY = currentY + dy[direction];
        u16 nextX = currentX + dx[direction];
        
        // bounds check
        if (nextY > 0 && nextY < mapHeight - 1 && nextX > 0 && nextX < mapWidth - 1) {
            currentY = nextY;
            currentX = nextX;
        }
    }
}

void generateDungeon(u16 mapHeight, u16 mapWidth, u16 steps, u16 dungeonLayout[][mapWidth]) {

    for(u16 y = 0; y < mapHeight; y++){
        for(u16 x = 0; x < mapWidth; x++){
            dungeonLayout[y][x] = EMPTY_TILE;
        }
    }
 
    carveStartingRoom(mapHeight, mapWidth, dungeonLayout);

    dungeonRandomWalk(mapHeight, mapWidth, steps, dungeonLayout);

    outlineWalls(mapHeight, mapWidth, dungeonLayout);

}

void setupDungeon(void) {

    generateDungeon(DUNGEON_HEIGHT, DUNGEON_WIDTH, 100, myDungeon);
    

}


void drawDungeonMap(u16 mapHeight, u16 mapWidth, u16 dungeonLayout[][mapWidth]) {
    for(u16 y = 0; y < mapHeight; y++){
        for(u16 x = 0; x < mapWidth; x++){

            u16 tileId = dungeonLayout[y][x];


            VDP_setTileMapXY(BG_A,
                TILE_ATTR_FULL(PAL0, 0, 0, 0, tileId),
                x, y);
        }
    }
}
