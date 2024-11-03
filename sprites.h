#ifndef SPRITES_H
#define SPRITES_H

// Sprite Tables
// The way these tables work is that each value represents a point in memory from the OAM tileset.
// Each value should point towards the top-left corner of a given sprite.
// The location of each sprite (after loading) can be easily seen through something like Mesen.
const u16 playerSpriteTable[43] = {
    // Walking + Idle
	0,2,0,2,	// Down, Flip last frame
	4,6,4,6,	// Up, Flip last frame
	8,10,		// Side
	
	// Shooting
	12,14,64,66,68,70,			// Down
	72,74,76,78,128,
	130,132,134,136,138,140,	// Side
	142,192,194,196,198,
	200,202,204,206,256,258,	// Up
	260,262,264,266,268
};

const u16 soulSpriteTable[16] = {
	// Idle
	352,354,
	
	// Dissappear
	356,358,360,362,364,366,
	416,418,420,422,424,426,
	428,430
};

const u16 bulletSpriteTable[6] = {
	320,322,324,326,328,330
};

// Structs
enum PlayerStates {
    PS_DOWN = 0,
	PS_UP = 4,
	PS_SIDE = 8
};

enum SoulStates{
	SS_IDLE = 0,
	SS_DEAD = 2
};

#endif