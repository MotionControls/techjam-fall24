#ifndef OBJECTS_H
#define OBJECTS_H
#include <snes/snestypes.h>
#include "fp_math.h"

typedef struct {
  u8 scrX, scrY;
  sfx wX, wY;
  sfx dX, dY;
  ufx speed;

  s8 hitBoxOffsetX, hitBoxOffsetY;
  u8 hitBoxSizeX, hitBoxSizeY;

  u8 l, r, t, b; // debugging vars, redundant but helpful
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
  u8 sprState;   // The sprite state of the object.
  u8 frameTimer; // The amount of time the current frame has lasted.
  u8 timePerFrame;
} s_animData;

typedef struct {
  s_physicsData pData;
  s_spriteData sData;
  s_animData aData;
  void (*update_ptr)(u16, struct s_objectData *, struct Level *);
  void (*draw_ptr)(struct s_objectData *);
} s_objectData;

// Level Stuffs
#define LEVEL_TILE_SIZE 16
#define LEVEL_MAX_OBJECTS 16

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