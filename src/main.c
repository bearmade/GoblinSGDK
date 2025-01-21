
#include <genesis.h>
#include <resources.h>


void delayVBlank(u16 vblanks);
void vblankCallback();
void inputHandler(u16 joy, u16 changed, u16 state);
void nameGenerator();
void initMap();
void showMenu();
void drawMap();


#define MAP_WIDTH 32
#define MAP_HEIGHT 20
#define TEXT_COLOR 5
#define WALL_COLOR 13
#define FLOOR_COLOR 0
#define PLAYER_COLOR 1
#define ATTACK_COLOR 7
#define SNAKE_COLOR 7

u16 ind = TILE_USER_INDEX;
char map[MAP_WIDTH][MAP_HEIGHT];
u16 playerX, playerY;
u16 displayX, displayY;
u16 playerDir;
char tempMapA = ';';
char tempMapB = ';';
char tempMapC = ';';
u16 delayCounter;
bool is_start_pressed = 0;
u16 random_number;
char Name[19];
char noS = 'n';
u16 sCount = 0;
u16 total_sCount = 0;
u16 floorCount = 0;

int main()
{

    VDP_loadTileSet(tileset1.tileset, 1, DMA);
    PAL_setPalette(PAL1, tileset1.palette->data, DMA);
    VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 26), 1, 1);
    
	VDP_setScreenWidth256();//256 pixels / 8 = 32 tiles    
	VDP_setScreenHeight224();//224 pixels / 8 = 28 tiles
    JOY_init();
    JOY_setEventHandler(inputHandler);
    showMenu();
    delayCounter = 0;
    SYS_setVIntCallback(vblankCallback);
    char numString[2];
    initMap();
    drawMap();

    while(1)
    {
        
        if(is_start_pressed)
        {
            sprintf(numString, "%u", random_number);
            VDP_drawText(numString, 10, 13);
            VDP_drawText("Start Pressed", 8,23);
        }
        else
        {
            VDP_clearTextArea(10, 13, 21, 1);
            VDP_clearTextArea(8, 23, 21, 1);
        }
        char numberString[3];
		sprintf(numberString, "%u", is_start_pressed);
        VDP_drawText(numberString, 24, 8);
        
        if(random_number > 75 && is_start_pressed)
        {
            VDP_drawText(Name, 10, 20);
        }
        else 
        {
            nameGenerator();
            VDP_clearTextArea(10, 20, 21, 1);
        }

        VDP_waitVSync();
        SYS_doVBlankProcess();
    }
    return (0);
}
void inputHandler(u16 joy, u16 changed, u16 state){


        if (changed & state & BUTTON_START)
		{
       
            random_number = random() % 100;
			if(is_start_pressed){
                is_start_pressed = 0;
            }
            else {
                is_start_pressed = 1;
            }
		}
}
void nameGenerator(){

    char firstName1[5][2] = {"Sn", "Sk", "Kr", "Br", "Gr"};
    char firstName2[5][2] = {"ar", "um", "ay", "ee", "oo"};
    char firstName3[5][3] = {"po ", "py ", "bs ", "ble", "gle"};
    char middleName[5][3] = {"Tum", "Bum", "Rub", "Hum", "Gru"};
    char lastName1[5][4] = {"thum", "bopo", "arum", "atum", "abum"};
    char lastName2[5][4] = {"lo  ", "bles", "gles", "po  ", "py  "};
    u16 rand = random() % 5;
    u16 rand2 = random() % 5;
    u16 rand3 = random() % 5;
    u16 rand4 = random() % 5;
    u16 rand5 = random() % 5;
    u16 rand6 = random() % 5;
    u16 rand7 = random() % 5;
    u16 rand8 = random() % 5;
    Name[0] = firstName1[rand][0];
    Name[1] = firstName1[rand][1];
    Name[2] = firstName2[rand2][0];
    Name[3] = firstName2[rand2][1];
    Name[4] = firstName3[rand3][0];
    Name[5] = firstName3[rand3][1];
    Name[6] = firstName3[rand3][2];
    Name[7] = ' ';
    Name[8] = middleName[rand4][0];
    Name[9] = middleName[rand4][1];
    Name[10] = middleName[rand4][2];
    Name[11] = lastName1[rand5][0];
    Name[12] = lastName1[rand5][1];
    Name[13] = lastName1[rand5][2];
    Name[14] = lastName1[rand5][3];
    Name[15] = lastName2[rand6][0];
    Name[16] = lastName2[rand6][1];
    Name[17] = lastName2[rand6][2];
    Name[18] = lastName2[rand6][3];
    sprintf(Name, "%s", Name);

 }


void delayVBlank(u16 vblanks) {
    delayCounter = vblanks;
    while (delayCounter > 0) {
        SYS_doVBlankProcess();
    }
}
void vblankCallback() {
    if (delayCounter > 0) {
        delayCounter--;
    }
}
void showMenu() {
    
    VDP_drawText("Press START to generate map", displayX, displayY + 3);
    
    while (1) {
        u16 value = JOY_readJoypad(JOY_1);
        
        if (value & BUTTON_START) {
			VDP_drawText("                            ", displayX, displayY + 3);
            break;  // Exit the menu loop when Start button is pressed
        }
        
        SYS_doVBlankProcess();
    }
}

void initMap() {
    for (u16 i = 0; i < MAP_WIDTH; i++) {
        for (u16 j = 0; j < MAP_HEIGHT; j++) {
            map[i][j] = ' ';
        }
    }
}   

void drawMap() {
    for (u16 i = 0; i < MAP_WIDTH; i++) {
        for (u16 j = 0; j < MAP_HEIGHT; j++) {
            if (map [i][j] == ' ') {
                VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 0, FALSE, FALSE, 26), i, j);
            }
        
        }
    }

}