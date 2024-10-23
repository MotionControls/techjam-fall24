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
#define PLAYER_OAM_ID		0
#define PLAYER_HORT			16
#define PLAYER_VERT			32

// Bullet Stuffs
#define BULLET_MAX_BULLETS	3
#define BULLET_HORT			8
#define BULLET_VERT			8

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
	u8 visible;
	u8 hortFlip;
	u8 vertFlip;
}Bullet;

// Init globals.
u16 pad0, storePad0;	// pad0 = Current Input
						// storePad0 = Last Frame's Input

// Function protos.
u8 CheckCollision(Player player, u8* arr, u8 xStart, u8 xRange, u8 yStart, u8 yRange);
void Player_Step(Player *player);

int main(void){
	// Init player.
	Player player = {
		0, 0,
		CharToUFX(100, 0), CharToUFX(100, 0),
		0, 0,
		CharToUFX(1, 0),
		
		0, 0, 0, 0,
		1,
		0
	};
	
	// Init SNES
	consoleInit();
	
	// Init OAM stuffs.
	// The OAM ID should be in increments of 4.
	// ex. Player = 0, Enemy = 4
	oamInitGfxSet(
		&johnspr, (&johnspr_end - &johnspr),	// Sprites + Length
		&johnpal, (&johnpal_end - &johnpal),	// Palette + Length
		0,										// Palette Bank
		MEM_SPRITES,							// Where to put sprites.
		3 << 5									// Size of sprites.
												// https://github.com/alekmaul/pvsneslib/wiki/Sprites#sprite-sizes
	);
	oamSet(0, 100, 100, 3, 0, 0, 0, 0);
    oamSetEx(0, OBJ_SMALL, 1);
    oamSetVisible(0, OBJ_SHOW);
	
	// Init BG stuffs.
	bgInitTileSet(
		0,
		&iconsbg, &iconspal, 0,
		(&iconsbg_end - &iconsbg), (&iconspal_end - &iconspal),
		BG_16COLORS,
		MEM_BACKGROUNDS
	);
	
	bgInitMapSet(
		0,
		&iconsmap, (&iconsmap_end - &iconsmap),
		SC_32x32,
		MEM_MAPS
	);
	
	// This needs to be done after ANY new backgrounds are loaded.
	// All accessible backgrounds are enabled by default.
	bgSetDisable(1);
	bgSetDisable(2);
	setScreenOn();
	
	// Activate Mode1 and set tile size to 8x8.
	setMode(BG_MODE1, 0);

	// Logo Loop.
	// Show logos for ~3 seconds.
	// Fade to black in ~1 second.
	/*unsigned int frameCounter = 0;
	while(frameCounter < 60 * 4){
		// TODO
		frameCounter++;
		WaitForVBlank();
	}
	
	// Main Menu Loop.
	// Show main menu until pressed start.
	// Fade to black in ~1 second.
	u8 showMainMenu = 1;
	while(showMainMenu){
		// TODO
		showMainMenu = 1;
		WaitForVBlank();
	}*/
	
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
/*	void Player_Step()
	Does all player-related duties when called.
*/
void Player_Step(Player *player){
	// Physics.
	player->cx = player->cy = 0;
	if(pad0){	// If a button has been pressed.
		if(pad0 & KEY_LEFT)
			player->cx = -player->speed;
		if(pad0 & KEY_RIGHT)
			player->cx = player->speed;
		if(pad0 & KEY_UP)
			player->cy = -player->speed;
		if(pad0 & KEY_DOWN)
			player->cy = player->speed;
	}
	
	player->x += player->cx;
	player->y += player->cy;
	
	player->rx = UFXToChar(player->x);
	player->ry = UFXToChar(player->y);
	
	// Update player OAM object.
	oamSet(player->oamID, player->rx, player->ry, 3, player->hortFlip, 0, playerSpriteTable[player->curFrame + player->sprState], 0);
}

/*	TODO
	Returns 1 if collision, 0 otherwise.
*/
u8 CheckCollision(Player player, u8* arr, u8 xStart, u8 xRange, u8 yStart, u8 yRange){}