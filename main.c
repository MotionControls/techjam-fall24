// https://github.com/alekmaul/pvsneslib/wiki

#include <snes.h>
#include "fp_math.h"
#include "object_funcs.h"
#include "level_defs.h"

// Sprites
extern char johnspr, sheepspr_end;
extern char johnpal, johnpal_end;

extern char sheepspr, sheepspr_end;
extern char sheeppal, sheeppal_end;

extern char bulletspr, bulletspr_end;
extern char bulletbluepal, bulletbluepal_end;

// BGs
extern char iconsbg, iconsbg_end;
extern char iconspal, iconspal_end;
extern char iconsmap, iconsmap_end;

// TILED
extern char tiled_testbg, tiled_testbg_end;
extern char tiled_testpal, tiled_testpal_end;
extern char tiled_testmap, tiled_testtiles, tiled_testprops;

// Font
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
#define PLAYER_SPRITES &sheepspr
#define PLAYER_SPRITES_SIZE (&sheepspr_end - &sheepspr)
#define PLAYER_PALETTE &sheeppal
#define PLAYER_PALETTE_SIZE (&sheeppal_end - &sheeppal)
#define PLAYER_PALETTE_BANK 0

// Bullet Stuffs
#define BULLET_MAX_BULLETS 3
#define BULLET_HORT 8
#define BULLET_VERT 8
#define BULLET_SPRITES &sheepspr
#define BULLET_SPRITES_SIZE (&sheepspr_end - &sheepspr)
#define BULLET_PALETTE_SIZE (&sheeppal_end - &sheeppal) // The bullet palette cannot be const due to the colors of the bullet changing.
                                                      // The size of the palette SHOULD remain constant.

// BG Stuffs
#define BG_MAP_SIZE (&iconsmap_end - &iconsmap) // Given each background is 256x256, they should all very conveniently have the same map size.

// Init globals.
Level levels[1];     // Level Array
u16 pad0, storePad0; // pad0 = Current Input
                     // storePad0 = Last Frame's Input
Level cur_level;

Level Level_Init(u8 id);
void Level_Tick(u16 pad0, Level *level);

s_objectData Player_Init(u8 x, u8 y, ufx speed, Level* lvl);
s_objectData Target_Init(u8 x, u8 y, ufx speed, Level* lvl);
s_objectData Collider_Init(u8 x, u8 y, u8 sizeX, u8 sizeY, Level* lvl);

void BG_Change(u8 index, u8 *tiles, u16 tilesSize, u8 *palette, u16 paletteSize, u8 paletteBank, u16 tileMem, u8 *map, u16 mapSize, u16 mapMem);
void TLD_Change(u8 index, u8 *tiles, u16 tilesSize, u8 *palette, u16 paletteSize, u8 paletteBank, u16 tileMem, u8* tldMap, u8* tldTiles, u8* tldProps, u8 mapTileSize);

int main(void) {
    // Init SNES
    // consoleInit();
    //    consoleSetTextVramBGAdr(MEM_CONSOLE);
    //    consoleSetTextVramAdr(0x3000);
    //    consoleSetTextOffset(0x0100);
    //    consoleInitText(0, 16 * 2, &snesfont, &snespal);

    // Init player.
    // The OAM can only be used AFTER consoleInit so this should be done afterwards.
    //    s_objectData player = Player_Init(100, 100, CharToUFX(1, 0));
    //    s_objectData target = Target_Init(100, 100, CharToUFX(0, 0));
    cur_level = Level_Init(1);

    //Init BG stuffs.
	/*
    BG_Change(
        0,
        &iconsbg, (&iconsbg_end - &iconsbg),
        &iconspal, (&iconspal_end - &iconspal), 0,
        MEM_BACKGROUNDS,
        &iconsmap, BG_MAP_SIZE,
        MEM_MAPS);
	*/
	TLD_Change(
		0,
		&tiled_testbg, (&tiled_testbg_end - &tiled_testbg),
		&tiled_testpal, (&tiled_testpal_end - &tiled_testpal), 0,
		0x0000,
		(u8*)&tiled_testmap,
		(u8*)&tiled_testtiles,
		(u8*)&tiled_testprops,
		SC_32x32);

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

        // mapUpdate();
        // mapUpdateCamera(0, 0);

        Level_Tick(pad0, &cur_level);

		// For whatever reason with the map engine this needs to be done every frame.
		bgSetDisable(1);
		bgSetDisable(2);

        WaitForVBlank();
        mapVblank();
    }
    return 0;
}

void Level_Tick(u16 pad0, Level *level) {
    u8 i = 0;
    while(i < LEVEL_MAX_OBJECTS) {
        if(level->data->objects[i].aData.sprState == 255) {
            ++i;
            continue;
        }
        if(level->data->objects[i].update_ptr) (*level->data->objects[i].update_ptr)(pad0, &level->data->objects[i], level);
        if(level->data->objects[i].draw_ptr) (*level->data->objects[i].draw_ptr)(&level->data->objects[i]);
        ++i;
    }
}

Level Level_Init(u8 id) {
    u8 curObjID = 0;
    Level loaded_level = {
        0,
        0,
        0,
        0,
        0,
        0,
        100,
        100
    };
    clear_lvl_objs(&loaded_level);

    u8 i = 0;
    while(i < LEVEL_MAX_OBJECTS) {
        s_objDef objDef = levelDefs[id].objects[i];
        if(objDef.id  == (u8)-1) break;
        switch(objDef.id) {
            case OBJECT_PLAYER:
                add_obj_to_lvl(Player_Init(objDef.x, objDef.y, CharToUFX(1, 0), &loaded_level), &loaded_level);
                break;
            case OBJECT_TARGET:
                add_obj_to_lvl(Target_Init(objDef.x, objDef.y, CharToUFX(1, 0), &loaded_level), &loaded_level);
                break;
            case OBJECT_COLLIDER:
                add_obj_to_lvl(Collider_Init(objDef.x, objDef.y, objDef.sizeX, objDef.sizeY, &loaded_level), &loaded_level);
                break;
        }
        ++i;
    }
    return loaded_level;
}

// Functions
/*	Player Player_Init(x, y, speed);
	Returns a player struct.
x, y	;	Starting position.
speed	;	Starting speed.
*/
s_objectData Player_Init(u8 x, u8 y, ufx speed, Level* lvl) {
    // Init player.
    s_objectData player = generic_init_obj(OBJECT_PLAYER, x, y, speed, get_free_oamid(lvl), PLAYER_PALETTE, &player_tick, &player_draw);

    // Init OAM object.
    oamInitGfxSet(
        PLAYER_SPRITES, PLAYER_SPRITES_SIZE,       // Sprites + Length
        player.sData.palette, PLAYER_PALETTE_SIZE, // Palette + Length
        PLAYER_PALETTE_BANK,                       // Palette Bank
        MEM_SPRITES,                               // Where to put sprites.
        SPR_SIZE_16x32                             // Size of sprites.
    );

    oamSet(player.sData.oamID, player.pData.scrX, player.pData.scrY, 3, 0, 0, 0, 0);
    oamSetEx(player.sData.oamID, OBJ_SMALL, 1);
    oamSetVisible(player.sData.oamID, OBJ_SHOW);
	
	// This should probably be modular but for now we'll hard code it.
	player.aData.frameTimer = 0;
	player.aData.curFrame = 0;
	player.aData.sprState = 0;
	player.aData.timePerFrame = 30;
	
	// Init Bullet Sprites.
	oamInitGfxSet(
        &bulletspr, (&bulletspr_end - &bulletspr),
        &bulletbluepal, (&bulletbluepal_end - &bulletbluepal),
        PLAYER_PALETTE_BANK+1,
        0x5C00,
        SPR_SIZE_16x16
    );

    return player;
}


/*	Player Target_Init(x, y, speed);
	Returns a target struct.
x, y	;	Starting position.
speed	;	Starting speed.
*/
s_objectData Target_Init(u8 x, u8 y, ufx speed, Level* lvl) {
    // Init target.
    s_objectData target = generic_init_obj(OBJECT_TARGET, x, y, speed, get_free_oamid(lvl), PLAYER_PALETTE, &target_tick, &target_draw);

    // Init OAM object.
    oamInitGfxSet(
        PLAYER_SPRITES, PLAYER_SPRITES_SIZE,       // Sprites + Length
        target.sData.palette, PLAYER_PALETTE_SIZE, // Palette + Length
        PLAYER_PALETTE_BANK,                       // Palette Bank
        MEM_SPRITES,                               // Where to put sprites.
        SPR_SIZE_16x32                             // Size of sprites.
    );

    oamSet(target.sData.oamID, target.pData.scrX, target.pData.scrY, 3, 0, 0, 0, 0);
    oamSetEx(target.sData.oamID, OBJ_SMALL, 1);
    oamSetVisible(target.sData.oamID, OBJ_SHOW);

    target.aData.sprState = 0;

    return target;
}

/*	Player Target_Init(x, y, speed);
	Returns a collider object.
x, y	        ;	Starting position.
sizeX, sizeY	;	Size.
*/
s_objectData Collider_Init(u8 x, u8 y, u8 sizeX, u8 sizeY, Level* lvl) {
    // Init collider.
    s_objectData collider = generic_init_obj(OBJECT_COLLIDER, x, y, 0, 0, 0, NULL, NULL);

    collider.pData.hitBoxSizeX = sizeX;
    collider.pData.hitBoxSizeY = sizeY;
    collider.aData.sprState = 0;

    return collider;
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

/*	void TLD_Change(index, tiles, tilesSize, palette, paletteSize, paletteBank, tileMem, tldMap, tldTiles, tildProps, mapTileSize);
	Changes the background at index, using the PV's MAP engine.
...			;	Shared with BG_Change().
tldMap		;	TILED map data.
tldTiles	;	TILED tile data.
tldProps	;	TILED tile property data.
mapTileSize	;	Size of the map in 8x8 tiles.
*/
void TLD_Change(u8 index, u8 *tiles, u16 tilesSize, u8 *palette, u16 paletteSize, u8 paletteBank, u16 tileMem, u8* tldMap, u8* tldTiles, u8* tldProps, u8 mapTileSize){
	bgInitTileSet(index, tiles, palette, paletteBank, tilesSize, paletteSize, BG_16COLORS, tileMem);
    bgSetMapPtr(index, 0x6800, mapTileSize);	// The MAP is required by the map engine to be at location 0x6800.
	mapLoad(tldMap, tldTiles, tldProps);
}