#ifndef LEVEL_DEFS_H
#define LEVEL_DEFS_H
#include <snes.h>
#include "objects.h"

typedef struct {
    u8 id;
    u8 x, y;
    u8 sizeX, sizeY;
} s_objDef;

typedef struct {
    s_objDef objects[LEVEL_MAX_OBJECTS];
} s_levelDef;

s_levelDef levelDefs[1] = {
    {
        {
            {
                OBJECT_PLAYER,
                96, 96,
                0, 0
            },
            {
                OBJECT_TARGET,
                128, 96,
                0, 0
            },
            {
                OBJECT_COLLIDER,
                80, 64, 
                80, 16
            },
            {
                OBJECT_COLLIDER,
                80, 80, 
                16, 48
            },
            {
                OBJECT_COLLIDER,
                80, 128,
                80, 16
            },
            {
                OBJECT_COLLIDER,
                144, 80, 
                16, 48
            },
            {-1}
        }
    }
};

#endif