#ifndef RET2S2_H
#define RET2S2_H

#include "levels.h"

struct ret2s2 {
    int menu_index;
    int menu_selection_index;
    int menu_enabled;
    int is_warping;
    level_data warp_selected_level;
};

extern struct ret2s2 ret2s2;

void draw_menu();
void begin_warp();
void handle_warp();

#endif