#include <gb/gb.h>
#include <stdio.h>
#include "GameCharacter.c"
#include "gamesprites.c"
#include "simplebackground.c"
#include "simplebackgroundmap.c"

struct GameCharacter ship;
struct GameCharacter bug;
UBYTE spritesize = 8;

void performantdelay(UINT8 numloops) {
    UINT8 i;
    for (i = 0; i < numloops; i++) {
        wait_vbl_done();
    }
}

UBYTE checkCollisions(struct GameCharacter* one, struct GameCharacter* two) {
    return ((one->x >= two->x) && (one->x <= two->x + two->width)) 
    && ((one->y >= two->y) && (one->y <= two->y + two->height)) || 

    ((two->x >= one->x) && (two->x <= one->x + one->width)) 
    && ((two->y >= one->y) && (two->y <= one->y + one->height));
}

void moveGameCharacter(struct GameCharacter* character, 
                        UINT8 x, UINT8 y) {
    move_sprite(character->spritids[0], x, y);
    move_sprite(character->spritids[1], x+spritesize, y);
    move_sprite(character->spritids[2], x, y+spritesize);
    move_sprite(character->spritids[3], x+spritesize, y+spritesize);
}

void setupShip() {
    ship.x = 80;
    ship.y = 130;
    ship.width = 16;
    ship.height = 16;

    // load sprites for ship
    set_sprite_tile(0,0);
    ship.spritids[0] = 0;
    set_sprite_tile(1,1);
    ship.spritids[1] = 1;
    set_sprite_tile(2,2);
    ship.spritids[2] = 2;
    set_sprite_tile(3,3);
    ship.spritids[3] = 3;

    moveGameCharacter(&ship, ship.x, ship.y);
}

void setupBug() {
    bug.x = 30;
    bug.y = 0;
    bug.width = 16;
    bug.height = 16;

    // load sprites for bug
    set_sprite_tile(4,4);
    bug.spritids[0] = 4;
    set_sprite_tile(5,5);
    bug.spritids[1] = 5;
    set_sprite_tile(6,6);
    bug.spritids[2] = 6;
    set_sprite_tile(7,7);
    bug.spritids[3] = 7;

    moveGameCharacter(&bug, bug.x, bug.y);
}
void main() {
    set_bkg_data(0,15,backgroundtiles);
    set_bkg_tiles(0,0,20,54,spacemap);

    set_sprite_data(0, 8, gamesprites);
    setupShip();
    setupBug();

    SHOW_SPRITES; 
    SHOW_BKG;
    DISPLAY_ON;

    while (!checkCollisions(&ship, &bug)) {
        scroll_bkg(0,-1);
        switch (joypad())
        {
        case J_LEFT:
            ship.x -= 10;
            moveGameCharacter(&ship, ship.x, ship.y);
            break;
        case J_RIGHT:
            ship.x += 10;
            moveGameCharacter(&ship, ship.x, ship.y);
            break;
        case J_UP:
            ship.y -= 10;
            moveGameCharacter(&ship, ship.x, ship.y);
            break;
        case J_DOWN:
            ship.y += 10;
            moveGameCharacter(&ship, ship.x, ship.y);
            break;
        }

        bug.y += 5;
        if (bug.y >= 144)
        {
            bug.y=0;
            bug.x=ship.x;
        }
        moveGameCharacter(&bug, bug.x, bug.y);
        
        performantdelay(5);
    }

    printf("\n\n\n\n\n\n\n=== GAME OVER ===");
    
}