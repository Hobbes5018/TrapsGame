#include "TrapHeaders.h"

HobbesBool TrapIsOutOfWindow(HobbesSprite *sprite){
	if (sprite->hitbox->x < 0 || sprite->hitbox->y < 0 ||
	sprite->hitbox->x + sprite->hitbox->w > WINDOWX ||
	sprite->hitbox->y + sprite->hitbox->h > WINDOWY)
		return HOBBES_TRUE;
	return HOBBES_FALSE;
}