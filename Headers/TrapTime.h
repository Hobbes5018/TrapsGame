#ifndef TRAPTIME_H
#define TRAPTIME_H

#include "TrapHeaders.h"

#define FRAME_LENGTH (20)//50 frames per second (50*20 = 1.000)

uint64_t TrapGetTicks();
void TrapWaitNextFrame();

#endif