#ifndef TRAPCOLLISIONS_H
#define TRAPCOLLISIONS_H

#include "TrapHeaders.h"

enum TrapContact{
	TrapContactNone = 0,
	TrapContactLeft = 1,
	TrapContactRight = 2,
	TrapContactUp = 3,
	TrapContactDown = 4,
	TrapContactUpLeft = 5,
	TrapContactUpRight = 6,
	TrapContactDownLeft = 7,
	TrapContactDownRight = 8
};

HobbesBool TrapIsOutOfWindow(HobbesSprite *sprite);

#endif