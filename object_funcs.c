#include "object_funcs.h"
#include "sprites.h"

extern char sheepspr, sheepspr_end;
extern char sheeppal, sheeppal_end;

extern char bulletspr, bulletspr_end;
extern char bulletbluepal, bulletbluepal_end;

extern char soulspr, soulspr_end;
extern char soulbluepal, soulbluepal_end;

/*	Player Target_Init(x, y, speed);
	Returns a target struct.
x, y	;	Starting position.
speed	;	Starting speed.
*/
s_objectData Target_Init(u8 x, u8 y, ufx speed, u8 eBits, Level* lvl) {
    // Init target.
    s_objectData target = generic_init_obj(OBJECT_TARGET, x, y, speed, get_free_oamid(lvl), PLAYER_PALETTE, &target_tick, &target_draw);

    target.pData.eBits = eBits;

    // Init OAM object.
    oamInitGfxSet(
        &soulspr, (&soulspr_end - &soulspr),
        &soulbluepal, (&soulbluepal_end - &soulbluepal),
        PLAYER_PALETTE_BANK+2,
        0x5600,
        SPR_SIZE_16x32
    );

    oamSet(target.sData.oamID, target.pData.scrX, target.pData.scrY, 3, 0, 0, 0, 0);
    oamSetEx(target.sData.oamID, OBJ_SMALL, 1);
    oamSetVisible(target.sData.oamID, OBJ_SHOW);

    target.aData.sprState = 0;

    return target;
}


// Functions
/*	Player Player_Init(x, y, speed);
	Returns a player struct.
x, y	;	Starting position.
speed	;	Starting speed.
*/
s_objectData Player_Init(u8 x, u8 y, ufx speed, u8 eBits, Level* lvl) {
    // Init player.
    s_objectData player = generic_init_obj(OBJECT_PLAYER, x, y, speed, get_free_oamid(lvl), PLAYER_PALETTE, &player_tick, &player_draw);

    // Init OAM object.
    oamInitGfxSet(
        PLAYER_SPRITES, PLAYER_SPRITES_SIZE,       // Sprites + Length
        player.sData.palette, PLAYER_PALETTE_SIZE, // Palette + Length
        PLAYER_PALETTE_BANK,                       // Palette Bank
        MEM_SPRITES,                               // Where to put sprites.
        SPR_SIZE_16x32                             // Size of sprites.
    );

    oamSet(player.sData.oamID, player.pData.scrX, player.pData.scrY, 3, 0, 0, 0, 0);
    oamSetEx(player.sData.oamID, OBJ_SMALL, 1);
    oamSetVisible(player.sData.oamID, OBJ_SHOW);
	
	// This should probably be modular but for now we'll hard code it.
	player.aData.frameTimer = 0;
	player.aData.curFrame = 0;
	player.aData.sprState = 0;
	player.aData.timePerFrame = 30;
    player.pData.eBits = eBits;
	
	// Init Bullet Sprites
	oamInitGfxSet(
        &bulletspr, (&bulletspr_end - &bulletspr),
        &bulletbluepal, (&bulletbluepal_end - &bulletbluepal),
        PLAYER_PALETTE_BANK+1,
        0x5400,
        SPR_SIZE_16x16
    );

    return player;
}

// Functions
/*	Player Player_Init(x, y, speed);
	Returns a player struct.
x, y	;	Starting position.
speed	;	Starting speed.
*/
s_objectData Bullet_Init(u8 x, u8 y, sfx xSpeed, sfx ySpeed, u8 eBits, Level* lvl) {
    // Init player.
    s_objectData bullet = generic_init_obj(OBJECT_BULLET, x, y, CharToUFX(1, 0), get_free_oamid(lvl), PLAYER_PALETTE, &bullet_tick, &bullet_draw);
    bullet.pData.dX = xSpeed;
    bullet.pData.dY = ySpeed;
    bullet.pData.hitBoxSizeY = 16;
    bullet.pData.eBits = eBits;

    oamSet(bullet.sData.oamID, bullet.pData.scrX, bullet.pData.scrY, 3, 0, 0, 0, 0);
    oamSetEx(bullet.sData.oamID, OBJ_SMALL, 1);
    oamSetVisible(bullet.sData.oamID, OBJ_SHOW);
	
	// This should probably be modular but for now we'll hard code it.
	bullet.aData.frameTimer = 0;
	bullet.aData.curFrame = 0;
	bullet.aData.sprState = 0;
	bullet.aData.timePerFrame = 30;

    return bullet;
}


/*	Player Target_Init(x, y, speed);
	Returns a collider object.
x, y	        ;	Starting position.
sizeX, sizeY	;	Size.
*/
s_objectData Collider_Init(u8 x, u8 y, u8 sizeX, u8 sizeY, u8 eBits, Level* lvl) {
    // Init collider.
    s_objectData collider = generic_init_obj(OBJECT_COLLIDER, x, y, 0, 0, 0, NULL, NULL);

    collider.pData.hitBoxSizeX = sizeX;
    collider.pData.hitBoxSizeY = sizeY;
    collider.aData.sprState = 0;
    collider.pData.eBits = eBits;

    return collider;
}

s_objectData Angle_Init(u8 x, u8 y, u8 eBits, Level* lvl) {
    // Init collider.
    s_objectData angle = generic_init_obj(OBJECT_ANGLE, x, y, 0, 0, 0, NULL, NULL);

    angle.pData.hitBoxSizeX = 16;
    angle.pData.hitBoxSizeY = 16; 
    angle.aData.sprState = 0;
    angle.pData.eBits = eBits;

    return angle;
}

void obj_kill(s_objectData* obj) {
    obj->aData.sprState = 255;
    oamSetVisible(obj->sData.oamID, OBJ_HIDE);
    obj->draw_ptr = NULL;
    obj->update_ptr = NULL;
}

void add_obj_to_lvl(s_objectData obj, Level *level) {
    u8 idx = get_free_obj_slot(level);
    level->data->objects[idx] = obj;
}

void clear_lvl_objs(Level *level) {
    u8 i = 0;
    while(i < LEVEL_MAX_OBJECTS) {
        obj_kill(&level->data->objects[i]);
        ++i;
    }
}

// don't... don't read it please
u8 get_free_oamid(Level* level) {
    // create an index
    u16 i = 0;
    // bitmask we use to mark free positions with 1's
    u16 free_id = 0xFFFF;
    while(i < LEVEL_MAX_OBJECTS) {
        // if this sprite is free, we skip
        if(level->data->objects[i].aData.sprState == 255) {
            ++i;
            continue;
        }
        // black magic bit manip
        // essentially converts the multiples of 4 to bit positions
        // so 4 * 3 (12) because OAM IDs are multiples of 4
        // 4 * 3 becomes 1 << 3 for the mask, which marks OAM ID 12 as in-use
        free_id &= ~(1 << (level->data->objects[i].sData.oamID >> 2));
        ++i;
    }
    i = 0;
    // increment i until we find an unused OAM ID, prefering lower ones
    while((free_id & (1 << i)) == 0) {
        i++;
    }
    return i << 2;
}

u8 get_free_obj_slot(Level* level) {
    // create an index
    u8 i = 0;
    while(level->data->objects[i].aData.sprState != 255 && i <= LEVEL_MAX_OBJECTS) {
        ++i;
    }
    if(i == LEVEL_MAX_OBJECTS) i = -1;
    return i;
}

u16 CheckCollision_obj_obj(s_objectData *objA, s_objectData *objB) {
    u16 collision_result = 0;
    u16 objALeft = UFXToChar(UFXAdd(objA->pData.wX, objA->pData.hitBoxOffsetX));
    u16 objATop = UFXToChar(UFXAdd(objA->pData.wY, objA->pData.hitBoxOffsetY));
    u16 objARight = objALeft + objA->pData.hitBoxSizeX;
    u16 objABottom = objATop + objA->pData.hitBoxSizeY;

    u16 objBLeft = UFXToChar(UFXAdd(objB->pData.wX, objB->pData.hitBoxOffsetX));
    u16 objBTop = UFXToChar(UFXAdd(objB->pData.wY, objB->pData.hitBoxOffsetY));
    u16 objBRight = objBLeft + objB->pData.hitBoxSizeX;
    u16 objBBottom = objBTop + objB->pData.hitBoxSizeY;

    // if(objALeft > objBRight) goto finish_collision;
    // if(objATop > objBBottom) goto finish_collision;
    // if(objARight < objBLeft) goto finish_collision;
    // if(objABottom < objBTop) goto finish_collision;

    if (objALeft < objBRight && objARight > objBLeft && objATop < objBBottom && objABottom > objBTop) {
        collision_result |= 1;
        collision_result |= (objB->objID & 0b1111111) << 1;
        collision_result |= (objB->pData.eBits & 0b1111) << 12;

        // This looks expensive...
        if(objBLeft <= objARight && objARight - objBLeft < 2) {
            collision_result |= 1 << 8;
            goto finish_collision;
        }
        if(objBRight >= objALeft && objBRight - objALeft < 2) {
            collision_result |= 1 << 9;
            goto finish_collision;
        }
        if(objBTop <= objABottom && objABottom - objBTop < 2) {
            collision_result |= 1 << 10;
            goto finish_collision;
        }
        if(objBBottom >= objATop && objBBottom - objATop < 2) {
            collision_result |= 1 << 11;
            goto finish_collision;
        }
    }

    finish_collision:

    // Ends up looking like
    // eeeetblr iiiiiiic
    // e - extra bits
    // t - collision on top of objA
    // b - collision on bottom of objA
    // l - collision on left of objA
    // r - collision on right of objA
    // i - objB ID
    // c - collision happening

    return collision_result;
}

u8 Collide_obj_colliders(s_objectData* obj, Level* lvl) {
    u8 collision_dirs = 0;
    u8 i = 0;
    while(i < LEVEL_MAX_OBJECTS) {
        if(lvl->data->objects[i].aData.sprState == 255 || 
        lvl->data->objects[i].objID != OBJECT_COLLIDER) {
            ++i;
            continue;
        }
        u16 col_res = CheckCollision_obj_obj(obj, &lvl->data->objects[i]);
        if(col_res & 1 == 0) {
            ++i;
            continue;
        }
        collision_dirs |= col_res >> 8;
        collision_dirs |= 1 << 7;  
        ++i;
    }
    return collision_dirs;
}

u8 Collide_obj_angles(s_objectData* obj, Level* lvl) {
    u8 collision_dirs = 0;
    u8 i = 0;
    while(i < LEVEL_MAX_OBJECTS) {
        if(lvl->data->objects[i].aData.sprState == 255 || 
        lvl->data->objects[i].objID != OBJECT_ANGLE) {
            ++i;
            continue;
        }
        u16 col_res = CheckCollision_obj_obj(obj, &lvl->data->objects[i]);
        if(col_res & 1 == 0) {
            ++i;
            continue;
        }
        collision_dirs |= col_res >> 8;
        collision_dirs |= 1 << 7;  
        ++i;
    }
    return collision_dirs;
}

u8 Collide_obj_bullets(s_objectData* obj, Level* lvl) {
    u8 collision_dirs = 0;
    u8 i = 0;
    while(i < LEVEL_MAX_OBJECTS) {
        if(lvl->data->objects[i].aData.sprState == 255 || 
        lvl->data->objects[i].objID != OBJECT_BULLET) {
            ++i;
            continue;
        }
        u16 col_res = CheckCollision_obj_obj(obj, &lvl->data->objects[i]);
        if(col_res & 1 == 0) {
            ++i;
            continue;
        }
        collision_dirs |= col_res >> 8;
        collision_dirs |= 1 << 7;  
        ++i;
    }
    return collision_dirs;
}

s_objectData generic_init_obj(u8 id, u8 x, u8 y, ufx speed, u8 oamID, u8* palette, void (*update_ptr)(u16, struct s_objectData *), void (*draw_ptr)(struct s_objectData *))
{
    s_objectData obj = {
    {x, y,
        CharToSFX(x, 0), CharToSFX(y, 0),
        0, 0,
        speed,
        0, 8,
        16, 32,
        0, 0, 0, 0},
    {oamID,
        1,
        0,
        0,
        palette},
    {0, 0, 0, 0},
    id,
    update_ptr,
    draw_ptr};
    return obj;
}

void player_tick(u16 pad0, s_objectData *player, Level* level) {
    // Physics.
    player->pData.dX = player->pData.dY = 0;
    u8 collision_dirs = Collide_obj_colliders(player, level);

    if (pad0) { // If a button has been pressed.
        // Movement
        if (pad0 & KEY_LEFT && !(collision_dirs & COLLISION_LEFT)) {
            player->pData.dX = -player->pData.speed;
			player->aData.sprState = PS_SIDE;
			player->sData.hFlip = 1;
			
			// wwwwWWWWWWWWWWWWWWAITER
			// more band-aid fixes please.
			if(player->aData.curFrame > 1)
				player->aData.curFrame = 0;
		}
        if (pad0 & KEY_RIGHT && !(collision_dirs & COLLISION_RIGHT)) {
            player->pData.dX = player->pData.speed;
			player->aData.sprState = PS_SIDE;
			player->sData.hFlip = 0;
			if(player->aData.curFrame > 1)
				player->aData.curFrame = 0;
		}
        if (pad0 & KEY_UP && !(collision_dirs & COLLISION_UP)) {
            player->pData.dY = -player->pData.speed;
			player->aData.sprState = PS_UP;
		}
        if (pad0 & KEY_DOWN && !(collision_dirs & COLLISION_DOWN)) {
			player->pData.dY = player->pData.speed;
			player->aData.sprState = PS_DOWN;
		}

        if(pad0 & KEY_Y && player->pData.genTimer == 0) {
            spawn_bullet(player, level);
            player->pData.genTimer = 16;
        }

        // Shooting
        // BIG BIG TODO
		
		player->aData.frameTimer++;
    }

    if(player->pData.genTimer > 0) {
        --player->pData.genTimer;
    }

    player->pData.wX += player->pData.dX;
    player->pData.wY += player->pData.dY;

    player->pData.scrX = SFXToChar(player->pData.wX);
    player->pData.scrY = SFXToChar(player->pData.wY);
	
	// Animation
	if(player->aData.frameTimer > player->aData.timePerFrame){
		player->aData.frameTimer = 0;
		
		u8 maxFrame;
		switch(player->aData.sprState){
			default:
				maxFrame = 3;
			break;
			case PS_SIDE:
				maxFrame = 1;
			break;
		}
		
		if((player->aData.sprState == PS_UP || player->aData.sprState == PS_DOWN) && player->aData.curFrame % 2 == 0)
			player->sData.hFlip = !player->sData.hFlip;
		
		player->aData.curFrame = (player->aData.curFrame == maxFrame) ? 0 : player->aData.curFrame + 1;
	}
}

void spawn_bullet(s_objectData* obj, Level* lvl) {
    if(obj->aData.sprState == PS_SIDE) {
        if(obj->sData.hFlip == 0) {
            add_obj_to_lvl(Bullet_Init(obj->pData.scrX, obj->pData.scrY, CharToSFX(1, 0), CharToSFX(0, 0), 0, lvl), lvl);
        } else {
            add_obj_to_lvl(Bullet_Init(obj->pData.scrX, obj->pData.scrY, CharToSFX(-1, 0), CharToSFX(0, 0), 0, lvl), lvl);
        }
    }
    if(obj->aData.sprState == PS_UP) {
        add_obj_to_lvl(Bullet_Init(obj->pData.scrX, obj->pData.scrY, CharToSFX(0, 0), CharToSFX(-1, 0), 0, lvl), lvl);
    }
    if(obj->aData.sprState == PS_DOWN) {
        add_obj_to_lvl(Bullet_Init(obj->pData.scrX, obj->pData.scrY, CharToSFX(0, 0), CharToSFX(1, 0), 0, lvl), lvl);
    }
}

void player_draw(s_objectData *player) {
    //generic_draw(player);
	
	// This is moved here due to needing playerSpriteTable.
	// Alternativaley we could fit all sprites into a single sprite table and depend on sprState to point towards the right index.
	oamSetVisible(player->sData.oamID, player->sData.visible ? OBJ_SHOW : OBJ_HIDE);
    oamSet(player->sData.oamID, player->pData.scrX, player->pData.scrY, 3, player->sData.hFlip, player->sData.vFlip, playerSpriteTable[player->aData.curFrame + player->aData.sprState], 0);
}

void target_tick(u16 pad0, s_objectData *target, Level* level) {
    u8 collision_dirs = Collide_obj_bullets(target, level);

    if((collision_dirs & 0b1111) > 0) {
        obj_kill(target);
        return;
    }

    target->pData.wX += target->pData.dX;
    target->pData.wY += target->pData.dY;

    target->pData.scrX = SFXToChar(target->pData.wX);
    target->pData.scrY = SFXToChar(target->pData.wY);

    target->sData.visible = 1;//CheckCollision_obj_obj(&level->data->objects[0], target) & 1;
}

void target_draw(s_objectData *target) {
    //generic_draw(target);
	
	oamSetVisible(target->sData.oamID, target->sData.visible ? OBJ_SHOW : OBJ_HIDE);
    oamSet(target->sData.oamID, target->pData.scrX, target->pData.scrY, 3, target->sData.hFlip, target->sData.vFlip, soulSpriteTable[target->aData.curFrame + target->aData.sprState], 2);
}

void bullet_tick(u16 pad0, s_objectData *bullet, Level* level) {
    u8 collision_dirs = Collide_obj_colliders(bullet, level);

    // bullet only collides with collider type 0
    if((collision_dirs & 0b01110000) > 0) collision_dirs = 0;

    if((collision_dirs & 0b1111) > 0) {
        obj_kill(bullet);
        return;
    }

    collision_dirs = Collide_obj_angles(bullet, level);
    u8 angle = (collision_dirs & 0b01110000) >> 4;
    if((collision_dirs & 0b1111) > 0) {
        if(angle == 0 && bullet->pData.dX != 0) {
            bullet->pData.dY = bullet->pData.dX;
            bullet->pData.dX = 0;
        }
        if(angle == 1 && bullet->pData.dX != 0) {
            bullet->pData.dY = -bullet->pData.dX;
            bullet->pData.dX = 0;
        }
        if(angle == 2 && bullet->pData.dY != 0) {
            bullet->pData.dX = bullet->pData.dY;
            bullet->pData.dY = 0;
        }
        if(angle == 3 && bullet->pData.dY != 0) {
            bullet->pData.dX = -bullet->pData.dY;
            bullet->pData.dY = 0;
        }
    }

    bullet->pData.wX += bullet->pData.dX;
    bullet->pData.wY += bullet->pData.dY;

    bullet->pData.scrX = SFXToChar(bullet->pData.wX);
    bullet->pData.scrY = SFXToChar(bullet->pData.wY);
}

void bullet_draw(s_objectData *bullet) {
    generic_draw(bullet);
}

void generic_draw(s_objectData *object) {
    oamSetVisible(object->sData.oamID, object->sData.visible ? OBJ_SHOW : OBJ_HIDE);

    if(!object->sData.visible) return;

    // Update player OAM object.
    oamSet(object->sData.oamID, object->pData.scrX, object->pData.scrY, 3, object->sData.hFlip, object->sData.vFlip, 0, 0);
}

/*	void generic_copy_data_to_sneslib_obj(sobj, tobj);
	Copies relevant fields from sobj to tobj.
	https://alekmaul.github.io/pvsneslib/structt__objs.html
sobj	;	Generic Data Object
tobj	;	sneslib Data Object
*/
void generic_copy_data_to_sneslib_obj(s_objectData *sobj, t_objs *tobj) {
    //tobj->action = sobj->aData.sprState;		// This may be a stretch to include?
	//tobj->sprflip = sobj->sData.hFlip;		// Also may be a stretch? Not sure how sprflip is supposed to be used.
	//tobj->sprid3216 = sobj->sData.oamID;		// This isn't supposed to be the OAM ID?
	//tobj->sprpal = sobj->sData.palette;		// This isn't a pointer, so may just be asking for the palette bank index?
	//tobj->tempo = ???;						// What the fuck is tempo?
	//tobj->xofs = sobj->pData.hitBoxOffsetX;	// May be a stretch?
	//tobj->yofs = sobj->pData.hitBoxOffsetY;
	
	/*
	tobj->height = sobj->pData.hitBoxSizeY;
	tobj->width = sobj->pData.hitBoxSizeX;
	tobj->sprframe = sobj->aData.curFrame;
	tobj->sprnum = sobj->sData.oamID;
	tobj->xpos = SFXToChar(sobj->pData.wX);
	tobj->ypos = SFXToChar(sobj->pData.wY);
	*/
}

