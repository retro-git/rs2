#ifndef STARTUP_H
#define STARTUP_H

#include "game.h"
#include "libgraphics.h"

void startup();
void place_hooks();
void draw_watermark(RECT *recp, unsigned int *p);

#endif