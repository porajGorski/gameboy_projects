#include <gb/gb.h>
#include <stdio.h>
#include "MapTiles.c"
#include "PlanetMap.c"
#include "astronaut.c"

const char blankmap[1] = {0x00};
UINT8 playerlocation[2];

UBYTE canPlayerMove(UINT8 newplayer_x, UINT8 newplayer_y) {
    INT16 indexTLx, indexTLy, tileIndexTL; // TL - top left
    UBYTE result;

    indexTLx = (newplayer_x - 8) / 8; // we take care of the offset 8
    indexTLy = (newplayer_y - 16) / 8;
    tileIndexTL = 20 * indexTLy + indexTLx;

    result = PlanetMap[tileIndexTL] == blankmap[0];

    return result;
}

void animateSprite(UINT8 spriteindex, INT8 move_x, INT8 move_y){
    while (move_x != 0)
    {
        scroll_sprite(spriteindex, move_x < 0 ? -1 : 1, 0);
        move_x += (move_x < 0 ? 1 : -1);
        wait_vbl_done();
    }
    while (move_y != 0)
    {
        scroll_sprite(spriteindex, 0, move_y < 0 ? -1 : 1);
        move_y += (move_y < 0 ? 1 : -1);
        wait_vbl_done();
    }
}

void performantdelay(UINT8 numloops) {
    UINT8 i;
    for (i = 0; i < numloops; i++) {
        wait_vbl_done();
    }
}
void main() {
    UINT8 currentspriteindex = 0;

    set_bkg_data(0, 10, MapTiles);
    set_bkg_tiles(0,0,20,20, PlanetMap);

    set_sprite_data(0, 12, astronaut); 
    set_sprite_tile(0, currentspriteindex);

    playerlocation[0] = 24;
    playerlocation[1] = 32;
    move_sprite(0, playerlocation[0], playerlocation[1]);
    SHOW_SPRITES;
    SHOW_BKG;
    DISPLAY_ON;

    while (1) {
        switch (joypad())
        {
        case J_LEFT:
            if (canPlayerMove(playerlocation[0]-8,playerlocation[1]))
            {
                currentspriteindex = 6;
                set_sprite_tile(0, currentspriteindex);
                animateSprite(0, -8, 0);
            }
            break;
        case J_RIGHT:
            if (canPlayerMove(playerlocation[0]+8,playerlocation[1]))
            {
                currentspriteindex = 3;
                set_sprite_tile(0, currentspriteindex);
                animateSprite(0, 8, 0);
            }
            break;
        case J_UP:
            if (canPlayerMove(playerlocation[0],playerlocation[1]-8))
            {
                currentspriteindex = 9;
                set_sprite_tile(0, currentspriteindex);
                animateSprite(0, 0, -8);
            }
            break;
        case J_DOWN:
            if (canPlayerMove(playerlocation[0],playerlocation[1]+8))
            {
                currentspriteindex = 0;
                set_sprite_tile(0, currentspriteindex);
                animateSprite(0, 0, 8);
            }
            break;
        }
        performantdelay(6);
    }
    
}