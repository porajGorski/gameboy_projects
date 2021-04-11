#include <gb/gb.h>

// generical character structure: id, position, graphics

struct GameCharacter
{
    UBYTE spritids[4];
    UINT8 x;
    UINT8 y;
    UINT8 width;
    UINT8 height;
};
