#include "../inc/level.h"
#include "../inc/globals.h"
#include <resources.h>


Map* level_1_map;
Map* level_2_map;
u16 LENGTH_OF_LEVEL_ARRAY = 16;
u16 SIZE_OF_LEVEL_ARRAY = 224;
u16 LEVEL_COL[224] = { 0 };
u16 LEVEL_COL2[224] = { 0 };

void displayLevel(){
	VDP_loadTileSet(&our_tileset, ind, DMA);
	 level_1_map = MAP_create(&our_level_map, BG_B, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, ind));
	 PAL_setPalette(PAL0, our_palette.data, DMA);
	 MAP_scrollTo(level_1_map, 0, 0);


}
