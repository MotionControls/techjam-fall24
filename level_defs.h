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

s_levelDef levelDefs[3] = {
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
    },
    {
        {
            {
                OBJECT_PLAYER,
                64, 48,
                0, 0
            },
            {
                OBJECT_TARGET,
                176, 48,
                0, 0
            },
            {
                OBJECT_TARGET,
                176, 160,
                0, 0
            },
            {
                OBJECT_TARGET,
                64, 160,
                0, 0
            },
            {
                OBJECT_COLLIDER,
                48, 32, 
                64, 16
            },
            {
                OBJECT_COLLIDER,
                96, 48, 
                64, 16
            },
            {
                OBJECT_COLLIDER,
                144, 32, 
                64, 16
            },
            {
                OBJECT_COLLIDER,
                48, 48, 
                16, 144
            },
            {
                OBJECT_COLLIDER,
                192, 48, 
                16, 144
            },
            {
                OBJECT_COLLIDER,
                144, 192, 
                64, 16
            },
            {
                OBJECT_COLLIDER,
                96, 176, 
                64, 16
            },
            {
                OBJECT_COLLIDER,
                48, 192, 
                64, 16
            },
            {
                OBJECT_COLLIDER,
                96, 128, 
                64, 16
            },
            {
                OBJECT_COLLIDER,
                96, 96, 
                64, 16
            },
            {-1}
        }
    },
    {
        {
            {
                OBJECT_PLAYER,
                64, 48,
                0, 0
            },
            {
                OBJECT_TARGET,
                176, 48,
                0, 0
            },
            {
                OBJECT_TARGET,
                176, 160,
                0, 0
            },
            {
                OBJECT_TARGET,
                64, 160,
                0, 0
            },
            {
                OBJECT_COLLIDER,
                48, 32, 
                64, 16
            },
            {
                OBJECT_COLLIDER,
                96, 48, 
                64, 16
            },
            {
                OBJECT_COLLIDER,
                144, 32, 
                64, 16
            },
            {
                OBJECT_COLLIDER,
                48, 48, 
                16, 144
            },
            {
                OBJECT_COLLIDER,
                192, 48, 
                16, 144
            },
            {
                OBJECT_COLLIDER,
                144, 192, 
                64, 16
            },
            {
                OBJECT_COLLIDER,
                96, 176, 
                64, 16
            },
            {
                OBJECT_COLLIDER,
                48, 192, 
                64, 16
            },
            {
                OBJECT_COLLIDER,
                96, 128, 
                64, 16
            },
            {
                OBJECT_COLLIDER,
                96, 96, 
                64, 16
            },
            {-1}
        }
    }
};

#endif