#include "TrapHeaders.h"

static HobbesBool TrapKeyboardState[6];

void TrapPumpEvents(void){//This functions updates the keyboard's state
						  //It doesn't keep trace of any other event.
	SDL_Event event;
	while (SDL_PollEvent(&event)){
		if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP){
			switch (event.key.keysym.sym){
			
				case SDLK_UP:
					TrapKeyboardState[TrapKeyUp] = (event.type == SDL_KEYDOWN);
					break;
				
				case SDLK_DOWN:
					TrapKeyboardState[TrapKeyDown] = (event.type == SDL_KEYDOWN);
					break;
				
				case SDLK_LEFT:
					TrapKeyboardState[TrapKeyLeft] = (event.type == SDL_KEYDOWN);
					if (event.type == SDL_KEYDOWN)
						Direction = LEFT;
					else if (Direction == LEFT)
						Direction = NO_MOVEMENT;
					break;
				
				case SDLK_RIGHT:
					TrapKeyboardState[TrapKeyRight] = (event.type == SDL_KEYDOWN);
					if (event.type == SDL_KEYDOWN)
						Direction = RIGHT;
					else if (Direction == RIGHT)
						Direction = NO_MOVEMENT;
					break;
				
				case SDLK_SPACE:
					TrapKeyboardState[TrapKeySpace] = (event.type == SDL_KEYDOWN);
					break;
				
				case SDLK_h:
					if (event.type == SDL_KEYDOWN)
						VisibleHitboxes = !VisibleHitboxes;
					break;
				
				case SDLK_q:
					TrapRunning = HOBBES_FALSE;
			}
		}
	}
}

HobbesBool TrapGetKeyState(int key){
	TrapPumpEvents();
	if (key < 6 && key > 0)//key has a valid value
		return TrapKeyboardState[key];
	return 0;//If key has not a valid value, returns 0
}