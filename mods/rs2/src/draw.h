#ifndef DRAW_HOOK_H
#define DRAW_HOOK_H

typedef enum {
    FREE_SLOT = 0,
    DRAW_TEXT_TIMEOUT = 1,
} DRAW_COMMAND_TYPE;

typedef struct {
    char* text;
    uint16_t x;
    uint16_t y;
    uint16_t col;
    uint16_t cur_time;
    uint16_t start_time;
    uint16_t end_time;
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
void begin_warp();
void DrawLine(short x0, short y0, Color c0, short x1, short y1, Color c1);

#endif