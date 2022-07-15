#include "game.h"

void patch_jump(int32_t *overwrite_loc, int32_t jump_loc)
{
    int32_t instr = ((jump_loc & 0b00000011111111111111111111111111) >> 2) | 0b0001100000000000000000000000000;
    *overwrite_loc = instr;
}