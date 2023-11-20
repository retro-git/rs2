#ifndef DRAW_HOOK_H
#define DRAW_HOOK_H

#include "menus.h"

#define MAX_MENU_ITEMS_PER_COL 15

#define MENU_Y_COORD(index) (40 + 10 * (index % MAX_MENU_ITEMS_PER_COL))
#define MENU_X_COORD (FRAME_WIDTH / 3)
#define MENU_X_COORD_VALUE (FRAME_WIDTH / 3 + 160)

typedef struct {
    bool active;
    char* text;
    uint16_t x;
    uint16_t y;
    uint8_t col;
    uint16_t max_ticks;
    uint16_t ticks;
} draw_command_t;

void savestate_draw_msg(char *msg);
void draw_hook(unsigned int unk);
void draw_menu();
void begin_warp(uint8_t level_index);
void UpdateGame_Normal_hook();

extern LevelData levels_table[NUM_LEVELS];
extern draw_command_t active_draw_command;

#endif

