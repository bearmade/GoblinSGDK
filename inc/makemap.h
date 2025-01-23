#ifndef MAKE_MAP_H
#define MAKE_MAP_H

#include <genesis.h>



u16 countNeighbors(u16 type, u16 y, u16 x);
u16 countNeighborsEX(u16 type, u16 y, u16 x);
u16 countNeighborsBigMap(u16 type, u16 y, u16 x);
u16 randSet(u16 a, u16 b, u16 c, u16 d);
void initMap(u16 yy, u16 xx, u16 mapheight, u16 mapwidth);
void makeRocks(u16 yy, u16 xx, u16 mapheight, u16 mapwidth, u16 type);
void makeWall(u16 yy, u16 xx, u16 mapheight, u16 mapwidth, u16 side);
void makeGrass(u16 yy, u16 xx, u16 mapheight, u16 mapwidth);
void makeExit(u16 yy, u16 xx, u16 mapheight, u16 mapwidth);
void makePlayerSpawn(u16 yy, u16 xx, u16 mapheight, u16 mapwidth);
void makeInnerWall(u16 yy, u16 xx, u16 mapheight, u16 mapwidth);
void makeTreeTops(u16 yy, u16 xx, u16 mapheight, u16 mapwidth);
void makeDoorways();
void randWalk(u16 yy, u16 xx, u16 mapheight, u16 mapwidth, u16 steps);
void randWorldWalk(u16 mapheight, u16 mapwidth, u16 steps);
void ruleTileWORLD();
void makeRoom(u16 yy, u16 xx, u16 mapheight, u16 mapwidth, u16 type);
void updateCurrentRoom();
void convertMapArrays();
void displayRoom();
void create16by16TileRand(bool vertFlip, u16 a, u16 b, u16 c, u16 d);
void create16by16Tile(bool horFlip, bool topPrio, bool bottomPrio, bool topBG, bool bottomBG, u16 a, u16 b, u16 c, u16 d);
void create16by16TileIndexed(bool horFlip, bool topPrio, bool bottomPrio, bool topBG, bool bottomBG, u16 ind);
void create32by16TileIndexed(bool horFlip, bool topPrio, bool bottomPrio, bool topBG, bool bottomBG, u16 ind);
void makeWorldMap();
void bigMapCA();
void makeRocksBigMap(u16 yy, u16 xx, u16 mapheight, u16 mapwidth, u16 type);

void makeMap();
extern u8 LEVEL_TILES[14][16];
extern u8 WORLD_TILES[8][8][14][16];
//extern u8 UNDERWORLD_TILES[8][8][14][16];
extern u8 WORLD_LAYOUT[8][8]; 
extern u8 WORLD_LAYOUT_CA[112][128];
//extern u8 tempWORLD_LAYOUT_CA[112][128]; 
extern u16 roomY;
extern u16 roomX;
extern u16 currentWorldX;
extern u16 currentWorldY;
extern u16 mw;
extern u16 mh;
extern u16 mapwidth;
extern u16 mapheight;
extern u16 mapIndex;    
extern u16 result;
extern u16 xx, yy;
extern u16 buildingPosX;
extern u16 buildingPosY;
bool enemyDisplayed;

#endif