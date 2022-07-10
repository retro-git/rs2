#ifndef RS2_H
#define RS2_H

#include "levels.h"

struct rs2 {
    uint16_t initialised;
    int menu_index;
    int menu_selection_index;
    int menu_enabled;
    uint16_t is_warping;
    level_data warp_selected_level;
    savestate_t savestate;
    CdlCB read_callback;
};

extern struct rs2 rs2;

void draw_menu();
void begin_warp();
void handle_warp();

#endif