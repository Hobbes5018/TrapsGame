#include "TrapHeaders.h"

HobbesHitbox *GroundHB;
HobbesHitbox *TestHB;
HobbesHitbox *HBList[2];

int Level1(){
	static HobbesBool Init = HOBBES_TRUE;
	if (Init){
		GroundHB = HobbesCreateHitbox(0, WINDOWY-3*64, WINDOWX, 3*64, HOBBES_TRUE);
		TestHB = HobbesCreateHitbox(50, 156, WINDOWX-50, 64, HOBBES_TRUE);
		HBList[0] = GroundHB;
		HBList[1] = TestHB;
		
		TrapSpike->hitbox->collisionnable = HOBBES_TRUE;
		TrapSpike->hidden = HOBBES_FALSE;
		HobbesPlaceSprite(TrapSpike, WINDOWX-300, WINDOWY-256);
		
		Init = HOBBES_FALSE;
	}

	TrapMove(HBList, 2, HOBBES_TRUE);
	
	if (HobbesHasContact(TrapPlayer->hitbox, TrapSpike->hitbox)){
		HobbesPlaceSprite(TrapPlayer, BaseSpawnX, BaseSpawnY);
		printf("1 life lost!\n");
		NumberOfLives -=1;
		if (NumberOfLives == 0){
			HobbesDestroyHitbox(GroundHB);
			TrapRunning = HOBBES_FALSE;
		}
	}
	
	//Display
	for (int x = 0; x < WINDOWX; x += 64){
		for (int y = WINDOWY-3*64; y <= WINDOWY-64; y += 64){
			HobbesPlaceSprite(TrapBrick, x, y);
			if (HobbesDisplaySprite(TrapBrick) != 0){
				fprintf(stderr, "Error while displaying ground (%s)!", SDL_GetError());
				TrapRunning = HOBBES_FALSE;
			}
		}
	}
	for (int x = 50; x < WINDOWX; x += 64){
		HobbesPlaceSprite(TrapBrick, x, 156);
		HobbesDisplaySprite(TrapBrick);
	}
	
	if (HobbesDisplaySprite(TrapSpike) != 0){
		fprintf(stderr, "Error while displaying spike (%s)!", SDL_GetError());
		TrapRunning = HOBBES_FALSE;
	}
	
	TrapShowLivesCount();
	TrapShowHitboxes((HobbesHitbox *[]){GroundHB, TestHB, TrapPlayer->hitbox, TrapSpike->hitbox}, 4);
	
	if (HobbesHasContact(TrapPlayer->hitbox, WindowRightBorder)){
		return 1;
	}
		
	return 0;
}