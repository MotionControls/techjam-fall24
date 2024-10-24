#ifndef SPRITE_TABLES
#define SPRITE_TABLES

// Sprite Tables
// The way these tables work is that each value represents a point in memory from the OAM tileset.
// Each value should point towards the top-left corner of a given sprite.
// The location of each sprite (after loading) can be easily seen through something like Mesen.
const u8 playerSpriteTable[12] = {
    0x0,0x2,0x4,0x6,	// Down
	0x8,0xA,0xC,0xE,	// Up
	0x40,0x42,0x44,0x46	// Side
};

// Structs
enum PlayerStates {
    PS_DOWN = 0,
	PS_UP = 4,
	PS_SIDE = 8
};

#endif