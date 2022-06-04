#ifndef TRAP_MISC
#define TRAP_MISC

#include "TrapHeaders.h"

extern void TrapUpdateGlobals(HobbesHitbox **hitboxes, int count);
extern void TrapMove(HobbesHitbox **hitboxes, int count, HobbesBool UpdateGlobals);
extern void TrapShowHitboxes(HobbesHitbox **hitboxes, int count);
extern void TrapShowLivesCount();

#endif