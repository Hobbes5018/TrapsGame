#include "TrapHeaders.h"

uint64_t TrapGetTicks(){
	uint64_t LastFrame = SDL_GetTicks();
	uint64_t TimeSinceLastFrame = SDL_GetTicks();
	TimeSinceLastFrame -= LastFrame;
	LastFrame += TimeSinceLastFrame;
	return TimeSinceLastFrame;
}

void TrapWaitNextFrame(){
	uint32_t TimeToDelay = FRAME_LENGTH - TrapGetTicks();
	if (TimeToDelay < 20)
		SDL_Delay(TimeToDelay);
}