#ifndef TRAPKEYBOARD_H
#define TRAPKEYBOARD_H

#include "TrapHeaders.h"

typedef enum {
	TrapKeyNone = 0,
	TrapKeyUp = 1,
	TrapKeyDown = 2,
	TrapKeyLeft = 3,
	TrapKeyRight = 4,
	TrapKeySpace = 5
} TrapKey;

extern void TrapPumpEvents(void);
extern HobbesBool TrapGetKeyState(int key);

#endif