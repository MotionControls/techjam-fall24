/*	TODO
- Better documentation.
- General cleanup.
- Fix lag.
- Fix collisions to better match level tiles.
- Functional shooting animations.
x Fix objs only drawing the sheep.
-	Fix Bullet using wrong sprite size.
- Fix random junk data appearing in code.
- Fix cloud background not showing.

So I think a problem with the current backend is that it's a tad over-engineered.
Including colliders, we only have 5 objects in the game.
As of right now it's a bit hard to understand what the current system is doing and the
longer I look at it the more I realize that it might be simpler just to have a struct per-object.
It would be less modular and kinda gross, but it would probably make the game run better and fix
the current sprite issues.

No I am a FOOL.
oamSet takes a gfx offset, NOT a memory address.
So all of the sprite tables are wrong.
The only reason I never noticed is because the only player sprites you see are the only sprites I set correctly.
Ouch.
*/

// https://github.com/alekmaul/pvsneslib/wiki

#include <snes.h>
#include "fp_math.h"
#include "object_funcs.h"
#include "level_defs.h"
#include "externs.h"

#define BG_MAP_SIZE (&level1map_end - &level1map) // Given each background is 256x256, they should all very conveniently have the same map size.

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
    // Init first level.
	cur_level = Level_Init(level_num);

    // Activate Mode1 and set tile size to 8x8.
    setMode(BG_MODE1, 0);

    // Game Loop.
    while (1) {
        // Poll input.
        storePad0 = pad0;
        pad0 = padsCurrent(0);

		Level_Tick(pad0, &cur_level);

        WaitForVBlank();
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

void handle_obj(s_objDef objDef, Level* lvl) {
    switch(objDef.id) {
            case OBJECT_PLAYER:
                add_obj_to_lvl(Player_Init(objDef.x, objDef.y, CharToUFX(1, 0), objDef.extra_bits, lvl), lvl);
                break;
            case OBJECT_TARGET:
                add_obj_to_lvl(Target_Init(objDef.x, objDef.y, CharToUFX(1, 0), objDef.extra_bits, lvl), lvl);
                break;
            case OBJECT_COLLIDER:
                add_obj_to_lvl(Collider_Init(objDef.x, objDef.y, objDef.sizeX, objDef.sizeY, objDef.extra_bits, lvl), lvl);
                break;
            case OBJECT_ANGLE:
                add_obj_to_lvl(Angle_Init(objDef.x, objDef.y, objDef.extra_bits, lvl), lvl);
                break;
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
        handle_obj(objDef, &loaded_level);
        ++i;
    }
	
	/*
	BG_Change(0, 
		levelDefs[id].bg, levelDefs[id].bgSize, 
		levelDefs[id].pal, levelDefs[id].palSize, 0, 
		MEM_BACKGROUNDS, 
		levelDefs[id].map, BG_MAP_SIZE, MEM_MAPS);
	*/
	
	// Some very gross hardcoding.
	// So I'd like to get the above to work but it's introducing tons of junk data.
	// I don't really wanna keep working on this bit so this'll stay.
	// It only effects level loading so I'm not as bothered by it.
	switch(id){
		case 0:
			BG_Change(0, &level1bg, (&level1bg_end - &level1bg), &level1pal, (&level1pal_end - &level1pal), 0, MEM_BACKGROUNDS, &level1map, BG_MAP_SIZE, MEM_MAPS);
		break;
		case 1:
			BG_Change(0, &level2bg, (&level2bg_end - &level2bg), &level2pal, (&level2pal_end - &level2pal), 0, MEM_BACKGROUNDS, &level2map, BG_MAP_SIZE, MEM_MAPS);
		break;
		case 2:
			BG_Change(0, &level3bg, (&level3bg_end - &level3bg), &level3pal, (&level3pal_end - &level3pal), 0, MEM_BACKGROUNDS, &level3map, BG_MAP_SIZE, MEM_MAPS);
		break;
		case 3:
			BG_Change(0, &level4bg, (&level4bg_end - &level4bg), &level4pal, (&level4pal_end - &level4pal), 0, MEM_BACKGROUNDS, &level4map, BG_MAP_SIZE, MEM_MAPS);
		break;
		case 4:
			BG_Change(0, &level5bg, (&level5bg_end - &level5bg), &level5pal, (&level5pal_end - &level5pal), 0, MEM_BACKGROUNDS, &level5map, BG_MAP_SIZE, MEM_MAPS);
		break;
		case 5:
			BG_Change(0, &level1bg, (&level1bg_end - &level1bg), &level1pal, (&level1pal_end - &level1pal), 0, MEM_BACKGROUNDS, &level1map, BG_MAP_SIZE, MEM_MAPS);
		break;
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
	bgSetDisable(2);	// This disables BG1. Important since it points to 0x0000 and that's where we store the MAP for the level.
	setScreenOn();		// Turns the screen on. Has to be done everytime a new BG is loaded.
}