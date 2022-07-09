#ifndef RS2_H
#define RS2_H

#include "levels.h"

struct rs2 {
    int menu_index;
    int menu_selection_index;
    int menu_enabled;
    int is_warping;
    level_data warp_selected_level;
    savestate_t savestate;
};

extern struct rs2 rs2;

void draw_menu();
void begin_warp();
void handle_warp();

#endif