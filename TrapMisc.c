#include "TrapMisc.h"

void TrapUpdateGlobals(HobbesHitbox **hitboxes, int count){
	HobbesBool Computed = HOBBES_FALSE;
	PlayerContacts = 0;
	MaxLeftMove = LEFT;
	MaxRightMove = RIGHT;
	MaxUpMove = JUMP_SPEED;
	MaxDownMove = FALLING_SPEED;
	
	Compute:
	for (int i = 0; i < count; i++){
		PlayerContacts |= HobbesHasContact(TrapPlayer->hitbox, hitboxes[i]);
		
		if ((PlayerY < hitboxes[i]->y+hitboxes[i]->h && PlayerY >= hitboxes[i]->y) ||
		(PlayerY+PlayerH <= hitboxes[i]->y+hitboxes[i]->h &&
		PlayerY+PlayerH > hitboxes[i]->y) ||
		(PlayerY <= hitboxes[i]->y && PlayerY+PlayerH >= hitboxes[i]->y+hitboxes[i]->h))
		{
			if (hitboxes[i]->x+hitboxes[i]->w - PlayerX > MaxLeftMove
			&& hitboxes[i]->x < PlayerX){
				MaxLeftMove = hitboxes[i]->x+hitboxes[i]->w - PlayerX;
			}
			if (hitboxes[i]->x - (PlayerX+PlayerW) < MaxRightMove
			&& hitboxes[i]->x+hitboxes[i]->w > PlayerX+PlayerW){
				MaxRightMove = hitboxes[i]->x - (PlayerX+PlayerW);
			}
		}
		
		if ((PlayerX < hitboxes[i]->x+hitboxes[i]->w && PlayerX >= hitboxes[i]->x) ||
		(PlayerX+PlayerW <= hitboxes[i]->x+hitboxes[i]->w &&
		PlayerX+PlayerW > hitboxes[i]->x) ||
		(PlayerX <= hitboxes[i]->x && PlayerX+PlayerW >= hitboxes[i]->x+hitboxes[i]->w))
		{
			if (hitboxes[i]->y+hitboxes[i]->h - PlayerY > MaxUpMove
			&& hitboxes[i]->y < PlayerY){
				MaxUpMove = hitboxes[i]->y+hitboxes[i]->h - PlayerY;
			}
			
			
			if (hitboxes[i]->y - (PlayerY+PlayerH) < MaxDownMove
			&& hitboxes[i]->y+hitboxes[i]->h > PlayerY+PlayerH){
				MaxDownMove = hitboxes[i]->y - (PlayerY+PlayerH);
			}
		}
	}
	if (!Computed){
		HobbesHitbox *windowborders[4];
		windowborders[0] = WindowUpBorder;
		windowborders[1] = WindowRightBorder;
		windowborders[2] = WindowDownBorder;
		windowborders[3] = WindowLeftBorder;
		hitboxes = windowborders;
		count = 4;
		Computed = HOBBES_TRUE;
		goto Compute;
	}
	
	//Jumping phase handling
	if (TrapGetKeyState(TrapKeyUp) && (PlayerContacts & (1<<3))){
		JumpPhase = GOING_UP;
		JumpStartHeight = PlayerY;
	}
	if (JumpStartHeight -  PlayerY >= JUMP_HEIGHT || (PlayerContacts & (1<<2))){
		JumpPhase = AT_TOP;
		AtTopSince += 1;
	}
	if (AtTopSince == TIME_AT_TOP || ((!(PlayerContacts & (1<<3)) && JumpPhase != GOING_UP))){
		JumpPhase = FALLING;
	}
	if ((PlayerContacts & (1<<3)) && JumpPhase == FALLING){//Has a bottom contact
		AtTopSince = 0;
		JumpPhase = NO_MOVEMENT;
	}
}

void TrapMove(HobbesHitbox **hitboxes, int count, HobbesBool UpdateGlobals){
	if (UpdateGlobals)
		TrapUpdateGlobals(hitboxes, count);
	
	switch (Direction){
		case LEFT:
			if (!(PlayerContacts & (1<<0))){
				HobbesMoveSprite(TrapPlayer, MaxLeftMove, 0);
				if (TrapIsOutOfWindow(TrapPlayer)){
					HobbesPlaceSprite(TrapPlayer, 0, PlayerY);
				}
			}
			break;
		
		case RIGHT:
			if (!(PlayerContacts & (1<<1))){
				HobbesMoveSprite(TrapPlayer, MaxRightMove, 0);
				if (TrapIsOutOfWindow(TrapPlayer)){
					HobbesPlaceSprite(TrapPlayer, WINDOWX-32, PlayerY);
				}
			}
			break;
	}
	
	
	//Here we handle the jumping animation according to the phase
	if (JumpPhase == GOING_UP){
		HobbesMoveSprite(TrapPlayer, 0, MaxUpMove);
	}
	if (JumpPhase == FALLING){
		HobbesMoveSprite(TrapPlayer, 0, MaxDownMove);
	}
}

void TrapShowHitboxes(HobbesHitbox **hitboxes, int count){
	if (VisibleHitboxes){
		for (int i = 0; i < count; i++){
			HobbesDrawHitboxBorder(hitboxes[i], TrapRenderer, 0, 255, 255, 255);
		}
	}
}

void TrapShowLivesCount(){
	for (int i = 0; i < NumberOfLives; i++){
		HobbesDisplaySpriteAlt(TrapPlayer, NULL, &((HobbesRect){i*32+5*i, 5, PlayerW, PlayerH}),
		0, NULL, HobbesFlipNone);
	}
	SDL_SetRenderDrawColor(TrapRenderer, 0, 255, 0, 255);
	SDL_RenderDrawLine(TrapRenderer, 0, 10+PlayerH, WINDOWX, 10+PlayerH);
	SDL_SetRenderDrawColor(TrapRenderer, 0, 0, 0, 255);
}