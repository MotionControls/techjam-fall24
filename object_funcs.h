#ifndef OBJECT_FUNCS_H
#define OBJECT_FUNCS_H
#include "objects.h"

enum COLLISION_RESULT_DIR {
    COLLISION_UP =    0b1000,
    COLLISION_DOWN =  0b0100,
    COLLISION_LEFT =  0b0010,
    COLLISION_RIGHT = 0b0001
};

u8 CheckCollision_obj_obj(s_objectData *objA, s_objectData *objB);
u8 CheckCollision_obj_level(s_objectData *objA, Level* level);

s_objectData generic_init_obj(
    u8 x, u8 y, 
    ufx speed, 
    u8 oamID, u8* palette, 
    void (*update_ptr)(u16, struct s_objectData *), 
    void (*draw_ptr)(struct s_objectData *));

void player_tick(u16, s_objectData *, Level*);
void player_draw(s_objectData *);
void target_tick(u16, s_objectData *, Level*);
void target_draw(s_objectData *);
void generic_copy_data_to_sneslib_obj(s_objectData *);
void generic_draw(s_objectData *);
#endif