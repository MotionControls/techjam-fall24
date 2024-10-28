#ifndef OBJECT_FUNCS_H
#define OBJECT_FUNCS_H
#include "objects.h"
#include <snes.h>

// Player Stuffs
#define PLAYER_OAMID 0 // This is ideally the ONLY OAM that should be const.
#define PLAYER_HORT 16
#define PLAYER_VERT 32
#define PLAYER_PALETTE_BANK 0

enum COLLISION_RESULT_DIR {
    COLLISION_UP =    0b1000,
    COLLISION_DOWN =  0b0100,
    COLLISION_LEFT =  0b0010,
    COLLISION_RIGHT = 0b0001
};

s_objectData Target_Init(u8 x, u8 y, ufx speed, Level* lvl);
s_objectData Player_Init(u8 x, u8 y, ufx speed, Level* lvl);
s_objectData Bullet_Init(u8 x, u8 y, sfx xSpeed, sfx ySpeed, Level* lvl);
s_objectData Collider_Init(u8 x, u8 y, u8 sizeX, u8 sizeY, Level* lvl);

void add_obj_to_lvl(s_objectData obj, Level *level);
void clear_lvl_objs(Level *level);
u8 get_free_oamid(Level*);
u8 get_free_obj_slot(Level*);
void spawn_bullet(s_objectData* obj, Level* lvl);
void obj_kill(s_objectData* obj);

u16 CheckCollision_obj_obj(s_objectData *objA, s_objectData *objB);
u8 Collide_obj_colliders(s_objectData* obj, Level* lvl);
u8 Collide_obj_bullets(s_objectData* obj, Level* lvl);

s_objectData generic_init_obj(u8 id,
    u8 x, u8 y, 
    ufx speed, 
    u8 oamID, u8* palette, 
    void (*update_ptr)(u16, struct s_objectData *), 
    void (*draw_ptr)(struct s_objectData *));

void player_tick(u16, s_objectData *, Level*);
void player_draw(s_objectData *);
void bullet_tick(u16, s_objectData *, Level*);
void bullet_draw(s_objectData *);
void target_tick(u16, s_objectData *, Level*);
void target_draw(s_objectData *);
void generic_copy_data_to_sneslib_obj(s_objectData *, t_objs*);
void generic_draw(s_objectData *);
#endif