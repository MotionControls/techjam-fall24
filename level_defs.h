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
                100, 100,
                0, 0
            },
            {
                OBJECT_TARGET,
                150, 200,
                0, 0
            },
            {
                OBJECT_COLLIDER,
                240, 0, 
                16, 240
            },
            {
                OBJECT_COLLIDER,
                0, 0, 
                240, 16
            },
            {
                OBJECT_COLLIDER,
                16, 207, 
                224, 16
            },
            {
                OBJECT_COLLIDER,
                0, 16, 
                16, 224
            },
            {
                OBJECT_COLLIDER,
                100, 100, 
                32, 32
            },
            {-1}
        }
    }
};

#endif