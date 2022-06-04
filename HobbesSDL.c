#include <SDL2/SDL.h>
#include "HobbesSDL.h"
//#include <unistd.h>
#include <stddef.h>
#define Ellipsis ((void)0)
/*#define HOBBES_TRUE (SDL_TRUE)
#define HOBBES_FALSE (SDL_FALSE)

#define HOBBES_TYPEERROR (30)
#define HOBBES_RANGEERROR (31)

#define HOBBES_NOEVENT (0)
#define HOBBES_NULLEVENT ((HobbesEvent){HOBBES_NOEVENT, HOBBES_NULLKEYBOARDEVENT, HOBBES_NULLMOUSEEVENT, HOBBES_NULLWINDOWEVENT})

#define HOBBES_PRESS (1)
#define HOBBES_RELEASE (0)

#define HOBBES_KEYDOWN (50)
#define HOBBES_KEYUP (51)
#define HOBBES_KEYBOARDEVENT (52)
#define HOBBES_UNKNOWNKEY (65535)
#define HOBBES_NULLKEYBOARDEVENT ((HobbesKeyboardEvent){65535, 0})

#define HOBBES_WINDOWSHOWN (70)
#define HOBBES_WINDOWMOVE (71)
#define HOBBES_WINDOWRESIZE (72)
#define HOBBES_WINDOWENTER (73)
#define HOBBES_WINDOWLEAVE (74)
#define HOBBES_WINDOWCLOSE (75)
#define HOBBES_WINDOWEVENT (76)
#define HOBBES_NULLWINDOWEVENT ((HobbesWindowEvent){0, 0, 0, 0, 0})

#define HOBBES_MOUSEBUTTONUP (0)
#define HOBBES_MOUSEBUTTONDOWN (1)
#define HOBBES_MOUSEEVENT (82)
#define HOBBES_MOUSESCROLL (83)
#define HOBBES_MOUSEMOTION (84)
#define HOBBES_NULLMOUSEEVENT ((HobbesMouseEvent){0, 0, 0, 0, 0, 0, 0})
#define HOBBES_MOUSEBUTTONNONE (0)*/
//static HobbesBool HobbesKeyboardState[69];

int HobbesErrno = 0;

const int HobbesFlipHorizontal = 0;
const int HobbesFlipVertical = 1;
const int HobbesFlipBoth = 2;
const int HobbesFlipNone = 3;

int HobbesErrno;

int HobbesInit(void){
	int ret = SDL_Init(SDL_INIT_VIDEO);
	if (ret != 0)
		fprintf(stderr, "Error while initializing SDL2: %s", SDL_GetError());

	return ret;
}

int HobbesDisplaySprite(HobbesSprite *sprite);


HobbesSprite *HobbesCreateSprite(SDL_Renderer *renderer, const char src[], int x, int y){
	HobbesSprite *structure = malloc(5 * (int)(sizeof (void *))+sizeof (_Bool));
	structure->renderer = renderer;
	structure->surface = SDL_LoadBMP(src);
	if (structure->surface == NULL){
		fprintf(stderr, "Error while loading image on surface: %s\n", SDL_GetError());
		return NULL;
	}
	
	structure->texture = SDL_CreateTextureFromSurface(renderer, structure->surface);
	if (structure->texture == NULL){
		fprintf(stderr, "Error while creating texture from surface: %s\n", SDL_GetError());
		return NULL;
	}
	
	
	int rectw, recth;
	
	if (SDL_QueryTexture(structure->texture, NULL, NULL, &rectw, &recth) != 0){
		fprintf(stderr, "Error while getting texture info: %s\n", SDL_GetError());
		return NULL;
	}
	
	size_t sizeint = sizeof (int);
	structure->rect = malloc(4*sizeint);
	structure->rect->x = x;
	structure->rect->y = y;
	structure->rect->w = rectw;
	structure->rect->h = recth;
	if (rectw == 0 || recth == 0){
		fprintf(stderr, "Error while creating texture\n");
		return NULL;
	}
	structure->hidden = 0;
	
	structure->hitbox = malloc(4*sizeint+sizeof (_Bool));
	structure->hitbox->x = x;
	structure->hitbox->y = y;
	structure->hitbox->w = rectw;
	structure->hitbox->h = recth;
	structure->hitbox->collisionnable = SDL_TRUE;
	
	return structure;
}



void HobbesDestroySprite(HobbesSprite *sprite){
	SDL_FreeSurface(sprite->surface);
	SDL_DestroyTexture(sprite->texture);
	free(sprite->rect);
	free(sprite->hitbox);
	free(sprite);
}



HobbesSprite *HobbesCopySprite(HobbesSprite *sprite, int x, int y){
	HobbesSprite *structure = malloc(5 * (int)(sizeof (void *))+sizeof (_Bool));
	structure->renderer = sprite->renderer;
	structure->surface = sprite->surface;
	structure->texture = sprite->texture;
	structure->rect = sprite->rect;
	structure->hidden = sprite->hidden;
	structure->rect->x = x;
	structure->rect->y = y;
	
	structure->hitbox->x = x;
	structure->hitbox->y = y;
	structure->hitbox->w = sprite->hitbox->w;
	structure->hitbox->h = sprite->hitbox->h;
	structure->hitbox->collisionnable = sprite->hitbox->collisionnable;
	
	return structure;
}



int HobbesMoveSprite(HobbesSprite *sprite, int x, int y){
	sprite->rect->x += x;
	sprite->rect->y += y;
	sprite->hitbox->x += x;
	sprite->hitbox->y += y;
	return 0;
}



int HobbesDisplaySprite(HobbesSprite *sprite){
	if (sprite->hidden)
		return 0;

	if (SDL_RenderCopy(sprite->renderer, sprite->texture, NULL, &((SDL_Rect){sprite->rect->x, sprite->rect->y, sprite->rect->w, sprite->rect->h})) != 0){
		fprintf(stderr, "Error while displaying sprite: %s\n", SDL_GetError());
		return -1;
	}
//	(SDL_Rect *)sprite->rect
	
	return 0;
}



int HobbesDisplaySpriteAlt(HobbesSprite *sprite, HobbesRect *srcrect, HobbesRect *dstrect, double rotation, HobbesPoint *center, int flipmode){
	int ret;
	SDL_RendererFlip NewFlipmode;
	switch (flipmode){
		case 0://HobbesFlipHorizontal
			NewFlipmode = SDL_FLIP_HORIZONTAL;
		case 1://HobbesFlipVertical
			NewFlipmode = SDL_FLIP_VERTICAL;
		case 2://HobbesFlipBoth
			NewFlipmode = SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL;
		case 3://HobbesFlipNone
			NewFlipmode = SDL_FLIP_NONE;
	}
	SDL_Rect newsrcrect;
	SDL_Rect newdstrect;
	SDL_Point newcenter;
	SDL_Rect *ptrsrcrect = &newsrcrect;
	SDL_Rect *ptrdstrect = &newdstrect;
	SDL_Point *ptrcenter = &newcenter;
	if (srcrect == NULL)
		ptrsrcrect = NULL;
	else
		newsrcrect = ((SDL_Rect){srcrect->x, srcrect->y, srcrect->w, srcrect->h});
	
	if (dstrect == NULL)
		ptrdstrect = NULL;
	else
		newdstrect = ((SDL_Rect){dstrect->x, dstrect->y, dstrect->w, dstrect->h});
	
	if (center == NULL)
		ptrcenter = NULL;
	else
		newcenter = ((SDL_Point){center->x, center->y});
		
	return SDL_RenderCopyEx(sprite->renderer, sprite->texture,
	ptrsrcrect, ptrdstrect,
	rotation, ptrcenter,
	NewFlipmode);
	
	
/*	switch (flipmode){
		case 0://HobbesFlipHorizontal
			ret = SDL_RenderCopyEx(sprite->renderer, sprite->texture,
			&((SDL_Rect){srcrect->x, srcrect->y, srcrect->w, srcrect->h}),
			&((SDL_Rect){dstrect->x, dstrect->y, dstrect->w, dstrect->h}),
			rotation, &((SDL_Point){center->x, center->y}), SDL_FLIP_HORIZONTAL);
			break;
		
		case 1://HobbesFlipVertical
			ret = SDL_RenderCopyEx(sprite->renderer, sprite->texture,
			&((SDL_Rect){srcrect->x, srcrect->y, srcrect->w, srcrect->h}),
			&((SDL_Rect){dstrect->x, dstrect->y, dstrect->w, dstrect->h}),
			rotation, &((SDL_Point){center->x, center->y}), SDL_FLIP_VERTICAL);
			break;
		
		case 2://HobbesFlipBoth
			ret = SDL_RenderCopyEx(sprite->renderer, sprite->texture,
			&((SDL_Rect){srcrect->x, srcrect->y, srcrect->w, srcrect->h}),
			&((SDL_Rect){dstrect->x, dstrect->y, dstrect->w, dstrect->h}),
			rotation, &((SDL_Point){center->x, center->y}),
			SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
			break;
		
		case 3://HobbesFlipNone
			ret = SDL_RenderCopyEx(sprite->renderer, sprite->texture,
			&((SDL_Rect){srcrect->x, srcrect->y, srcrect->w, srcrect->h}),
			&((SDL_Rect){dstrect->x, dstrect->y, dstrect->w, dstrect->h}),
			rotation, &((SDL_Point){center->x, center->y}), SDL_FLIP_NONE);
			break;
	}
	return ret;*/
}



int HobbesDisplaySpritePos(HobbesSprite *sprite, int x, int y){
	return HobbesDisplaySpriteAlt(sprite, NULL,
	&((HobbesRect){x, y, sprite->rect->w, sprite->rect->h}),
	0., NULL, HobbesFlipNone);
}



int HobbesPlaceSprite(HobbesSprite *sprite, int x, int y){
	sprite->rect->x = x;
	sprite->rect->y = y;
	sprite->hitbox->x = x;
	sprite->hitbox->y = y;
	return 0;
}



void HobbesSetSpriteSize(HobbesSprite *sprite, int w, int h){
	sprite->rect->w = w;
	sprite->rect->h = h;
	sprite->hitbox->w = w;
	sprite->hitbox->h = h;
}



HobbesBool HobbesHasIntersection(HobbesHitbox *hitbox1, HobbesHitbox *hitbox2){
	if (!hitbox1->collisionnable || !hitbox2->collisionnable)
		return HOBBES_FALSE;

	else {
		SDL_Rect rect1 = {hitbox1->x, hitbox1->y, hitbox1->w, hitbox1->h};
		SDL_Rect rect2 = {hitbox2->x, hitbox2->y, hitbox2->w, hitbox2->h};
		return SDL_HasIntersection(&rect1, &rect2);
	}
}



uint8_t HobbesHasContact(HobbesHitbox *hitbox1, HobbesHitbox *hitbox2){
	if (hitbox1 == NULL || hitbox2 == NULL)
		return 0;
	
	uint8_t ret = 0;
	if (((hitbox1->y >= hitbox2->y && hitbox1->y < hitbox2->y + hitbox2->h) ||
	(hitbox1->y + hitbox1->h > hitbox2->y && hitbox1->y + hitbox1->h < hitbox2->y + hitbox2->h) ||
	(hitbox1->y > hitbox2->y && hitbox1->y+hitbox1->h < hitbox2->y+hitbox2->h))
	&& hitbox1->x + hitbox1->w >= hitbox2->x && hitbox1->x+hitbox1->w <= hitbox2->x+hitbox2->w){
		ret |= HobbesContactRight;
	}
	
	if (((hitbox1->y >= hitbox2->y && hitbox1->y < hitbox2->y + hitbox2->h) ||
	(hitbox1->y + hitbox1->h > hitbox2->y && hitbox1->y + hitbox1->h < hitbox2->y + hitbox2->h) ||
	(hitbox1->y > hitbox2->y && hitbox1->y+hitbox1->h < hitbox2->y+hitbox2->h))
	&& hitbox1->x <= hitbox2->x+hitbox2->w && hitbox1->x >= hitbox2->x){
		ret |= HobbesContactLeft;
	}
	
	if (((hitbox1->x >= hitbox2->x && hitbox1->x < hitbox2->x + hitbox2->w) ||
	(hitbox1->x + hitbox1->w > hitbox2->x && hitbox1->x + hitbox1->w < hitbox2->x + hitbox2->w) ||
	(hitbox1->x > hitbox2->x && hitbox1->x+hitbox1->w < hitbox2->x+hitbox2->w))
	&& hitbox1->y + hitbox1->h >= hitbox2->y && hitbox1->y+hitbox1->h <= hitbox2->y+hitbox2->h){
		ret |= HobbesContactBottom;
	}
	
	if (((hitbox1->x >= hitbox2->x && hitbox1->x < hitbox2->x + hitbox2->w) ||
	(hitbox1->x + hitbox1->w > hitbox2->x && hitbox1->x + hitbox1->w < hitbox2->x + hitbox2->w) ||
	(hitbox1->x < hitbox2->x && hitbox1->x+hitbox1->w > hitbox2->x+hitbox2->w))
	&& hitbox1->y <= hitbox2->y+hitbox2->h && hitbox1->y >= hitbox2->y){
		ret |= HobbesContactTop;
	}
	
	return ret;
}
/*HobbesBool HobbesHasContact(HobbesHitbox *hitbox1, HobbesHitbox *hitbox2){
	if (!hitbox1->collisionnable || !hitbox2->collisionnable)
		return HOBBES_FALSE;

	else {
		SDL_Rect rect1 = {hitbox1->x-1, hitbox1->y-1, hitbox1->w+2, hitbox1->h+2};
		SDL_Rect rect2 = {hitbox2->x, hitbox2->y, hitbox2->w, hitbox2->h};
		return SDL_HasIntersection(&rect1, &rect2);
	}
}

HobbesBool HobbesHasContactAdv(HobbesHitbox *hitbox, HobbesHitbox *hitboxes[]){
	int count = (sizeof hitboxes)/(sizeof (HobbesHitbox *));
	HobbesBool ret = 0;
	for (int i = 0; i < count; i++){
		if (HobbesHasContact(hitbox, hitboxes[i])){
			ret = HOBBES_TRUE;
			break;
		}
	}
	return ret;
}*/

HobbesHitbox *HobbesCreateHitbox(int x, int y, int w, int h, HobbesBool collisionnable){
	HobbesHitbox *ptr = malloc(4*sizeof(int) + sizeof(HobbesBool));
	ptr->x = x;
	ptr->y = y;
	ptr->w = w;
	ptr->h = h;
	ptr->collisionnable = collisionnable;
	return ptr;
}

void HobbesDestroyHitbox(HobbesHitbox *hitbox){
	free(hitbox);
}

void HobbesUpdateWindow(HobbesRenderer *renderer){
	SDL_RenderPresent(renderer);
}

int HobbesDrawHitboxBorder(HobbesHitbox *hitbox, HobbesRenderer *renderer,
uint8_t r, uint8_t g, uint8_t b, uint8_t a){

	int ret = 0;

	uint8_t ancientr;
	uint8_t ancientg;
	uint8_t ancientb;
	uint8_t ancienta;
	
	ret = SDL_GetRenderDrawColor(renderer, &ancientr, &ancientg, &ancientb, &ancienta);
	if (ret != 0){
		fprintf(stderr, "Error while showing hitbox border (%s)!\n", SDL_GetError());
		return ret;
	}
	
	ret = SDL_SetRenderDrawColor(renderer, r, g, b, a);
	if (ret != 0){
		fprintf(stderr, "Error while showing hitbox border (%s)!\n", SDL_GetError());
		return ret;
	}
	
	ret = SDL_RenderDrawLine(renderer, hitbox->x, hitbox->y,
	hitbox->x, hitbox->y+hitbox->h);
	if (ret != 0){
		fprintf(stderr, "Error while showing hitbox border (%s)!\n", SDL_GetError());
		return ret;
	}
	
	ret = SDL_RenderDrawLine(renderer, hitbox->x, hitbox->y+hitbox->h,
	hitbox->x+hitbox->w, hitbox->y+hitbox->h);
	if (ret != 0){
		fprintf(stderr, "Error while showing hitbox border (%s)!\n", SDL_GetError());
		return ret;
	}
	
	ret = SDL_RenderDrawLine(renderer, hitbox->x+hitbox->w, hitbox->y+hitbox->h,
	hitbox->x+hitbox->w, hitbox->y);
	if (ret != 0){
		fprintf(stderr, "Error while showing hitbox border (%s)!\n", SDL_GetError());
		return ret;
	}
	
	ret = SDL_RenderDrawLine(renderer, hitbox->x+hitbox->w, hitbox->y,
	hitbox->x, hitbox->y);
	if (ret != 0){
		fprintf(stderr, "Error while showing hitbox border (%s)!\n", SDL_GetError());
		return ret;
	}
	
	ret = SDL_SetRenderDrawColor(renderer, ancientr, ancientg, ancientb, ancienta);
	if (ret != 0){
		fprintf(stderr, "Error while showing hitbox border (%s)!\n", SDL_GetError());
		return ret;
	}
	
	return 0;
}

/*HobbesBool HobbesGetEvent(HobbesEvent *Event){

	SDL_Event event;
	HobbesBool done = HOBBES_FALSE;
	
	while (!done){ //This is to avoid getting useless window events at the beginning
		
		if (SDL_PollEvent(&event) == 0){
			*Event = HOBBES_NULLEVENT;//If there is no event, set Event to a null event
			return 0;
		}
		
		if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP){ //e.g. keyboard event
			Event->mouse = HOBBES_NULLMOUSEEVENT;
			Event->window = HOBBES_NULLWINDOWEVENT;

			_Bool state = (event.type == SDL_KEYDOWN);
			Event->keyboard.state = state;
			uint16_t key;
			
			switch (event.key.keysym.sym){ //Here we update HobbesKeyboardState and set
										   //the variable key to the matching keycode
				case SDLK_0:HobbesKeyboardState[0] = state;Event->keyboard.key = HOBBESK_0;break;
				case SDLK_1:HobbesKeyboardState[1] = state;Event->keyboard.key = HOBBESK_1;break;
				case SDLK_2:HobbesKeyboardState[2] = state;Event->keyboard.key = HOBBESK_2;break;
				case SDLK_3:HobbesKeyboardState[3] = state;Event->keyboard.key = HOBBESK_3;break;
				case SDLK_4:HobbesKeyboardState[4] = state;Event->keyboard.key = HOBBESK_4;break;
				case SDLK_5:HobbesKeyboardState[5] = state;Event->keyboard.key = HOBBESK_5;break;
				case SDLK_6:HobbesKeyboardState[6] = state;Event->keyboard.key = HOBBESK_6;break;
				case SDLK_7:HobbesKeyboardState[7] = state;Event->keyboard.key = HOBBESK_7;break;
				case SDLK_8:HobbesKeyboardState[8] = state;Event->keyboard.key = HOBBESK_8;break;
				case SDLK_9:HobbesKeyboardState[9] = state;Event->keyboard.key = HOBBESK_9;break;
				case SDLK_a:HobbesKeyboardState[10] = state;Event->keyboard.key = HOBBESK_A;break;
				case SDLK_b:HobbesKeyboardState[11] = state;Event->keyboard.key = HOBBESK_B;break;
				case SDLK_c:HobbesKeyboardState[12] = state;Event->keyboard.key = HOBBESK_C;break;
				case SDLK_d:HobbesKeyboardState[13] = state;Event->keyboard.key = HOBBESK_D;break;
				case SDLK_e:HobbesKeyboardState[14] = state;Event->keyboard.key = HOBBESK_E;break;
				case SDLK_f:HobbesKeyboardState[15] = state;Event->keyboard.key = HOBBESK_F;break;
				case SDLK_g:HobbesKeyboardState[16] = state;Event->keyboard.key = HOBBESK_G;break;
				case SDLK_h:HobbesKeyboardState[17] = state;Event->keyboard.key = HOBBESK_H;break;
				case SDLK_i:HobbesKeyboardState[18] = state;Event->keyboard.key = HOBBESK_I;break;
				case SDLK_j:HobbesKeyboardState[19] = state;Event->keyboard.key = HOBBESK_J;break;
				case SDLK_k:HobbesKeyboardState[20] = state;Event->keyboard.key = HOBBESK_K;break;
				case SDLK_l:HobbesKeyboardState[21] = state;Event->keyboard.key = HOBBESK_L;break;
				case SDLK_m:HobbesKeyboardState[22] = state;Event->keyboard.key = HOBBESK_M;break;
				case SDLK_n:HobbesKeyboardState[23] = state;Event->keyboard.key = HOBBESK_N;break;
				case SDLK_o:HobbesKeyboardState[24] = state;Event->keyboard.key = HOBBESK_O;break;
				case SDLK_p:HobbesKeyboardState[25] = state;Event->keyboard.key = HOBBESK_P;break;
				case SDLK_q:HobbesKeyboardState[26] = state;Event->keyboard.key = HOBBESK_Q;break;
				case SDLK_r:HobbesKeyboardState[27] = state;Event->keyboard.key = HOBBESK_R;break;
				case SDLK_s:HobbesKeyboardState[28] = state;Event->keyboard.key = HOBBESK_S;break;
				case SDLK_t:HobbesKeyboardState[29] = state;Event->keyboard.key = HOBBESK_T;break;
				case SDLK_u:HobbesKeyboardState[30] = state;Event->keyboard.key = HOBBESK_U;break;
				case SDLK_v:HobbesKeyboardState[31] = state;Event->keyboard.key = HOBBESK_V;break;
				case SDLK_w:HobbesKeyboardState[32] = state;Event->keyboard.key = HOBBESK_W;break;
				case SDLK_x:HobbesKeyboardState[33] = state;Event->keyboard.key = HOBBESK_X;break;
				case SDLK_y:HobbesKeyboardState[34] = state;Event->keyboard.key = HOBBESK_Y;break;
				case SDLK_z:HobbesKeyboardState[35] = state;Event->keyboard.key = HOBBESK_Z;break;
				case SDLK_F1:HobbesKeyboardState[36] = state;Event->keyboard.key = HOBBESK_F1;break;
				case SDLK_F2:HobbesKeyboardState[37] = state;Event->keyboard.key = HOBBESK_F2;break;
				case SDLK_F3:HobbesKeyboardState[38] = state;Event->keyboard.key = HOBBESK_F3;break;
				case SDLK_F4:HobbesKeyboardState[39] = state;Event->keyboard.key = HOBBESK_F4;break;
				case SDLK_F5:HobbesKeyboardState[40] = state;Event->keyboard.key = HOBBESK_F5;break;
				case SDLK_F6:HobbesKeyboardState[41] = state;Event->keyboard.key = HOBBESK_F6;break;
				case SDLK_F7:HobbesKeyboardState[42] = state;Event->keyboard.key = HOBBESK_F7;break;
				case SDLK_F8:HobbesKeyboardState[43] = state;Event->keyboard.key = HOBBESK_F8;break;
				case SDLK_F9:HobbesKeyboardState[44] = state;Event->keyboard.key = HOBBESK_F9;break;
				case SDLK_F10:HobbesKeyboardState[45] = state;Event->keyboard.key = HOBBESK_F10;break;
				case SDLK_F11:HobbesKeyboardState[46] = state;Event->keyboard.key = HOBBESK_F11;break;
				case SDLK_F12:HobbesKeyboardState[47] = state;Event->keyboard.key = HOBBESK_F12;break;
				case SDLK_F13:HobbesKeyboardState[48] = state;Event->keyboard.key = HOBBESK_F13;break;
				case SDLK_F14:HobbesKeyboardState[49] = state;Event->keyboard.key = HOBBESK_F14;break;
				case SDLK_F15:HobbesKeyboardState[50] = state;Event->keyboard.key = HOBBESK_F15;break;
				case SDLK_F16:HobbesKeyboardState[51] = state;Event->keyboard.key = HOBBESK_F16;break;
				case SDLK_F17:HobbesKeyboardState[52] = state;Event->keyboard.key = HOBBESK_F17;break;
				case SDLK_F18:HobbesKeyboardState[53] = state;Event->keyboard.key = HOBBESK_F18;break;
				case SDLK_F19:HobbesKeyboardState[54] = state;Event->keyboard.key = HOBBESK_F19;break;
				case SDLK_F20:HobbesKeyboardState[55] = state;Event->keyboard.key = HOBBESK_F20;break;
				case SDLK_F21:HobbesKeyboardState[56] = state;Event->keyboard.key = HOBBESK_F21;break;
				case SDLK_F22:HobbesKeyboardState[57] = state;Event->keyboard.key = HOBBESK_F22;break;
				case SDLK_F23:HobbesKeyboardState[58] = state;Event->keyboard.key = HOBBESK_F23;break;
				case SDLK_F24:HobbesKeyboardState[59] = state;Event->keyboard.key = HOBBESK_F24;break;
				case SDLK_VOLUMEUP:HobbesKeyboardState[60] = state;Event->keyboard.key = HOBBESK_VOLUMEUP;break;
				case SDLK_VOLUMEDOWN:HobbesKeyboardState[61] = state;Event->keyboard.key = HOBBESK_VOLUMEDOWN;break;
				case SDLK_UP:HobbesKeyboardState[62] = state;Event->keyboard.key = HOBBESK_UPARROW;break;
				case SDLK_DOWN:HobbesKeyboardState[63] = state;Event->keyboard.key = HOBBESK_DOWNARROW;break;
				case SDLK_LEFT:HobbesKeyboardState[64] = state;Event->keyboard.key = HOBBESK_LEFTARROW;break;
				case SDLK_RIGHT:HobbesKeyboardState[65] = state;Event->keyboard.key = HOBBESK_RIGHTARROW;break;
				case SDLK_SPACE:HobbesKeyboardState[66] = state;Event->keyboard.key = HOBBESK_SPACEBAR;break;
				case SDLK_RETURN:HobbesKeyboardState[67] = state;Event->keyboard.key = HOBBESK_RETURN;break;
				case SDLK_BACKSPACE:HobbesKeyboardState[68] = state;Event->keyboard.key = HOBBESK_BACKSPACE;break;
			}
			done = HOBBES_TRUE;
		}
		
		if (event.type == SDL_WINDOWEVENT){

			uint8_t type;
			HobbesBool get_wevent;
			
			if (event.window.type == SDL_WINDOWEVENT_SHOWN){
				Event->window.type = HOBBES_WINDOWSHOWN;
				type = HOBBES_WINDOWSHOWN;
				get_wevent = HOBBES_TRUE;
			}
			
			else if (event.window.type == SDL_WINDOWEVENT_MOVED){
				Event->window.type = HOBBES_WINDOWMOVE;
				type = HOBBES_WINDOWMOVE;
				get_wevent = HOBBES_TRUE;
			}
				
			else if (event.window.type == SDL_WINDOWEVENT_RESIZED){
				Event->window.type = HOBBES_WINDOWRESIZE;
				type = HOBBES_WINDOWRESIZE;
				get_wevent = HOBBES_TRUE;
			}
				
			else if (event.window.type == SDL_WINDOWEVENT_ENTER){
				Event->window.type = HOBBES_WINDOWENTER;
				type = HOBBES_WINDOWENTER;
				get_wevent = HOBBES_TRUE;
			}
				
			else if (event.window.type == SDL_WINDOWEVENT_LEAVE){
				Event->window.type = HOBBES_WINDOWLEAVE;
				type = HOBBES_WINDOWLEAVE;
				get_wevent = HOBBES_TRUE;
			}
				
			else if (event.window.type == SDL_WINDOWEVENT_CLOSE){
				Event->window.type = HOBBES_WINDOWCLOSE;
				type = HOBBES_WINDOWCLOSE;
				get_wevent = HOBBES_TRUE;
			}
			
			else
				get_wevent = HOBBES_FALSE;
			
			if (get_wevent){
			
				if (type == HOBBES_WINDOWSHOWN || type == HOBBES_WINDOWCLOSE//No move and no
				|| type == HOBBES_WINDOWENTER || type == HOBBES_WINDOWLEAVE)//resize
				{
					Event->window.xmove = 0;
					Event->window.ymove = 0;
					Event->window.xresize = 0;
					Event->window.yresize = 0;
				}
				else if (type == HOBBES_WINDOWMOVE){
					Event->window.xmove = event.window.data1;
					Event->window.ymove = event.window.data2;
					
					Event->window.xresize = 0;
					Event->window.yresize = 0;
					
				} else if (type == HOBBES_WINDOWRESIZE){
					Event->window.xresize = event.window.data1;
					Event->window.yresize = event.window.data2;
					
					Event->window.xmove = 0;
					Event->window.ymove = 0;
				}
				done = HOBBES_TRUE;
			}
		}
		
		if (event.type == SDL_MOUSEBUTTONUP || event.type == SDL_MOUSEBUTTONDOWN
		|| event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEWHEEL)
		{
			
			uint8_t type;
			if (event.type == SDL_MOUSEBUTTONUP){
				type = HOBBES_MOUSEBUTTONUP;
				Event->mouse.type = HOBBES_MOUSEBUTTONUP;
			}
			
			if (event.type == SDL_MOUSEBUTTONDOWN){
				type = HOBBES_MOUSEBUTTONDOWN;
				Event->mouse.type = HOBBES_MOUSEBUTTONDOWN;
			}
			
			if (event.type == SDL_MOUSEMOTION){
				type = HOBBES_MOUSEMOTION;
				Event->mouse.type = HOBBES_MOUSEMOTION;
			}
			
			if (event.type == SDL_MOUSEWHEEL){
				type = HOBBES_MOUSESCROLL;
				Event->mouse.type = HOBBES_MOUSESCROLL;
			}

			
			if (type == HOBBES_MOUSEBUTTONUP || type == HOBBES_MOUSEBUTTONDOWN){
				if (event.button.button == SDL_BUTTON_LEFT)
					Event->mouse.button = HOBBESB_LEFT;
				if (event.button.button == SDL_BUTTON_RIGHT)
					Event->mouse.button = HOBBESB_RIGHT;
				if (event.button.button == SDL_BUTTON_MIDDLE)
					Event->mouse.button = HOBBESB_MIDDLE;
				
				Event->mouse.state = (event.button.state == SDL_PRESSED);
				
				Event->mouse.xmotion = 0;
				Event->mouse.ymotion = 0;
				Event->mouse.xscroll = 0;
				Event->mouse.yscroll = 0;
				
				Event->mouse.xpos = event.button.x;
				Event->mouse.ypos = event.button.y;
			}
			else if (type == HOBBES_MOUSEMOTION){
			
				Event->mouse.type = HOBBES_MOUSEMOTION;
				
				Event->mouse.button = HOBBESB_NONE;
				Event->mouse.state = 0;
				
				Event->mouse.xmotion = event.motion.xrel;
				Event->mouse.ymotion = event.motion.yrel;
				
				Event->mouse.xscroll = 0;
				Event->mouse.yscroll = 0;
				
				Event->mouse.xpos = event.motion.x;
				Event->mouse.ypos = event.motion.y;

			}
			else {//e.g. type == HOBBES_MOUSESCROLL
				Event->mouse.type = HOBBES_MOUSESCROLL;
				
				Event->mouse.button = HOBBESB_NONE;
				Event->mouse.state = 0;
				Event->mouse.xmotion = 0;
				Event->mouse.ymotion = 0;
				Event->mouse.xpos = 0;
				Event->mouse.ypos = 0;
		
				Event->mouse.xscroll = event.wheel.x;
				Event->mouse.yscroll = event.wheel.y;
			} 
			done = HOBBES_TRUE;
		}
	}
	return 0;
}

HobbesBool *HobbesGetKeyboardState(){
	return &(HobbesKeyboardState[0]);
}

_Bool HobbesGetKeyState(int key){
	if (key < 0 || key > 68){
		HobbesErrno = HOBBES_RANGEERROR;
		return 0;
	}
	return HobbesGetKeyboardState()[key];
}


long long HobbesGetEvent(HobbesEvent *event){
	HobbesPumpEvents();
	if (HobbesPipeEventsCount == 0){
		*event = HOBBES_NULLEVENT;
		return HobbesPipeEventsCount;
	}
	
	HobbesPipeEventsCount -= 1;
	char type[1];
	size_t eventsize;
	if (read(HobbesPipeRead, type, 1) != 0){
		perror("Error while reading first char");
	}
	printf("First char has been read\n");
	switch ((uint8_t)type[0]){
		case HOBBES_KEYBOARDEVENT: eventsize = 3;
		case HOBBES_MOUSEEVENT: eventsize = 15;
		case HOBBES_WINDOWEVENT: eventsize = 9;
	}
	printf("Type of event has been determined\n");
	char data[eventsize];
	printf("A char array was allocated\n");
	if (read(HobbesPipeRead, data, eventsize) != 0){
		perror("Error while reading a whole event");
	}
	printf("The whole event has been read\n");
	
	
	if ((uint8_t)(type[0]) == HOBBES_KEYBOARDEVENT){
		event->mouse = HOBBES_NULLMOUSEEVENT;
		event->window = HOBBES_NULLWINDOWEVENT;
		
		event->keyboard.key = (uint16_t)data[0];
		event->keyboard.key <<= 8;
		event->keyboard.key += (uint16_t)data[1];
		event->keyboard.state = (HobbesBool)data[2];
	}
	
	if ((uint8_t)(type[0]) == HOBBES_MOUSEEVENT){
		event->keyboard = HOBBES_NULLKEYBOARDEVENT;
		event->window = HOBBES_NULLWINDOWEVENT;
		
		event->mouse.type = (uint8_t)data[0];
		event->mouse.button = (uint8_t)data[1];
		event->mouse.state = (HobbesBool)data[2];
		
		event->mouse.xmotion = (uint16_t)data[3];
		event->mouse.xmotion <<= 8;
		event->mouse.xmotion += (uint16_t)data[4];
		
		event->mouse.ymotion = (uint16_t)data[5];
		event->mouse.ymotion <<= 8;
		event->mouse.ymotion += (uint16_t)data[6];
		
		event->mouse.xscroll = (uint16_t)data[7];
		event->mouse.xscroll <<= 8;
		event->mouse.xscroll += (uint16_t)data[8];
		
		event->mouse.yscroll = (uint16_t)data[9];
		event->mouse.yscroll <<= 8;
		event->mouse.yscroll += (uint16_t)data[10];
		
		event->mouse.xpos = (uint16_t)data[11];
		event->mouse.xpos <<= 8;
		event->mouse.xpos += (uint16_t)data[12];
		
		event->mouse.ypos = (uint16_t)data[13];
		event->mouse.ypos <<= 8;
		event->mouse.ypos += (uint16_t)data[14];
	}
	
	if ((uint8_t)(type[0]) == HOBBES_WINDOWEVENT){
		event->keyboard = HOBBES_NULLKEYBOARDEVENT;
		event->mouse = HOBBES_NULLMOUSEEVENT;
		
		event->window.type = (uint8_t)data[0];
		
		event->window.xmove = (int16_t)data[1];
		event->window.xmove <<= 8;
		event->window.xmove += (int16_t)data[2];
		
		event->window.ymove = (int16_t)data[3];
		event->window.ymove <<= 8;
		event->window.ymove += (int16_t)data[4];
		
		event->window.xresize = (int16_t)data[5];
		event->window.xresize <<= 8;
		event->window.xresize += (int16_t)data[6];
		
		event->window.xresize = (int16_t)data[7];
		event->window.xresize <<= 8;
		event->window.xresize += (int16_t)data[8];
	}
	return HobbesPipeEventsCount;
}
*/

void HobbesQuit(void){
	SDL_Quit();
}
