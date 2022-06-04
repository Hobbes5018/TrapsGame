#ifndef TRAPINIT_H
#define TRAPINIT_H

#include "TrapHeaders.h"

extern int TrapInitSDL();
extern HobbesWindow *TrapInitWindow(const char name[]);
extern HobbesRenderer *TrapInitRenderer();
extern int TrapInitContext(const char name[]);
extern int TrapInitSprites();
extern void TrapGetWindowSize(int *w, int *h);

#endif