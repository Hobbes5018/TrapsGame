#include "TrapHeaders.h"



int main(void){
	if (TrapInitContext("Trap adventures") != 0){
		TrapRunning = HOBBES_FALSE;
	}
	
	int levelret = 0;
	
	while (TrapRunning){
		if (TrapGetKeyState(TrapKeySpace))
			TrapRunning = HOBBES_FALSE;
		
		if (SDL_RenderClear(TrapRenderer) != 0){
			fprintf(stderr, "Error while clearing renderer: %s\n", SDL_GetError());
			TrapRunning = HOBBES_FALSE;
		}
		
		PlayerX = TrapPlayer->rect->x;
		PlayerY = TrapPlayer->rect->y;

		/*=======================================================================*/
		switch (CurrentLevel){
			case 1:
				levelret = Level1();
				if (levelret == 1)
					CurrentLevel = 2;
				break;
			case 2:
				levelret = Level2();
				if (levelret)
					TrapRunning = HOBBES_FALSE;
				break;
		}
		/*=======================================================================*/
		
		
		if (HobbesDisplaySprite(TrapPlayer) != 0)
			TrapRunning = HOBBES_FALSE;

		SDL_RenderPresent(TrapRenderer);
//		TrapWaitNextFrame();
		SDL_Delay(10);
	}

	fclose(stderr);
	HobbesDestroySprite(TrapPlayer);
	HobbesDestroySprite(TrapBrick);
	HobbesDestroySprite(TrapSpike);
	SDL_DestroyRenderer(TrapRenderer);
	SDL_DestroyWindow(TrapWindow);
	HobbesQuit();
	printf("End!\n");

    return 0;
}