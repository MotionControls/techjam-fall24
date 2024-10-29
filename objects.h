#ifndef OBJECTS_H
#define OBJECTS_H
#include <snes/snestypes.h>
#include "fp_math.h"

// Memory Stuffs
#define MEM_BACKGROUNDS 0x6000
#define MEM_CONSOLE 0x8000
#define MEM_MAPS 0x0000
#define MEM_SPRITES 0x4000

// Player Stuffs
#define PLAYER_OAMID 0 // This is ideally the ONLY OAM that should be const.
#define PLAYER_HORT 16
#define PLAYER_VERT 32
#define PLAYER_PALETTE_BANK 0
#define PLAYER_SPRITES &sheepspr
#define PLAYER_SPRITES_SIZE (&sheepspr_end - &sheepspr)
#define PLAYER_PALETTE &sheeppal
#define PLAYER_PALETTE_SIZE (&sheeppal_end - &sheeppal)

// Bullet Stuffs
#define BULLET_MAX_BULLETS 3
#define BULLET_HORT 8
#define BULLET_VERT 8
#define BULLET_SPRITES &bulletspr
#define BULLET_SPRITES_SIZE (&bulletspr_end - &bulletspr)
#define BULLET_PALETTE_SIZE (&bulletbluepal_end - &bulletbluepal) // The bullet palette cannot be const due to the colors of the bullet changing.
                                                      // The size of the palette SHOULD remain constant.

// Sprite Stuffs
// https://github.com/alekmaul/pvsneslib/wiki/Sprites#sprite-sizes
#define SPR_SIZE_8x8 0 << 5
#define SPR_SIZE_16x16 3 << 5
#define SPR_SIZE_32x32 5 << 5
#define SPR_SIZE_16x32 6 << 5

typedef enum {
  OBJECT_PLAYER = 0,
  OBJECT_TARGET = 1,
  OBJECT_COLLIDER = 2,
  OBJECT_BULLET = 3,
  OBJECT_ANGLE = 4,
} e_objectID;

typedef struct {
  u8 scrX, scrY;
  sfx wX, wY;
  sfx dX, dY;
  ufx speed;

  s8 hitBoxOffsetX, hitBoxOffsetY;
  u8 hitBoxSizeX, hitBoxSizeY;

  u16 eBits, r, t, genTimer;
} s_physicsData;

typedef struct {
  u8 oamID;
  u8 visible;
  u8 hFlip;
  u8 vFlip;
  u8 *palette;
} s_spriteData;

typedef struct {
  u8 curFrame;   // The index of the current frame from the animation table.
  u8 sprState;   // The sprite state of the object. *Never* set to 255, refers to an inactive sprite
  u8 frameTimer; // The amount of time the current frame has lasted.
  u8 timePerFrame;
} s_animData;

typedef struct {
  s_physicsData pData;
  s_spriteData sData;
  s_animData aData;
  u8 objID;
  void (*update_ptr)(u16, struct s_objectData *, struct Level *);
  void (*draw_ptr)(struct s_objectData *);
} s_objectData;

// Level Stuffs
#define LEVEL_TILE_SIZE 16
#define LEVEL_MAX_OBJECTS 20

typedef struct {
  u8 numObj;                                       // Number of objects in the level
  u8 collision[LEVEL_TILE_SIZE * LEVEL_TILE_SIZE]; // Collision Array
  s_objectData objects[LEVEL_MAX_OBJECTS];         //[LEVEL_MAX_OBJECTS];         // The list of all objects in the level
} s_levelData;

typedef struct {
    u8 *tiles, *palette, *map;          // BG Data
    u8 tilesSize, paletteSize, mapSize; // BG Data Sizes
    u8 xSpawn, ySpawn;                  // Player's Spawn Position
    s_levelData *data;
} Level;

#endif