#ifndef DRAW_HOOK_H
#define DRAW_HOOK_H

#include "menus.h"

#define MAX_MENU_ITEMS_PER_COL 15

#define MENU_Y_COORD(index) (40 + 10 * (index % MAX_MENU_ITEMS_PER_COL))
#define MENU_X_COORD (FRAME_WIDTH / 3)
#define MENU_X_COORD_VALUE (FRAME_WIDTH / 3 + 160)

typedef enum {
    FREE_SLOT = 0,
    DRAW_TEXT_TIMEOUT = 1,
} DRAW_COMMAND_TYPE;

typedef struct {
    char* text;
    uint16_t x;
    uint16_t y;
    uint8_t col;
    uint16_t cur_time;
    uint16_t start_time;
    uint16_t end_time;
    bool gameplay_should_draw;
} draw_text_timeout_data_t;

typedef struct {
    DRAW_COMMAND_TYPE type;
    union {
        draw_text_timeout_data_t draw_text_timeout_data;
    } d;
} draw_command_t;

void add_draw_command(DRAW_COMMAND_TYPE type, void *data);
void draw_hook(unsigned int unk);
void draw_menu();
void begin_warp(uint8_t level_index);
void DrawLine(short x0, short y0, Color c0, short x1, short y1, Color c1);

extern LevelData levels_table[NUM_LEVELS];

#endif

