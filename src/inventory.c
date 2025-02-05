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

void addItemm(u16 item, u16 quantity);
void removeItem(u16 item, u16 quantity);
void buyItem(u16 item, u16 quantity);
void sellItem(u16 item, u16 quantity);

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
        skin += quantity;
        break;
        case 4:
        eyes += quantity;
        break;
        case 5:
        fang += quantity;
        break;
        case 6:
        horn += quantity;
        break;
        case 7:
        tail += quantity;
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

void buyItem(u16 item, u16 quantity) {




}

void sellItem(u16 item, u16 quantity) {


}