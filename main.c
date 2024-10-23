// https://github.com/alekmaul/pvsneslib/wiki

#include <snes.h>
#include "fp_math.h"

// Sprites
extern char johnspr, johnspr_end;
extern char johnpal, johnpal_end;

// BGs
extern char iconsbg, iconsbg_end;
extern char iconspal, iconspal_end;
extern char iconsmap, iconsmap_end;

// Screen Stuffs
#define RES_X			256
#define RES_Y			224

// Memory Stuffs
#define MEM_SPRITES			0x4000
#define MEM_BACKGROUNDS		0x6000
#define MEM_MAPS			0x0000

// Player Stuffs
#define PLAYER_OAMID		0	// This is ideally the ONLY OAM that should be const.
#define PLAYER_HORT			16
#define PLAYER_VERT			32
#define PLAYER_SPRITES		&johnspr
#define PLAYER_SPRITES_SIZE	(&johnspr_end - &johnspr)
#define PLAYER_PALETTE		&johnpal
#define PLAYER_PALETTE_SIZE	(&johnpal_end - &johnpal)
#define PLAYER_PALETTE_BANK	0

// Bullet Stuffs
#define BULLET_MAX_BULLETS	3
#define BULLET_HORT			8
#define BULLET_VERT			8
#define BULLET_SPRITES		&johnspr
#define BULLET_SPRITES_SIZE	(&johnspr_end - &johnspr)
#define BULLET_PALETTE_SIZE	(&johnpal_end - &johnpal)	// The bullet palette cannot be const due to the colors of the bullet changing.
														// The size of the palette SHOULD remain constant.

// Sprite Tables
// The way these tables work is that each value represents a point in memory from the OAM tileset.
// Each value should point towards the top-left corner of a given sprite.
// The location of each sprite (after loading) can be easily seen through something like Mesen.
const u8 playerSpriteTable[1] = {
	0	// Where the data??? Can't find the data??? Whyre is the dtaa???
};

// Structs
enum PlayerStates{	// TODO: Explain how these work in tandem with playerSpriteTable.
	PS_IDLE	= 0
};

// Any player related vars that aren't expected to change should be #defined.
typedef struct{
	// Position
	u8 rx, ry;	// The on-screen position.
	ufx x, y;	// The real position. Unsigned fixed number.
	sfx cx, cy;	// The change in position. Signed fixed number.
	ufx speed;	// How fast the player moves in any given direction. Unsigned fixed number.
	
	// Sprites
	u8 curFrame;		// The index of the current frame from playerSpriteTable.
	u8 sprState;		// The sprite state of the player. Values must be from PlayerStates
	u8 frameTimer;		// The amount of time the current frame has lasted.
	u8 timePerFrame;	// The max amount of time each frame can last.
	u8 visible;			// Whether or not sprite is visible.
	u8 hortFlip;		// Whether or not to flip the player sprite horizontally.
						// I don't suspect we'll need vertFlip but it's easy to add if we do.
}Player;

typedef struct{
	// Position
	// Same functions as in player.
	u8 rx, ry;
	ufx x, y;
	sfx cx, cy;
	ufx speed;
	
	// Sprites
	u8 oamID;
	u8 visible;
	u8 hortFlip;
	u8 vertFlip;
	u8* palette;
}Bullet;

// Init globals.
u16 pad0, storePad0;	// pad0 = Current Input
						// storePad0 = Last Frame's Input

// Function protos.
u8 CheckCollision(Player player, u8* arr, u8 xStart, u8 xRange, u8 yStart, u8 yRange);

Player Player_Init(u8 x, u8 y, ufx speed);
void Player_Step(Player *player);

void BG_Change(u8 index, u8* tiles, u16 tilesSize, u8* palette, u16 paletteSize, u8 paletteBank, u16 tileMem, u8* map, u16 mapSize, u16 mapMem);

int main(void){
	// Init SNES
	consoleInit();
	
	// Init player.
	// The OAM can only be used AFTER consoleInit so this should be done afterwards.
	Player player = Player_Init(100, 100, CharToUFX(1, 0));
	
	// Init BG stuffs.
	BG_Change(
		0,
		&iconsbg, (&iconsbg_end - &iconsbg),
		&iconspal, (&iconspal_end - &iconspal), 0,
		MEM_BACKGROUNDS,
		&iconsmap, (&iconsmap_end - &iconsmap),
		MEM_MAPS
	);
	
	// This needs to be done after ANY new backgrounds are loaded.
	// All accessible backgrounds are enabled by default.
	bgSetDisable(1);
	bgSetDisable(2);
	setScreenOn();
	
	// Activate Mode1 and set tile size to 8x8.
	setMode(BG_MODE1, 0);

	// Game Loop.
	while (1){
		// Poll input.
		storePad0 = pad0;
		pad0 = padsCurrent(0);
		
		// Step OAM objects.
		Player_Step(&player);
		
		WaitForVBlank();
	}
	return 0;
}

// Functions
/*	Player Player_Init(x, y, speed);
	Returns a player struct.
x, y	;	Starting position.
speed	;	Starting speed.
*/
Player Player_Init(u8 x, u8 y, ufx speed){
	// Init player.
	Player player = {
		x, y,
		CharToUFX(x, 0), CharToUFX(y, 0),
		0, 0,
		speed,
		0,0,0,0,
		1,
		0
	};
	
	// Init OAM object.
	oamInitGfxSet(
		PLAYER_SPRITES, PLAYER_SPRITES_SIZE,	// Sprites + Length
		PLAYER_PALETTE, PLAYER_PALETTE_SIZE,	// Palette + Length
		PLAYER_PALETTE_BANK,					// Palette Bank
		MEM_SPRITES,							// Where to put sprites.
		3 << 5									// Size of sprites.
												// https://github.com/alekmaul/pvsneslib/wiki/Sprites#sprite-sizes
	);
	oamSet(PLAYER_OAMID, player.rx, player.ry, 3, 0, 0, 0, 0);
    oamSetEx(PLAYER_OAMID, OBJ_SMALL, 1);
    oamSetVisible(PLAYER_OAMID, OBJ_SHOW);
	
	return player;
}

/*	void Player_Step();
	Does all player-related duties when called.
*/
void Player_Step(Player *player){
	// Physics.
	player->cx = player->cy = 0;
	if(pad0){	// If a button has been pressed.
		// Movement
		if(pad0 & KEY_LEFT)
			player->cx = -player->speed;
		if(pad0 & KEY_RIGHT)
			player->cx = player->speed;
		if(pad0 & KEY_UP)
			player->cy = -player->speed;
		if(pad0 & KEY_DOWN)
			player->cy = player->speed;
		
		// Shooting
		
	}
	
	player->x += player->cx;
	player->y += player->cy;
	
	player->rx = UFXToChar(player->x);
	player->ry = UFXToChar(player->y);
	
	// Update player OAM object.
	oamSet(PLAYER_OAMID, player->rx, player->ry, 3, player->hortFlip, 0, playerSpriteTable[player->curFrame + player->sprState], 0);
}

/*	void BG_Change(index, tiles, tilesSize, palette, paletteSize, paletteBank, tileMem, map, mapSize, mapMem);
	Changes the background at index.
index		;	Index of the background to change.
tiles		;	Memory pointer of the tiles.
tilesSize	;	Size of the tile memory.
palette		;	Memory pointer of the palette.
paletteSize	;	Size of the palette memory.
paletteBank	;	Which palette bank to store the palette in.
tileMem		;	Where to store the tiles in memory.
map			;	Memory pointer of the map.
mapSize		;	Size of the map memory.
mapMem		;	Where to store the map in memory.
*/
void BG_Change(u8 index, u8* tiles, u16 tilesSize, u8* palette, u16 paletteSize, u8 paletteBank, u16 tileMem, u8* map, u16 mapSize, u16 mapMem){
	bgInitTileSet(index, tiles, palette, paletteBank, tilesSize, paletteSize, BG_16COLORS, tileMem);
	bgInitMapSet(index, map, mapSize, SC_32x32, mapMem);
}

/*	TODO
	Returns 1 if collision, 0 otherwise.
*/
u8 CheckCollision(Player player, u8* arr, u8 xStart, u8 xRange, u8 yStart, u8 yRange){}