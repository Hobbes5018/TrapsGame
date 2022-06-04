#include "TrapHeaders.h"

HobbesHitbox *GroundLeftHB;
HobbesHitbox *GroundRightHB;
HobbesHitbox *HBList[2];

int Level2(){
	static HobbesBool Init = HOBBES_FALSE;
	if (!Init){
		GroundLeftHB = HobbesCreateHitbox(0, WINDOWY-3*64, 64, 3*64, HOBBES_TRUE);
		GroundRightHB = HobbesCreateHitbox(WINDOWX-128, WINDOWY-3*64, 128, 3*64, HOBBES_TRUE);
		HBList[0] = GroundLeftHB;
		HBList[1] = GroundRightHB;
		Init = HOBBES_TRUE;
	}
	
	TrapMove(HBList, 2, HOBBES_TRUE);
	
	TrapShowHitboxes((HobbesHitbox *[]){GroundLeftHB, GroundRightHB,
	TrapPlayer->hitbox}, 3);
	
	//Display
	for (int y = WINDOWY-3*64; y < WINDOWY; y += 64){
		HobbesPlaceSprite(TrapBrick, 0, y);
		HobbesDisplaySprite(TrapBrick);
	}
	for (int x = WINDOWX-128; x < WINDOWX; x += 64){
		for (int y = WINDOWY-3*64; y < WINDOWY; y += 64){
			HobbesPlaceSprite(TrapBrick, x, y);
			HobbesDisplaySprite(TrapBrick);
		}
	}
	
	return 0;
}