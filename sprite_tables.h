#ifndef SPRITE_TABLES
#define SPRITE_TABLES

// Sprite Tables
// The way these tables work is that each value represents a point in memory from the OAM tileset.
// Each value should point towards the top-left corner of a given sprite.
// The location of each sprite (after loading) can be easily seen through something like Mesen.
const u8 playerSpriteTable[55] = {
    // Walking + Idle
	0x0,0x2,0x0,0x2,	// Down, Flip last frame
	0x4,0x6,0x4,0x6,	// Up, Flip last frame
	0x8,0xA,			// Side
	
	// Shooting
	0x40C0,0x40E0,0x4400,0x4420,0x4440,0x4460,	// Down
	0x4480,0x44A0,0x44C0,0x44E0,0x4800,
	0x4820,0x4840,0x4860,0x4880,0x48A0,0x48C0,	// Left
	0x48E0,0x4C00,0x4C20,0x4C40,0x4C60,
	0x4C80,0x4CA0,0x4CC0,0x4CE0,0x5000,0x5020,	// Side
	0x5040,0x5060,0x5080,0x50A0,0x50C0
};

// Structs
enum PlayerStates {
    PS_DOWN = 0,
	PS_UP = 4,
	PS_SIDE = 8
};

#endif