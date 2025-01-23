#include "../inc/debuginfo.h"
#include "../inc/level.h"


char info[10];

void debuginfo(){

	sprintf(info, "%10i", LEVEL_COL[0]);
	VDP_setTextPalette(2);
	VDP_drawTextBG(BG_B, info, 10, 200);
}
