#include "object_funcs.h"
#include <snes.h>
#include "sprite_tables.h"

u8 CheckCollision_obj_obj(s_objectData *objA, s_objectData *objB) {
    u8 collision_result = 0;
    u8 objALeft = UFXToChar(UFXAdd(objA->pData.wX, objA->pData.hitBoxOffsetX));
    u8 objATop = UFXToChar(UFXAdd(objA->pData.wY, objA->pData.hitBoxOffsetY));
    u8 objARight = objALeft + objA->pData.hitBoxSizeX;
    u8 objABottom = objATop + objA->pData.hitBoxSizeY;

    u8 objBLeft = SFXToChar(SFXAdd(objB->pData.wX, objB->pData.hitBoxOffsetX));
    u8 objBTop = SFXToChar(SFXAdd(objB->pData.wY, objB->pData.hitBoxOffsetY));
    u8 objBRight = objBLeft + objB->pData.hitBoxSizeX;
    u8 objBBottom = objBTop + objB->pData.hitBoxSizeY;

    objA->pData.t = objATop;
    objA->pData.b = objABottom;
    objA->pData.l = objALeft;
    objA->pData.r = objARight;

    objB->pData.t = objBTop;
    objB->pData.b = objBBottom;
    objB->pData.l = objBLeft;
    objB->pData.r = objBRight;

    if (objALeft < objBRight && objARight > objBLeft && objATop < objBBottom && objABottom > objBTop) {
        collision_result |= 1;
    }
    return collision_result;
}

s_objectData generic_init_obj(u8 x, u8 y, ufx speed, u8 oamID, u8* palette, void (*update_ptr)(u16, struct s_objectData *), void (*draw_ptr)(struct s_objectData *))
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
    update_ptr,
    draw_ptr};
    return obj;
}

void player_tick(u16 pad0, s_objectData *player, Level* level) {
    // Physics.
    player->pData.dX = player->pData.dY = 0;
    if (pad0) { // If a button has been pressed.
        // Movement
        if (pad0 & KEY_LEFT){
            player->pData.dX = -player->pData.speed;
			player->aData.sprState = PS_SIDE;
			player->sData.hFlip = 0;
		}
        if (pad0 & KEY_RIGHT){
            player->pData.dX = player->pData.speed;
			player->aData.sprState = PS_SIDE;
			player->sData.hFlip = 1;
		}
        if (pad0 & KEY_UP){
            player->pData.dY = -player->pData.speed;
			player->aData.sprState = PS_UP;
		}
        if (pad0 & KEY_DOWN){
			player->pData.dY = player->pData.speed;
			player->aData.sprState = PS_DOWN;
		}

        // Shooting
        // BIG BIG TODO
		
		player->aData.frameTimer++;
    }

    player->pData.wX += player->pData.dX;
    player->pData.wY += player->pData.dY;

    player->pData.scrX = SFXToChar(player->pData.wX);
    player->pData.scrY = SFXToChar(player->pData.wY);
	
	// Animation
	if(player->aData.frameTimer > player->aData.timePerFrame){
		player->aData.frameTimer = 0;
		
		// Maybe make the max possible frame a var in aData?
		player->aData.curFrame = (player->aData.curFrame == 2) ? 0 : player->aData.curFrame + 1;
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
    target->pData.dX = target->pData.dY = 100;

    target->pData.wX += target->pData.dX;
    target->pData.wY += target->pData.dY;

    target->pData.scrX = SFXToChar(target->pData.wX);
    target->pData.scrY = SFXToChar(target->pData.wY);

    target->sData.visible = CheckCollision_obj_obj(&level->data->objects[0], target) & 1;
}

void target_draw(s_objectData *target) {
    generic_draw(target);
}

void generic_draw(s_objectData *object) {
    oamSetVisible(object->sData.oamID, object->sData.visible ? OBJ_SHOW : OBJ_HIDE);

    // Update player OAM object.
    oamSet(object->sData.oamID, object->pData.scrX, object->pData.scrY, 3, object->sData.visible, 0, 0, 0);
}