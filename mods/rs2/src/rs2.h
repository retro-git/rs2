#ifndef RS2_H
#define RS2_H

#include "levels.h"
#include "../../../include/common.h"

struct rs2 {
    uint16_t initialised;
    int16_t menu_index;
    int16_t menu_selection_index;
    int16_t menu_enabled;
    uint16_t is_warping;
    level_data warp_selected_level;
    savestate_t savestate;
    CdlCB read_callback;
    int32_t button_holdtimes[16];
};

extern struct rs2 rs2;

void handle_input();
void patch_jump(int32_t *overwrite_loc, int32_t jump_loc);
void read_cb(unsigned char status, unsigned char *result);
void handle_warp();

#endif