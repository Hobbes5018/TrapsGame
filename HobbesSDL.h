#ifndef HOBBESSDL_H
#define HOBBESSDL_H
#include <SDL2/SDL.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdalign.h>

#define HOBBES_TRUE (SDL_TRUE)
#define HOBBES_FALSE (SDL_FALSE)

#define HOBBES_TYPEERROR (30)
#define HOBBES_RANGEERROR (31)

/*#define HOBBES_NOEVENT (0)
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
#define HOBBESB_NONE (0)*/



typedef _Bool HobbesBool;
typedef void HobbesVoid;
typedef SDL_Event HobbesEvent;

typedef SDL_Window HobbesWindow;
typedef SDL_Renderer HobbesRenderer;

typedef struct HobbesHitbox {
	int x;
	int y;
	int w;
	int h;
	HobbesBool collisionnable;
} HobbesHitbox;

typedef struct HobbesRect {
	int x;
	int y;
	int w;
	int h;
} HobbesRect;

typedef struct HobbesSprite {
	SDL_Renderer *renderer;
	SDL_Surface *surface;
	SDL_Texture *texture;
	HobbesRect *rect;
	HobbesHitbox *hitbox;
	HobbesBool hidden;
} HobbesSprite;

typedef struct HobbesPoint {
	int x;
	int y;
} HobbesPoint;

typedef enum{
	HobbesContactNone = 0,
	HobbesContactLeft = 1,
	HobbesContactRight = 2,
	HobbesContactTop = 4,
	HobbesContactBottom = 8,
	HobbesContactTopLeft = 16,
	HobbesContactTopRight = 32,
	HobbesContactBottomLeft = 64,
	HobbesContactBottomRight = 128
} HobbesContact;



/*typedef struct HobbesKeyboardEvent {
	uint16_t key;
	HobbesBool state;
} HobbesKeyboardEvent;

typedef struct HobbesMouseEvent{
	uint8_t type;
	uint8_t button;
	HobbesBool state;
	int16_t xmotion;
	int16_t ymotion;
	int16_t xscroll;
	int16_t yscroll;
	int16_t xpos;
	int16_t ypos;
} HobbesMouseEvent;

typedef struct HobbesWindowEvent{
	uint8_t type;
	int16_t xmove;
	int16_t ymove;
	int16_t xresize;
	int16_t yresize;
} HobbesWindowEvent;

typedef struct HobbesEvent{
	uint8_t type;
	HobbesKeyboardEvent keyboard;
	HobbesMouseEvent mouse;
	HobbesWindowEvent window;
} HobbesEvent;*/



extern int HobbesErrno;
extern int HobbesInit(void);
extern void HobbesQuit(void);

extern HobbesSprite *HobbesCreateSprite(SDL_Renderer *renderer, const char src[], int x, int y);
extern void HobbesDestroySprite(HobbesSprite *sprite);
extern HobbesSprite *HobbesCopySprite(HobbesSprite *sprite, int x, int y);
extern int HobbesMoveSprite(HobbesSprite *sprite, int x, int y);
extern int HobbesPlaceSprite(HobbesSprite *sprite, int x, int y);
extern void HobbesSetSpriteSize(HobbesSprite *sprite, int x, int y);

extern int HobbesDisplaySprite(HobbesSprite *sprite);
extern int HobbesDisplaySpriteAlt(HobbesSprite *sprite, HobbesRect *srcrect, HobbesRect *dstrect, double rotation, HobbesPoint *center, int flipmode);
extern int HobbesDisplaySpritePos(HobbesSprite *sprite, int x, int y);

extern HobbesBool HobbesHasIntersection(HobbesHitbox *object1, HobbesHitbox *object2);
extern uint8_t HobbesHasContact(HobbesHitbox *hitbox1, HobbesHitbox *hitbox2);
extern int HobbesDrawHitboxBorder(HobbesHitbox *hitbox, HobbesRenderer *renderer,
uint8_t r, uint8_t g, uint8_t b, uint8_t a);

extern void HobbesUpdateWindow(HobbesRenderer *renderer);

extern HobbesBool HobbesHasContactAdv(HobbesHitbox *hitbox, HobbesHitbox *hitboxes[]);
extern HobbesHitbox *HobbesCreateHitbox(int x, int y, int w, int h, HobbesBool collisionnable);
extern void HobbesDestroyHitbox(HobbesHitbox *hitbox);

/*
extern _Bool *HobbesGetKeyboardState();
extern _Bool HobbesGetKeyState(int key);
extern void HobbesPumpEvents(void);
extern int HobbesInit(void);
extern HobbesBool HobbesGetEvent(HobbesEvent *event);
extern void HobbesQuit(void);
*/


extern const int HobbesFlipHorizontal;
extern const int HobbesFlipVertical;
extern const int HobbesFlipBoth;
extern const int HobbesFlipNone;
extern int HobbesErrno;



/*typedef enum{
	HOBBESK_0 = 0,
	HOBBESK_1 = 1,
	HOBBESK_2 = 2,
	HOBBESK_3 = 3,
	HOBBESK_4 = 4,
	HOBBESK_5 = 5,
	HOBBESK_6 = 6,
	HOBBESK_7 = 7,
	HOBBESK_8 = 8,
	HOBBESK_9 = 9,
	HOBBESK_A = 10,
	HOBBESK_B = 11,
	HOBBESK_C = 12,
	HOBBESK_D = 13,
	HOBBESK_E = 14,
	HOBBESK_F = 15,
	HOBBESK_G = 16,
	HOBBESK_H = 17,
	HOBBESK_I = 18,
	HOBBESK_J = 19,
	HOBBESK_K = 20,
	HOBBESK_L = 21,
	HOBBESK_M = 22,
	HOBBESK_N = 23,
	HOBBESK_O = 24,
	HOBBESK_P = 25,
	HOBBESK_Q = 26,
	HOBBESK_R = 27,
	HOBBESK_S = 28,
	HOBBESK_T = 29,
	HOBBESK_U = 30,
	HOBBESK_V = 31,
	HOBBESK_W = 32,
	HOBBESK_X = 33,
	HOBBESK_Y = 34,
	HOBBESK_Z = 35,
	HOBBESK_F1 = 36,
	HOBBESK_F2 = 37,
	HOBBESK_F3 = 38,
	HOBBESK_F4 = 39,
	HOBBESK_F5 = 40,
	HOBBESK_F6 = 41,
	HOBBESK_F7 = 42,
	HOBBESK_F8 = 43,
	HOBBESK_F9 = 44,
	HOBBESK_F10 = 45,
	HOBBESK_F11 = 46,
	HOBBESK_F12 = 47,
	HOBBESK_F13 = 48,
	HOBBESK_F14 = 49,
	HOBBESK_F15 = 50,
	HOBBESK_F16 = 51,
	HOBBESK_F17 = 52,
	HOBBESK_F18 = 53,
	HOBBESK_F19 = 54,
	HOBBESK_F20 = 55,
	HOBBESK_F21 = 56,
	HOBBESK_F22 = 57,
	HOBBESK_F23 = 58,
	HOBBESK_F24 = 59,
	HOBBESK_VOLUMEUP = 60,
	HOBBESK_VOLUMEDOWN = 61,
	HOBBESK_UPARROW = 62,
	HOBBESK_DOWNARROW = 63,
	HOBBESK_LEFTARROW = 64,
	HOBBESK_RIGHTARROW = 65,
	HOBBESK_SPACEBAR = 66,
	HOBBESK_RETURN = 67,
	HOBBESK_BACKSPACE = 68,
} HobbesKey;

typedef enum {
	HOBBESB_LEFT = 1,//0 is occuped by HOBBES_MOUSEBUTTONNONE
	HOBBESB_RIGHT = 2,
	HOBBESB_MIDDLE = 3,
} HobbesMouseButton;

*/
#endif
