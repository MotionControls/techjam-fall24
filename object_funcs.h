#ifndef OBJECT_FUNCS_H
#define OBJECT_FUNCS_H
#include "objects.h"

u8 CheckCollision_obj_obj(s_objectData *objA, s_objectData *objB);

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
void generic_draw(s_objectData *);
#endif