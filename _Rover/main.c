#include <gb/gb.h>
#include <stdio.h>
#include "rover.c"

INT8 playerlocation[2]; // stores 2 int8 variables - x, y positions of player
// variables below - vital for jump() method:
BYTE jumping; // stores 2 bytes of data 0 or 1
UINT8 gravity = -2;
UINT8 currentspeedY;
UINT8 floorYPosition = 100;

void performantdelay(UINT8 numloops) { // much more efficient delay method
    UINT8 i;
    for (i = 0; i < numloops; i++) {
        wait_vbl_done();
        /** HALTs the CPU and waits for the vertical blank interrupt (VBL) to finish.

        This is often used in main loops to idle the CPU at low power
        until it's time to start the next frame. It's also useful for
        syncing animation with the screen re-draw. */
    }
}

INT8 wouldhitsurface(UINT8 projectedYPosition){
    // method assessing if player hit the floor surface
    // if position is larger or equals the floor, return floor position - has hit the floor
    if (projectedYPosition >= floorYPosition) {
        return floorYPosition;
    }
    return -1; // otherwise return '-1', assessed later in the conditional
}

void jump(UINT8 spriteId, UINT8 spriteLocation[2]){
    INT8 possiblesurfaceY; // variable for storing temporary Y position, when falling

    if (jumping == 0) {
        jumping = 1;
        currentspeedY = 10;
    }

    currentspeedY = currentspeedY + gravity; // gravity effect when jumping, 
    // in 'while' loop below, speed will decelerate by gravity value each loop

    spriteLocation[1] = spriteLocation[1] - currentspeedY; // new location in Y direction

    possiblesurfaceY = wouldhitsurface(spriteLocation[1]); // we asses if we had hit the floor or not

    if (possiblesurfaceY > -1) {
        // conditional to assess the hit floor, if so then jumping is 0 and we set new 
        // position to the 'floorYPosition', so that sprite does not go past it
        jumping = 0;
        move_sprite(spriteId, spriteLocation[0], possiblesurfaceY);
    } 
    else {
        move_sprite(spriteId, spriteLocation[0], spriteLocation[1]); // if not, 'keep falling'
    }
}

void main() {
    set_sprite_data(0, 1, rover); 
    set_sprite_tile(0, 0);

    playerlocation[0] = 10;
    playerlocation[1] = floorYPosition;
    jumping = 0;

    move_sprite(0, playerlocation[0], playerlocation[1]);
   
    DISPLAY_ON;
    SHOW_SPRITES;

    while (1) {
        if ((joypad() & J_A) || jumping == 1) { // jumping set to 1, needed in jump() method
            jump(0, playerlocation);
        }
        
        if (joypad() & J_LEFT) {
            playerlocation[0] = playerlocation[0] - 2;
            move_sprite(0, playerlocation[0], playerlocation[1]);
        }

        if (joypad() & J_RIGHT) {
            playerlocation[0] = playerlocation[0] + 2;
            move_sprite(0, playerlocation[0], playerlocation[1]);
        }
        performantdelay(5);
    }  
}


