#include "TrapHeaders.h"

//Everything must be initialized so that we don't get a linker error.

HobbesWindow *TrapWindow = NULL;
HobbesRenderer *TrapRenderer = NULL;

HobbesSprite *TrapPlayer = NULL;
HobbesSprite *TrapBrick = NULL;
HobbesSprite *TrapSpike = NULL;

short int JumpPhase = 0;//This variable is also used for falling handling
//0->Not jumping/moving, 1->Going up, 2->Is at the top of the jump,
//3->Falling

int JumpStartHeight = 0;
int AtTopSince = 0;
short int Direction = 0;//0->Not moving, 1->Moving left, 2->Moving right

int PlayerX = 20;
int PlayerY = 536;
int PlayerW = 32;
int PlayerH = 44;

uint8_t PlayerContacts = 0;
int MaxLeftMove = 0;
int MaxRightMove = 0;
int MaxUpMove = 0;
int MaxDownMove = 0;

int BaseSpawnX = 5;
int BaseSpawnY = 55;

int BrickW = 64;
int BrickH = 64;

TrapKey LastDirKey = TrapKeyNone;
HobbesBool VisibleHitboxes = HOBBES_FALSE;

HobbesHitbox *WindowUpBorder = NULL;
HobbesHitbox *WindowDownBorder = NULL;
HobbesHitbox *WindowLeftBorder = NULL;
HobbesHitbox *WindowRightBorder = NULL;

int NumberOfLives = 10;
int CurrentLevel = 1;

HobbesBool TrapRunning = HOBBES_TRUE;