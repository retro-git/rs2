#include "../../../include/common.h"
#include "draw.h"

void draw_hook(unsigned int unk) {
    spyro_FUN_80013a14(unk);

    spyro_DrawText("hellohello", 100, 100, 1, 0);
}