#include "object_funcs.h"

u8 CheckCollision_obj_obj(s_objectData *objA, s_objectData *objB) {
    u8 collision_result = 0;
    u8 objALeft = UFXToChar(UFXAdd(objA->wX, objA->hitBoxOffsetX));
    u8 objATop = UFXToChar(UFXAdd(objA->wY, objA->hitBoxOffsetY));
    u8 objARight = objALeft + objA->hitBoxSizeX;
    u8 objABottom = objATop + objA->hitBoxSizeY;

    u8 objBLeft = SFXToChar(SFXAdd(objB->wX, objB->hitBoxOffsetX));
    u8 objBTop = SFXToChar(SFXAdd(objB->wY, objB->hitBoxOffsetY));
    u8 objBRight = objBLeft + objB->hitBoxSizeX;
    u8 objBBottom = objBTop + objB->hitBoxSizeY;

    objA->t = objATop;
    objA->b = objABottom;
    objA->l = objALeft;
    objA->r = objARight;

    objB->t = objBTop;
    objB->b = objBBottom;
    objB->l = objBLeft;
    objB->r = objBRight;

    if (objALeft < objBRight && objARight > objBLeft && objATop < objBBottom && objABottom > objBTop) {
        collision_result |= 1;
    }
    return collision_result;
}