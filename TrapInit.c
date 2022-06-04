#include "TrapHeaders.h"

static int WindowW = 0;
static int WindowH = 0;



/*These first four functions will be used to initialize the rendering context*/

int TrapInitSDL(){
	return HobbesInit();
}

HobbesWindow *TrapInitWindow(const char name[]){
	TrapWindow = SDL_CreateWindow(name, 0, 0, WINDOWX, WINDOWY,
	SDL_WINDOW_FULLSCREEN | SDL_WINDOW_BORDERLESS);
	if (TrapWindow == NULL){
		fprintf(stderr, "Error while creating window: %s\n", SDL_GetError());
	} else {
		SDL_GetWindowSize(TrapWindow, &WindowW, &WindowH);
	}
	
	WindowUpBorder = HobbesCreateHitbox(0, 10+PlayerH, WINDOWX, 1, HOBBES_TRUE);
	WindowRightBorder = HobbesCreateHitbox(WINDOWX, 0, 1, WINDOWY, HOBBES_TRUE);
	WindowDownBorder = HobbesCreateHitbox(0, WINDOWY, WINDOWX, 1, HOBBES_TRUE);
	WindowLeftBorder = HobbesCreateHitbox(-1, 0, 1, WINDOWY, HOBBES_TRUE);
	
	return TrapWindow;
}

HobbesRenderer *TrapInitRenderer(){
	TrapRenderer = SDL_CreateRenderer(TrapWindow, -1, 0);
	
	if (TrapRenderer == NULL){
		fprintf(stderr, "Error while creating renderer: %s\n", SDL_GetError());
		fclose(stderr);
	}
	if (SDL_SetRenderDrawBlendMode(TrapRenderer, SDL_BLENDMODE_BLEND) != 0)
		return NULL;

	return TrapRenderer;
}

int TrapInitContext(const char name[]){
//Initializes the rendering context using the above functions


	if (TrapInitSDL() != 0 || TrapInitWindow(name) == NULL || TrapInitRenderer() == NULL
	|| TrapInitSprites() != 0){
		fprintf(stderr, "Error while initializing rendering context!");
		return -1;
	}
	SDL_SetRenderDrawColor(TrapRenderer, 0, 0, 0, 255);
	
	return 0;
}

/*This function may be used to get the size of the window*/
void TrapGetWindowSize(int *w, int *h){
	*w = WindowW;
	*h = WindowH;
}


/*The following function will be used to create every sprite needed in the game*/

int TrapInitSprites(){

	TrapPlayer = HobbesCreateSprite(TrapRenderer, "Images/Player.bmp", 0, WINDOWY-3*64-44);
	
	if (TrapPlayer == NULL){
		return 1;
	}
	HobbesSetSpriteSize(TrapPlayer, 32, 44);
	
	
	TrapBrick = HobbesCreateSprite(TrapRenderer, "Images/Bricks.bmp", 0, 0);
	if (TrapBrick == NULL){
		return 1;
	}
	HobbesSetSpriteSize(TrapBrick, 64, 64);
	TrapBrick->hitbox->collisionnable = HOBBES_FALSE;
	
	
	TrapSpike = HobbesCreateSprite(TrapRenderer, "Images/Spike2.bmp", 0, 0);
	if (TrapSpike == NULL){
		return 1;
	}
	HobbesSetSpriteSize(TrapSpike, 32, 64);
	TrapSpike->hitbox->collisionnable = HOBBES_FALSE;
	
	
	return 0;
}