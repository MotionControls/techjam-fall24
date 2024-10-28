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

// BG Stuffs
#define BG_MAP_SIZE (&iconsmap_end - &iconsmap) // Given each background is 256x256, they should all very conveniently have the same map size.

// Init globals.
Level levels[1];     // Level Array
u16 pad0, storePad0; // pad0 = Current Input
                     // storePad0 = Last Frame's Input
Level cur_level;

Level Level_Init(u8 id);
void Level_Tick(u16 pad0, Level *level);

void BG_Change(u8 index, u8 *tiles, u16 tilesSize, u8 *palette, u16 paletteSize, u8 paletteBank, u16 tileMem, u8 *map, u16 mapSize, u16 mapMem);
void TLD_Change(u8 index, u8 *tiles, u16 tilesSize, u8 *palette, u16 paletteSize, u8 paletteBank, u16 tileMem, u8* tldMap, u8* tldTiles, u8* tldProps, u8 mapTileSize);

u8 level_num = 0;

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
    cur_level = Level_Init(0);

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
    u8 num_targets = 0;
    while(i < LEVEL_MAX_OBJECTS) {
        if(level->data->objects[i].aData.sprState == 255) {
            ++i;
            continue;
        }
        if(level->data->objects[i].objID == OBJECT_TARGET) ++num_targets;
        if(level->data->objects[i].update_ptr) (*level->data->objects[i].update_ptr)(pad0, &level->data->objects[i], level);
        if(level->data->objects[i].draw_ptr) (*level->data->objects[i].draw_ptr)(&level->data->objects[i]);
        ++i;
    }
    if(num_targets == 0) {
        ++level_num;
        cur_level = Level_Init(level_num);
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