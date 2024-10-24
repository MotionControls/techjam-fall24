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

  u8 l, r, t, b;
} s_objectData;

// Any player related vars that aren't expected to change should be #defined.
typedef struct {

  s_objectData data;

  // Sprites
  u8 curFrame;     // The index of the current frame from playerSpriteTable.
  u8 sprState;     // The sprite state of the player. Values must be from PlayerStates.
  u8 frameTimer;   // The amount of time the current frame has lasted.
  u8 timePerFrame; // The max amount of time each frame can last.
  u8 visible;      // Whether or not sprite is visible.
  u8 hortFlip;     // Whether or not to flip the player sprite horizontally.
                   // I don't suspect we'll need vertFlip but it's easy to add if we do.
} Player;

typedef struct {
  s_objectData data;

  // Sprites
  u8 oamID;
  u8 visible;
  u8 hortFlip;
  u8 vertFlip;
  u8 *palette;
} Bullet;

// This may not be needed? If they share the same variables then I don't see a reason to not reuse Bullet as Target.
typedef struct {

  s_objectData data;

  // Sprites
  u8 oamID;
  u8 visible;
  u8 hortFlip;
  u8 vertFlip;
  u8 *palette;
} Target;

#endif