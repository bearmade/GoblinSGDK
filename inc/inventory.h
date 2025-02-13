#ifndef INVENTORY_H
#define INVENTORY_H

void addItemm(u16 item, u16 quantity);
void removeItem(u16 item, u16 quantity);
void buyItem(u16 item, u16 quantity, u16 itemPrice);
void sellItem(u16 item, u16 quantity, u16 itemPrice);
void showMerchMenu();
void handleMerchantMenuInput();
void resetMerchantPosition();

extern u16 itemPrice;
static u16 menuSelection;
static u16 randomItemForSale;


u16 inventory[4][4]; 
u16 skulls;
u16 skull_base;
u16 meat;
u16 meat_base;
u16 bones;
u16 bones_base;
u16 skin;
u16 skin_base;
u16 eyes;
u16 eyes_base;
u16 fang;
u16 fang_base;
u16 horn;
u16 horn_base;
u16 tail;
u16 tail_base;


char skullName[5];
char meatName[5];
char bonesName[5];
char skinName[5];
char eyesName[5];
char fangName[5];
char hornName[5];
char tailName[5];

char skullAmount[5];
char meatAmount[5];
char bonesAmount[5];
char skinAmount[5];
char eyesAmount[5];
char fangAmount[5];
char hornAmount[5];
char tailAmount[5];


#define SKULLS 0
#define MEAT 1
#define BONES 2
#define SKIN 3
#define EYES 4
#define FANG 5
#define HORN 6
#define TAIL 7





#endif