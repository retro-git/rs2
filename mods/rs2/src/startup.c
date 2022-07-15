#include "game.h"
#include "hook.h"
#include "startup.h"
#include "libc.h" 
#include "libcd.h" 

void intro(int i)
{
    patch_jump((int32_t*)0x80077318, (int32_t)draw_watermark);
   // LIBC_bzero((void*)0x80180000, 0x1c000);
    GAME_FUN_80077374(i);
}

void draw_watermark(RECT *recp, unsigned int *p)
{
    *(char*)p = 0xdd;
    LIBG_LoadImage(recp, p);
}