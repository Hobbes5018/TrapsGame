#ifndef TRAPGLOBALS_H
#define TRAPGLOBALS_H

//First we have some random macros

#define Ellipsis (return 0)
#define Pass ((void)0)

//These macros are used to handle window size
#define WINDOWX (1280)
#define WINDOWY (720)

/*The macros that will be used for jumping*/
#define GOING_UP (1)
#define AT_TOP (2)
#define FALLING (3)
#define JUMP_HEIGHT (300)
#define FALLING_SPEED (6)
#define JUMP_SPEED (-6)
#define TIME_AT_TOP (1)//The time that should be spend at the top of the jump, in frames

#define NO_MOVEMENT (0)//Used for jumping AND direction handling

/*The macros that will be used for direction handling*/
#define LEFT (-5)
#define RIGHT (5)


//Now there are global variables

//Rendering context
extern HobbesWindow *TrapWindow;
extern HobbesRenderer *TrapRenderer;

//Sprites
extern HobbesSprite *TrapPlayer;
extern HobbesSprite *TrapBrick;
extern HobbesSprite *TrapSpike;


//Movment variables

extern short int JumpPhase;//This variable is also used for falling handling
//0->Not jumping/moving, 1->Going up, 2->Is at the top of the jump,
//3->Falling

extern int JumpStartHeight;
extern int AtTopSince;
extern short int Direction;//0->Not moving, 1->Moving left, 2->Moving right

extern int PlayerX;
extern int PlayerY;
extern int PlayerW;
extern int PlayerH;

extern uint8_t PlayerContacts;
extern int MaxLeftMove;
extern int MaxRightMove;
extern int MaxUpMove;
extern int MaxDownMove;

extern int BaseSpawnX;
extern int BaseSpawnY;

extern int BrickW;
extern int BrickH;

extern HobbesBool TrapRunning;

extern TrapKey LastDirKey;
extern HobbesBool VisibleHitboxes;

extern HobbesHitbox *WindowUpBorder;
extern HobbesHitbox *WindowDownBorder;
extern HobbesHitbox *WindowLeftBorder;
extern HobbesHitbox *WindowRightBorder;

extern int NumberOfLives;
extern int CurrentLevel;

#endif