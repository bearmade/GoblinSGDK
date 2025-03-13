#include "../inc/debuginfo.h"
#include "../inc/level.h"
#include "../inc/camera.h"
#include "../inc/player.h"
#include "../inc/globals.h"
#include <resources.h>
#include "../inc/titlescreen.h"
#include "../inc/makemap.h"
#include "../inc/gamemanager.h"
#include "../inc/battle.h"
#include "../inc/inventory.h"

// void addItemm(u16 item, u16 quantity);
// void removeItem(u16 item, u16 quantity);
// void buyItem(u16 item, u16 quantity, u16 itemPrice);
// void sellItem(u16 item, u16 quantity, u16 itemPrice);
// void resetMerchantPosition();

u16 itemPrice = 0;
static u16 menuSelection = 0;
static u16 randomItemForSale = 0;
// Add at the top with other globals
static u16 merchantInteractions = 0;
static u16 MAX_MERCHANT_INTERACTIONS = 6;

u16 inventory[4][4] =
{
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}

};



u16 skulls = 0;
u16 skull_base = 30;
u16 meat = 0;
u16 meat_base = 5;
u16 bones = 0;
u16 bones_base = 6;
u16 skin = 0;
u16 skin_base = 7;
u16 eyes = 0;
u16 eyes_base = 10;
u16 fang = 0;
u16 fang_base = 15;
u16 horn = 0;
u16 horn_base = 20;
u16 tail = 0;
u16 tail_base = 8;


void addItem(u16 item, u16 quantity){
    switch(item){
        case 0:
        skulls += quantity;
        break;
        case 1:
        meat += quantity;
        break;
        case 2:
        bones += quantity;
        break;
        case 3:
        horn += quantity;
        break;
        case 4:
        eyes += quantity;
        break;
        case 5:
        fang += quantity;
        break;

    }
    

}

void removeItem(u16 item, u16 quantity) {
    switch(item) {
        case 0:
            if (skulls >= quantity) {
                skulls -= quantity;
            }
            break;
        case 1:
            if (meat >= quantity) {
                meat -= quantity;
            }
            break;
        case 2:
            if (bones >= quantity) {
                bones -= quantity;
            }
            break;
        case 3:
            if (skin >= quantity) {
                skin -= quantity;
            }
            break;
        case 4:
            if (eyes >= quantity) {
                eyes -= quantity;
            }
            break;
        case 5:
            if (fang >= quantity) {
                fang -= quantity;
            }
            break;
        case 6:
            if (horn >= quantity) {
                horn -= quantity;
            }
            break;
        case 7:
            if (tail >= quantity) {
                tail -= quantity;
            }
            break;
        default:
            // Handle invalid item case if necessary
            break;
    }
}

void buyItem(u16 item, u16 quantity, u16 itemPrice) {
    // Check if player has enough gold
    if (player_gold >= itemPrice) {
        player_gold -= itemPrice;
        
        // Handle stat upgrades
        if (item == 8) {
           player_hp = player_hp_max;
        }
        else if (item == 9) {
            player_hp_max += (player_level * 10);
            player_hp = player_hp_max;
        }
        else if (item == 3) {
            player_defense += 1;
        } else if (item == 7) {
            player_attack += 1;
        } else {
            addItem(item, quantity);
        }
        
        // Display success message
        VDP_drawTextBG(BG_A, "   Purchased   ", 3, 7);
    } else {
        // Display insufficient funds message
        VDP_drawTextBG(BG_A, "Not enough gold", 3, 7);
    }
}void sellItem(u16 item, u16 quantity, u16 itemPrice) {
    u16 sellPrice = 0;
    
    // Calculate sell price based on item type
    switch(item) {
        case 0:
            if (skulls >= quantity) {
                sellPrice = itemPrice;
                player_gold += sellPrice;
                removeItem(item, quantity);
            }
            break;
        case 1:
            if (meat >= quantity) {
                sellPrice = itemPrice;
                player_gold += sellPrice;
                removeItem(item, quantity);
            }
            break;
        case 2:
            if (bones >= quantity) {
                sellPrice = itemPrice;
                player_gold += sellPrice;
                removeItem(item, quantity);
            }
            break;
        case 4:
            if (eyes >= quantity) {
                sellPrice = itemPrice;
                player_gold += sellPrice;
                removeItem(item, quantity);
            }
            break;
        case 5:
            if (fang >= quantity) {
                sellPrice = itemPrice;
                player_gold += sellPrice;
                removeItem(item, quantity);
            }
            break;
        case 6:
            if (horn >= quantity) {
                sellPrice = itemPrice;
                player_gold += sellPrice;
                removeItem(item, quantity);
            }
            break;
    }
}
void showMerchMenu() {
	randomItemForSale = random() % 10;
	u16 randomItemPrice = random() % 8;
 
	// //u16 itemPrice = 0;
     //player_gold += 10000;
	char itemPriceString[5];
	 // 0 = Buy, 1 = Sell

	if(bShowMerchMenu) {
        //display number of merchant interactions


		SPR_setVisibility(player, HIDDEN);
		VDP_clearTileMap(BG_B, ind, 0, TRUE);
        //VDP_clearTileMap(BG_A, ind, 0, TRUE);
		VDP_loadFontData(tileset_Font.tiles, 96, CPU);
		PAL_setPalette(PAL0, palette_Font.data, DMA);
           // draw merchant face
    drawBox(0, 11, 11, 11);
    PAL_setPalette(PAL2, face.palette->data, DMA);
	VDP_drawImageEx(BG_B, &face, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, ind), 1, 12, FALSE, TRUE);
    
    drawBox(21, 4, 11, 11);
    
    PAL_setPalette(PAL3, merchFace.palette->data, DMA);
	VDP_drawImageEx(BG_B, &merchFace, TILE_ATTR_FULL(PAL3, FALSE, FALSE, FALSE, 256), 22, 5, FALSE, TRUE);
   
		
		// Draw menu options
		//VDP_drawTextBG(BG_A, "Merchant", 4, 2);
        drawBox(0, 0, 21, 11);

        VDP_drawTextBG(BG_A, "I have wares, ", 1, 1);
        VDP_drawTextBG(BG_A, "if you have coin", 2, 3);


        
		switch (randomItemForSale) {
			case 0:
				VDP_drawTextBG(BG_A, "Skull", 2, 5);
				itemPrice = skull_base + (random() % skull_base);
				sprintf(itemPriceString, "%d", itemPrice);
				VDP_drawTextBG(BG_A, itemPriceString, 9, 5);
				VDP_drawTextBG(BG_A, " Gold?", 13, 5);
				break;
			case 1:
				VDP_drawTextBG(BG_A, "Meat", 2, 5);
				itemPrice = meat_base + (random() % meat_base);
				sprintf(itemPriceString, "%d", itemPrice);
				VDP_drawTextBG(BG_A, itemPriceString, 9, 5);
				VDP_drawTextBG(BG_A, " Gold?", 13, 5);
				break;
			case 2:
				VDP_drawTextBG(BG_A, "Bones", 2, 5);
				itemPrice = bones_base + (random() % bones_base);
				sprintf(itemPriceString, "%d", itemPrice);
				VDP_drawTextBG(BG_A, itemPriceString, 9, 5);
				VDP_drawTextBG(BG_A, " Gold?", 13, 5);
				break;
			case 3:
				VDP_drawTextBG(BG_A, "DEF UP", 2, 5);
				itemPrice = player_defense * (10 + random() % 10);
				sprintf(itemPriceString, "%d", itemPrice);
				VDP_drawTextBG(BG_A, itemPriceString, 9, 5);
				VDP_drawTextBG(BG_A, " Gold?", 13, 5);	
				break;
			case 4:
				VDP_drawTextBG(BG_A, "Eyes", 2, 5);
				itemPrice = eyes_base + (random() % eyes_base);
				sprintf(itemPriceString, "%d", itemPrice);
				VDP_drawTextBG(BG_A, itemPriceString, 9, 5);
				VDP_drawTextBG(BG_A, " Gold?", 13, 5);
				break;
			case 5:
				VDP_drawTextBG(BG_A, "Fangs", 2, 5);
				itemPrice = fang_base + (random() % fang_base);
				sprintf(itemPriceString, "%d", itemPrice);
				VDP_drawTextBG(BG_A, itemPriceString, 9, 5);
				VDP_drawTextBG(BG_A, " Gold?", 13, 5);
				break;
			case 6:
				VDP_drawTextBG(BG_A, "Horn", 2, 5);
				itemPrice = horn_base + (random() % horn_base);
				sprintf(itemPriceString, "%d", itemPrice);
				VDP_drawTextBG(BG_A, itemPriceString, 9, 5);
				VDP_drawTextBG(BG_A, " Gold?", 13, 5);
				break;
			case  7:
				VDP_drawTextBG(BG_A, "ATK UP", 2, 5);
				itemPrice = player_attack * (10 + random() % 10);
				sprintf(itemPriceString, "%d", itemPrice);
				VDP_drawTextBG(BG_A, itemPriceString, 9, 5);
				VDP_drawTextBG(BG_A, " Gold?", 13, 5);
				break;
            case 8:
                VDP_drawTextBG(BG_A, "Health", 2, 5);
                itemPrice = (player_hp_max - player_hp) * 2;
                sprintf(itemPriceString, "%d", itemPrice);
                VDP_drawTextBG(BG_A, itemPriceString, 9, 5);
                VDP_drawTextBG(BG_A, " Gold?", 13, 5);
                break;
            case 9:
                VDP_drawTextBG(BG_A, "HP UP", 2, 5);
                itemPrice = (player_hp_max) * 2;
                sprintf(itemPriceString, "%d", itemPrice);
                VDP_drawTextBG(BG_A, itemPriceString, 9, 5);
                VDP_drawTextBG(BG_A, " Gold?", 13, 5);
                break;
		}
	}
	else {
		// When closing menu, set new random merchant position
		resetMerchantPosition();
		//show player sprite
        PAL_setPalette(PAL2, our_sprite.palette->data, DMA);
        PAL_setPalette(PAL3, merchantSprite.palette->data, DMA);
		SPR_setVisibility(player, VISIBLE);
		//redraw screen
		VDP_loadTileSet(tileset1.tileset, 1, DMA);
		PAL_setPalette(PAL1, tileset1.palette->data, DMA);
		PAL_setPalette(PAL0, fg2.palette->data, DMA);

		VDP_drawTextBG(BG_A, "      ", 4, 2);
		VDP_drawTextBG(BG_A, "      ", 4, 4);
		VDP_drawTextBG(BG_A, "      ", 8, 4);
		VDP_drawTextBG(BG_A, "      ", 4, 6);
		displayRoom();
	}
}

		// Handle input for menu selection
			void handleMerchantMenuInput() {
                char merchantInteractionsString[5];
                sprintf(merchantInteractionsString, "%d", merchantInteractions);
                drawBox(21, 0, 11, 4);
                VDP_drawTextBG(BG_A, merchantInteractionsString, 25, 2);
                VDP_drawTextBG(BG_A, "/", 27, 2);
                char maxMerchInteractionsString[5];
                sprintf(maxMerchInteractionsString, "%d", MAX_MERCHANT_INTERACTIONS);
                VDP_drawTextBG(BG_A, maxMerchInteractionsString, 28, 2);
                //VDP_drawTextBG(BG_A, "5", 25, 2);
                VDP_drawTextBG(BG_A, (menuSelection == 0 ? "~ Buy" : "  Buy"), 13, 12);
		        VDP_drawTextBG(BG_A, (menuSelection == 1 ? "~ Sell" : "  Sell"), 13, 13);
                char goldDisplay[16];
                sprintf(goldDisplay, "Gold: %d", player_gold);
                VDP_drawTextBG(BG_A, goldDisplay, 1, 9);
                drawBox(11, 15, 21, 7);
                drawBox(11, 11, 10, 4);

                VDP_drawTextBG(BG_A, "Skull:", 12, 16);
                sprintf(skullAmount, "%d ", skulls);
                VDP_drawTextBG(BG_A, skullAmount, 18, 16);
                VDP_drawTextBG(BG_A, "Meat:", 12, 18);
                sprintf(meatAmount, "%d ", meat);
                VDP_drawTextBG(BG_A, meatAmount, 18, 18);
                VDP_drawTextBG(BG_A, "Bone:", 12, 20);
                sprintf(bonesAmount, "%d ", bones);
                VDP_drawTextBG(BG_A, bonesAmount, 18, 20);
                // VDP_drawTextBG(BG_B, "Skin:", 12, 22);
                // sprintf(skinAmount, "%d", skin);
                // VDP_drawTextBG(BG_B, skinAmount, 20, 22);
                VDP_drawTextBG(BG_A, "Eye:", 22, 16);
                sprintf(eyesAmount, "%d ", eyes);
                VDP_drawTextBG(BG_A, eyesAmount, 27, 16);
                VDP_drawTextBG(BG_A, "Fang:", 22, 18);
                sprintf(fangAmount, "%d ", fang);
                VDP_drawTextBG(BG_A, fangAmount, 27, 18);
                VDP_drawTextBG(BG_A, "Horn:", 22, 20);
                sprintf(hornAmount, "%d ", horn);
                VDP_drawTextBG(BG_A, hornAmount, 27, 20);

                drawBox(0, 22, 32, 6);
                VDP_drawTextBG(BG_A, "A:Confirm  B:Next  C:Leave", 3, 24);
                
                
                if(JOY_readJoypad(JOY_1) & BUTTON_B) {
                    merchantInteractions++;
                    if(merchantInteractions >= MAX_MERCHANT_INTERACTIONS + 1) {
                        bShowMerchMenu = FALSE;
                        merchantInteractions = 0;
                        showMerchMenu();
					    bPlayerCanMove = TRUE;
					    SPR_setVisibility(merchant, VISIBLE);
					    for( int i = 0; i < 24; i++){
						    VDP_clearTextLine(i);
					    }
                        displayRoom();
                    } else {
                        // Refresh merchant inventory
                        randomItemForSale = random() % 10;
                        showMerchMenu();
                    }
                    waitMs(200);
                }

				if(JOY_readJoypad(JOY_1) & BUTTON_UP) {
					if (menuSelection > 0) {
						menuSelection -= 1;
                        waitMs(50);
					}
				}
	
				if(JOY_readJoypad(JOY_1) & BUTTON_DOWN) {
					if (menuSelection < 1) {
						menuSelection += 1;
                        waitMs(50);
					}
				}
	
				if(JOY_readJoypad(JOY_1) & BUTTON_A) {
					if(menuSelection == 0) {
						buyItem(randomItemForSale, 1, itemPrice);
                        sprintf(goldDisplay, "Gold: %d", player_gold);
                        VDP_drawTextBG(BG_A, "            ", 1, 9);
                        VDP_drawTextBG(BG_A, goldDisplay, 1, 9);
                        SYS_doVBlankProcess();
                        waitMs(200);
					} else {
						sellItem(randomItemForSale, 1, itemPrice);
                        sprintf(goldDisplay, "Gold: %d", player_gold);
                        VDP_drawTextBG(BG_A, "            ", 1, 9);
                        VDP_drawTextBG(BG_A, goldDisplay, 1, 9);                        
                        SYS_doVBlankProcess();
                        waitMs(200);
					}
                }

             
                

			}

void resetMerchantPosition() {
    //MAX_MERCHANT_INTERACTIONS = (random() % 10) + 5;
    VDP_clearTextLine(24);
    merchWorldX = random() % 8;
    merchWorldY = random() % 8;
    while(WORLD_LAYOUT[merchWorldY][merchWorldX] != 1) {
        merchWorldX = random() % 8;
        merchWorldY = random() % 8;
    }
}

//function to reset merchant interactions
void resetMerchantInteractions() {
    MAX_MERCHANT_INTERACTIONS = (random() % 10) + 5;
    merchantInteractions = 0;
}
