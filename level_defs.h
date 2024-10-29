#ifndef LEVEL_DEFS_H
#define LEVEL_DEFS_H
#include <snes.h>
#include "objects.h"

typedef struct {
    u8 id;
    u8 x, y;
    u8 sizeX, sizeY;
    u8 extra_bits;
} s_objDef;

typedef struct {
    s_objDef objects[LEVEL_MAX_OBJECTS];
} s_levelDef;

s_levelDef levelDefs[5] = {
    {
        {
            {
                OBJECT_PLAYER,
                96, 96,
                0, 0,
                0
            },
            {
                OBJECT_TARGET,
                128, 96,
                0, 0,
                0
            },
            {
                OBJECT_COLLIDER,
                80, 64, 
                80, 16,
                0
            },
            {
                OBJECT_COLLIDER,
                80, 80, 
                16, 48,
                0
            },
            {
                OBJECT_COLLIDER,
                80, 128,
                80, 16,
                0
            },
            {
                OBJECT_COLLIDER,
                144, 80, 
                16, 48,
                0
            },
            {-1}
        }
    },
    {
        {
            {
                OBJECT_PLAYER,
                64, 48,
                0, 0,
                0
            },
            {
                OBJECT_TARGET,
                176, 48,
                0, 0,
                0
            },
            {
                OBJECT_TARGET,
                176, 160,
                0, 0,
                0
            },
            {
                OBJECT_TARGET,
                64, 160,
                0, 0,
                0
            },
            {
                OBJECT_COLLIDER,
                48, 32, 
                64, 16,
                0
            },
            {
                OBJECT_COLLIDER,
                96, 48, 
                64, 16,
                0
            },
            {
                OBJECT_COLLIDER,
                144, 32, 
                64, 16,
                0
            },
            {
                OBJECT_COLLIDER,
                48, 48, 
                16, 144,
                0
            },
            {
                OBJECT_COLLIDER,
                192, 48, 
                16, 144,
                0
            },
            {
                OBJECT_COLLIDER,
                144, 192, 
                64, 16,
                0
            },
            {
                OBJECT_COLLIDER,
                96, 176, 
                64, 16,
                0
            },
            {
                OBJECT_COLLIDER,
                48, 192, 
                64, 16,
                0
            },
            {
                OBJECT_COLLIDER,
                96, 128, 
                64, 16,
                0
            },
            {
                OBJECT_COLLIDER,
                96, 96, 
                64, 16,
                0
            },
            {-1}
        }
    },
    {
        {
            {
                OBJECT_PLAYER,
                80, 112,
                0, 0,
                0
            },
            {
                OBJECT_TARGET,
                160, 112,
                0, 0,
                0
            },
            {
                OBJECT_COLLIDER,
                64, 64, 
                16, 112,
                0
            },
            {
                OBJECT_COLLIDER,
                80, 64, 
                112, 16,
                0
            },
            {
                OBJECT_COLLIDER,
                176, 80, 
                16, 96,
                0
            },
            {
                OBJECT_COLLIDER,
                80, 160, 
                96, 16,
                0
            },
            {
                OBJECT_COLLIDER,
                112, 80, 
                32, 80,
                1
            },
            {-1}
        }
    },
    {
        {
            {
                OBJECT_PLAYER,
                3*16, 6*16,
                0, 0,
                0
            },
            {
                OBJECT_TARGET,
                12*16, 6*16,
                0, 0,
                0
            },
            {
                OBJECT_COLLIDER,
                2*16, 4*16, 
                16, 4*16,
                0
            },
            {
                OBJECT_COLLIDER,
                3*16, 4*16, 
                64, 16,
                0
            },
            {
                OBJECT_COLLIDER,
                6*16, 5*16, 
                1*16, 1*16,
                0
            },
            {
                OBJECT_COLLIDER,
                6*16, 6*16, 
                1*16, 1*16,
                1
            },
            {
                OBJECT_COLLIDER,
                6*16, 7*16, 
                1*16, 1*16,
                0
            },
            {
                OBJECT_COLLIDER,
                3*16, 8*16,
                4*16, 1*16,
                0
            },
            {
                OBJECT_COLLIDER,
                9*16, 5*16, 
                1*16, 1*16,
                0
            },
            {
                OBJECT_COLLIDER,
                9*16, 6*16, 
                1*16, 1*16,
                1
            },
            {
                OBJECT_COLLIDER,
                9*16, 7*16, 
                1*16, 1*16,
                0
            },
            {
                OBJECT_COLLIDER,
                9*16, 8*16, 
                5*16, 1*16,
                0
            },
            {
                OBJECT_COLLIDER,
                9*16, 4*16, 
                5*16, 1*16,
                0
            },
            {
                OBJECT_COLLIDER,
                13*16, 5*16, 
                1*16, 3*16,
                0
            },
            {-1}
        }
    },
    {
        {
            {
                OBJECT_PLAYER,
                3*16, 4*16,
                0, 0,
                0
            },
            {
                OBJECT_TARGET,
                12*16, 9*16,
                0, 0,
                0
            },
            {
                OBJECT_COLLIDER,
                2*16, 2*16, 
                5*16, 1*16,
                0
            },
            {
                OBJECT_COLLIDER,
                2*16, 3*16, 
                1*16, 4*16,
                0
            },
            {
                OBJECT_COLLIDER,
                3*16, 6*16, 
                5*16, 1*16,
                0
            },
            {
                OBJECT_COLLIDER,
                7*16, 3*16, 
                1*16, 1*16,
                1
            },
            {
                OBJECT_COLLIDER,
                7*16, 5*16, 
                1*16, 1*16,
                0
            },
            {
                OBJECT_ANGLE,
                13*16, 4*16,
                1*16, 1*16,
                0
            },
            {-1}
        }
    }
};

#endif