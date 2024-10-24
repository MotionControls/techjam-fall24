// https://github.com/alekmaul/pvsneslib/wiki

#include <snes.h>
#include "fp_math.h"
#include "object_funcs.h"

// Sprites
extern char johnspr, johnspr_end;
extern char johnpal, johnpal_end;

// BGs
extern char iconsbg, iconsbg_end;
extern char iconspal, iconspal_end;
extern char iconsmap, iconsmap_end;

extern char snesfont, snespal;

// Screen Stuffs
#define RES_X 256
#define RES_Y 224

// Memory Stuffs
#define MEM_SPRITES 0x4000
#define MEM_BACKGROUNDS 0x6000
#define MEM_CONSOLE 0x8000
#define MEM_MAPS 0x0000

// Sprite Stuffs
// https://github.com/alekmaul/pvsneslib/wiki/Sprites#sprite-sizes
#define SPR_SIZE_8x8 0 << 5
#define SPR_SIZE_16x16 3 << 5
#define SPR_SIZE_32x32 5 << 5
#define SPR_SIZE_16x32 6 << 5

// Player Stuffs
#define PLAYER_OAMID 0 // This is ideally the ONLY OAM that should be const.
#define PLAYER_HORT 16
#define PLAYER_VERT 32
#define PLAYER_SPRITES &johnspr
#define PLAYER_SPRITES_SIZE (&johnspr_end - &johnspr)
#define PLAYER_PALETTE &johnpal
#define PLAYER_PALETTE_SIZE (&johnpal_end - &johnpal)
#define PLAYER_PALETTE_BANK 0

// Bullet Stuffs
#define BULLET_MAX_BULLETS 3
#define BULLET_HORT 8
#define BULLET_VERT 8
#define BULLET_SPRITES &johnspr
#define BULLET_SPRITES_SIZE (&johnspr_end - &johnspr)
#define BULLET_PALETTE_SIZE (&johnpal_end - &johnpal) // The bullet palette cannot be const due to the colors of the bullet changing.
                                                      // The size of the palette SHOULD remain constant.

// BG Stuffs
#define BG_MAP_SIZE (&iconsmap_end - &iconsmap) // Given each background is 256x256, they should all very conveniently have the same map size.

// Level Stuffs
#define LEVEL_TILE_SIZE 16
#define LEVEL_MAX_TARGETS 8

// Sprite Tables
// The way these tables work is that each value represents a point in memory from the OAM tileset.
// Each value should point towards the top-left corner of a given sprite.
// The location of each sprite (after loading) can be easily seen through something like Mesen.
const u8 playerSpriteTable[2] = {
    0, // Where the data??? Can't find the data??? Whyre is the dtaa???
    1};

// Structs
enum PlayerStates { // TODO: Explain how these work in tandem with playerSpriteTable.
    PS_IDLE = 0
};

typedef struct {
    u8 *tiles, *palette, *map;                       // BG Data
    u16 tilesSize, paletteSize, mapSize;             // BG Data Sizes
    u8 xSpawn, ySpawn;                               // Player's Spawn Position
    u8 collision[LEVEL_TILE_SIZE * LEVEL_TILE_SIZE]; // Collision Array
    Target targets[LEVEL_MAX_TARGETS];               // Target Array
} Level;

// Init globals.
Level levels[1];     // Level Array
u16 pad0, storePad0; // pad0 = Current Input
                     // storePad0 = Last Frame's Input

// Function protos.
u8 CheckCollision(Player player, u8 *arr, u8 xStart, u8 xRange, u8 yStart, u8 yRange);

Player Player_Init(u8 x, u8 y, ufx speed);
void Player_Step(Player *player);

Target Target_Init(u8 x, u8 y, ufx speed);
void Target_Step(Target *target, Player *player);

void BG_Change(u8 index, u8 *tiles, u16 tilesSize, u8 *palette, u16 paletteSize, u8 paletteBank, u16 tileMem, u8 *map, u16 mapSize, u16 mapMem);

int main(void) {
    // Init SNES
    consoleInit();
    //    consoleSetTextVramBGAdr(MEM_CONSOLE);
    //    consoleSetTextVramAdr(0x3000);
    //    consoleSetTextOffset(0x0100);
    //    consoleInitText(0, 16 * 2, &snesfont, &snespal);

    // Init player.
    // The OAM can only be used AFTER consoleInit so this should be done afterwards.
    Player player = Player_Init(100, 100, CharToUFX(1, 0));
    Target target = Target_Init(100, 100, CharToUFX(0, 0));

    //Init BG stuffs.
    BG_Change(
        0,
        &iconsbg, (&iconsbg_end - &iconsbg),
        &iconspal, (&iconspal_end - &iconspal), 0,
        MEM_BACKGROUNDS,
        &iconsmap, BG_MAP_SIZE,
        MEM_MAPS);

    // This needs to be done after ANY new backgrounds are loaded.
    // All accessible backgrounds are enabled by default.
    bgSetDisable(1);
    bgSetDisable(2);

    setScreenOn();


    // Activate Mode1 and set tile size to 8x8.
    setMode(BG_MODE1, 0);


    // Game Loop.
    while (1) {
        // Poll input.
        storePad0 = pad0;
        pad0 = padsCurrent(0);

        // Step OAM objects.
        Player_Step(&player);
        Target_Step(&target, &player);

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
Player Player_Init(u8 x, u8 y, ufx speed) {
    // Init player.
    Player player = {
        {
            x, y,
            CharToSFX(x, 0), CharToSFX(y, 0),
            0, 0,
            speed,
            0, 8,
            16, 32,
        },
        0,
        0,
        0,
        0,
        1,
        0};

    // Init OAM object.
    oamInitGfxSet(
        PLAYER_SPRITES, PLAYER_SPRITES_SIZE, // Sprites + Length
        PLAYER_PALETTE, PLAYER_PALETTE_SIZE, // Palette + Length
        PLAYER_PALETTE_BANK,                 // Palette Bank
        MEM_SPRITES,                         // Where to put sprites.
        SPR_SIZE_16x32                       // Size of sprites.
        );

    oamSet(PLAYER_OAMID, player.data.scrX, player.data.scrY, 3, 0, 0, 0, 0);
    oamSetEx(PLAYER_OAMID, OBJ_SMALL, 1);
    oamSetVisible(PLAYER_OAMID, OBJ_SHOW);

    return player;
}


/*	Player Target_Init(x, y, speed);
	Returns a target struct.
x, y	;	Starting position.
speed	;	Starting speed.
*/
Target Target_Init(u8 x, u8 y, ufx speed) {
    // Init player.
    Target target = {
        {
            x, y,
            CharToSFX(x, 0), CharToSFX(y, 0),
            0, 0,
            speed,
            0, 8,
            16, 32,
        },
        4,
        1,
        0,
        0,
        NULL};

    // Init OAM object.
    oamInitGfxSet(
        PLAYER_SPRITES, PLAYER_SPRITES_SIZE, // Sprites + Length
        PLAYER_PALETTE, PLAYER_PALETTE_SIZE, // Palette + Length
        PLAYER_PALETTE_BANK,                 // Palette Bank
        MEM_SPRITES,                         // Where to put sprites.
        SPR_SIZE_16x32                       // Size of sprites.
        );

    oamSet(target.oamID, target.data.scrX, target.data.scrY, 3, 0, 0, 0, 0);
    oamSetEx(target.oamID, OBJ_SMALL, 1);
    oamSetVisible(target.oamID, OBJ_SHOW);

    return target;
}

/*	void Player_Step();
	Does all player-related duties when called.
*/
void Player_Step(Player *player) {
    // Physics.
    player->data.dX = player->data.dY = 0;
    if (pad0) { // If a button has been pressed.
        // Movement
        if (pad0 & KEY_LEFT)
            player->data.dX = -player->data.speed;
        if (pad0 & KEY_RIGHT)
            player->data.dX = player->data.speed;
        if (pad0 & KEY_UP)
            player->data.dY = -player->data.speed;
        if (pad0 & KEY_DOWN)
            player->data.dY = player->data.speed;

        // Shooting
        // BIG BIG TODO
    }

    player->data.wX += player->data.dX;
    player->data.wY += player->data.dY;

    player->data.scrX = SFXToChar(player->data.wX);
    player->data.scrY = SFXToChar(player->data.wY);

    // Update player OAM object.
    oamSet(PLAYER_OAMID, player->data.scrX, player->data.scrY, 3, player->hortFlip, 0, playerSpriteTable[player->curFrame + player->sprState], 0);
}

void Target_Step(Target *target, Player *player) {
    // Physics.
    target->data.dX = target->data.dY = 100;
    //	if(pad0){	// If a button has been pressed.
    //		// Movement
    //		if(pad0 & KEY_LEFT)
    //			player->data.dX = -player->data.speed;
    //		if(pad0 & KEY_RIGHT)
    //			player->data.dX = player->data.speed;
    //		if(pad0 & KEY_UP)
    //			player->data.dY = -player->data.speed;
    //		if(pad0 & KEY_DOWN)
    //			player->data.dY = player->data.speed;
    //
    //		// Shooting
    //		// BIG BIG TODO
    //	}

    target->data.wX += target->data.dX;
    target->data.wY += target->data.dY;

    target->data.scrX = SFXToChar(target->data.wX);
    target->data.scrY = SFXToChar(target->data.wY);

    target->visible = CheckCollision_obj_obj(&target->data, &player->data);

    //    consoleDrawText(10, 10, "%u", target->visible);

    oamSetVisible(target->oamID, target->visible ? OBJ_SHOW : OBJ_HIDE);

    // Update player OAM object.
    oamSet(target->oamID, target->data.scrX, target->data.scrY, 3, target->hortFlip, 0, playerSpriteTable[target->visible], 0);
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
void BG_Change(u8 index, u8 *tiles, u16 tilesSize, u8 *palette, u16 paletteSize, u8 paletteBank, u16 tileMem, u8 *map, u16 mapSize, u16 mapMem) {
    bgInitTileSet(index, tiles, palette, paletteBank, tilesSize, paletteSize, BG_16COLORS, tileMem);
    bgInitMapSet(index, map, mapSize, SC_32x32, mapMem);
}

/*	TODO
	Returns 1 if collision, 0 otherwise.
player			;	...
arr				;	1D level array.
xStart, yStart	;	What part of the array to start the sweep at.
xRange, yRange	;	How much of the array to sweep.
*/
u8 CheckCollision(Player player, u8 *arr, u8 xStart, u8 xRange, u8 yStart, u8 yRange) {}