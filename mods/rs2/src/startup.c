#include "game.h"
#include "hook.h"
#include "startup.h"
#include "libc.h" 
#include "libcd.h" 
#include "draw.h"

void startup(int i) {
    place_hooks();

    GAME_FUN_80077374(i);
}

void place_hooks()
{
    patch_jump((int32_t *)0x80015808, (int32_t)draw_hook);
}