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

// Init globals.
Level levels[1];     // Level Array
u16 pad0, storePad0; // pad0 = Current Input
                     // storePad0 = Last Frame's Input
Level cur_level;

Level Level_Init(u8 id);
void Level_Tick(u16 pad0, Level *level);
void add_obj_to_lvl(s_objectData obj, u8 *obj_id, Level *level);

s_objectData Player_Init(u8 x, u8 y, ufx speed);
s_objectData Target_Init(u8 x, u8 y, ufx speed);

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
    //    s_objectData player = Player_Init(100, 100, CharToUFX(1, 0));
    //    s_objectData target = Target_Init(100, 100, CharToUFX(0, 0));
    cur_level = Level_Init(0);

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

        Level_Tick(pad0, &cur_level);

        WaitForVBlank();
    }
    return 0;
}

void Level_Tick(u16 pad0, Level *level) {
    (*level->data->objects[0].update_ptr)(pad0, &level->data->objects[0], level);
    (*level->data->objects[0].draw_ptr)(&level->data->objects[0]);

    (*level->data->objects[1].update_ptr)(pad0, &level->data->objects[1], level);
    (*level->data->objects[1].draw_ptr)(&level->data->objects[1]);
    // for (u8 i = 0; i < level->data->numObj; i++) {
    //     (*level->data->objects[i].update_ptr)(pad0, &level->data->objects[i]);
    // }
    // for (u8 i = 0; i < level->data->numObj; i++) {
    //     (*level->data->objects[i].draw_ptr)(&level->data->objects[i]);
    // }
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

    loaded_level.data = malloc(sizeof(s_levelData));
    add_obj_to_lvl(Player_Init(loaded_level.xSpawn, loaded_level.ySpawn, CharToUFX(1, 0)), &curObjID, &loaded_level);
    add_obj_to_lvl(Target_Init(200, 150, CharToUFX(0, 0)), &curObjID, &loaded_level);
    return loaded_level;
}

void add_obj_to_lvl(s_objectData obj, u8 *obj_id, Level *level) {
    level->data->objects[*obj_id] = obj;
    (*obj_id)++;
    level->data->numObj++;
}

// Functions
/*	Player Player_Init(x, y, speed);
	Returns a player struct.
x, y	;	Starting position.
speed	;	Starting speed.
*/
s_objectData Player_Init(u8 x, u8 y, ufx speed) {
    // Init player.
    s_objectData player = generic_init_obj(x, y, speed, PLAYER_OAMID, PLAYER_PALETTE, &player_tick, &player_draw);

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

    return player;
}


/*	Player Target_Init(x, y, speed);
	Returns a target struct.
x, y	;	Starting position.
speed	;	Starting speed.
*/
s_objectData Target_Init(u8 x, u8 y, ufx speed) {
    // Init target.
    s_objectData target = generic_init_obj(x, y, speed, 4, PLAYER_PALETTE, &target_tick, &target_draw);

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

    return target;
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