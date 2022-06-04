#include "TrapHeaders.h"

int TrapDispSpriteAlt(HobbesSprite *sprite, int flipmode, double angle){
	static const int screenbaseW = 4096;
	static const int screenbaseH = 2304;
	int screennewW;
	int screennewH;
	TrapGetWindowSize(&screennewW, &screennewH);
	const int ratio = screennewH/screenbaseH;
	
	HobbesRect disprect = *(sprite->rect);
	disprect.x *= ratio;
	disprect.y *= ratio;
	disprect.w *= ratio;
	disprect.h *= ratio;
	
	return HobbesDisplaySpriteAlt(sprite, NULL, &disprect, angle, NULL, flipmode);
}

int TrapDispSprite(HobbesSprite *sprite){
	return TrapDispSpriteAlt(sprite, HobbesFlipNone, 0);
}