#include "game.h"
#include "draw.h"
#include "rs2.h"
#include "libcd.h"
#include "libc.h"
#include "spyro.h"
#include "common.h"
#include "vec3.h"
#include "gpu.h"
#include "menus.h"

#define FRAME_WIDTH 512
#define FRAME_HEIGHT 228

#define SCREEN_LEFT 0
#define SCREEN_RIGHT 512
#define SCREEN_TOP 12
#define SCREEN_BOTTOM 228

#define LINE_HEIGHT 12

draw_command_t draw_commands[4] = {0};

void add_draw_command(DRAW_COMMAND_TYPE type, void *data)
{
    for (uint16_t i = 0; i < sizeof(draw_commands) / sizeof(draw_command_t); i++)
    {
        if (draw_commands[i].type == FREE_SLOT)
        {
            switch (type)
            {
            case DRAW_TEXT_TIMEOUT:
                draw_commands[i].type = DRAW_TEXT_TIMEOUT;
                memcpy((void *)&draw_commands[i].d.draw_text_timeout_data, data, sizeof(draw_text_timeout_data_t));
                break;
            default:
                break;
            }
        }
    }
}

void draw_hook(unsigned int unk)
{
    GAME_FUN_80013a14(unk);

    for (uint16_t i = 0; i < sizeof(draw_commands) / sizeof(draw_command_t); i++)
    {
        switch (draw_commands[i].type)
        {
        case DRAW_TEXT_TIMEOUT:
            draw_text_timeout_data_t *data = &draw_commands[i].d.draw_text_timeout_data;
            if (data->cur_time > data->start_time && data->cur_time < data->end_time)
            {
                GAME_DrawText(data->text, data->x, data->y, data->col, 0);
            }
            data->cur_time += 1;
            if (data->cur_time > data->end_time)
            {
                draw_commands[i].type = FREE_SLOT;
            }
            break;
        case FREE_SLOT:
            break;
        default:
            break;
        }
    }

    // DrawLine(100, 50, col, 200, 100, col);

    if (rs2.menu_enabled)
        draw_menu();
}

void draw_menu()
{
    char buffer[32];
    MenuData* menu = &menus[rs2.menu_index];
    switch (menu->type)
    {
    case TELEPORT_MENU:
        LevelData* levels_table = menus[rs2.menu_index].d.levels_table;
        for (uint16_t i = 0; i < menu->num_options; i++)
        {
            LIBC_sprintf(buffer, "%s", levels_table[i].name);
            GAME_DrawText(buffer, i <= 14 ? 100 : 300, 40 + 10 * (i % 15), i == menu->menu_selection_index ? 1 : 0, 0);
        }
        break;
    case OPTIONS_MENU:
        OptionData* options_table = menus[rs2.menu_index].d.options_table;
        for (uint16_t i = 0; i < menu->num_options; i++)
        {
            OptionData* option = &options_table[i];
            LIBC_sprintf(buffer, "%s", options_table[i].name);

            uint32_t x = i <= 14 ? 100 : 300;
            uint32_t y = 40 + 10 * (i % 15);

            GAME_DrawText(buffer, x, y, i == menu->menu_selection_index ? 1 : 0, 0);
            switch (option->type)
            {
                case OPTION_TOGGLE: {
                    GAME_DrawText(option->d.toggled ? "ON" : "OFF", x + 80, y, (option->d.toggled == 1 ? TEXTCOL_GREEN : TEXTCOL_RED), 0);
                }
                break;
                case OPTION_NUMBER: {
                    LIBC_sprintf(buffer, "%d", option->d.number);
                    GAME_DrawText(buffer, x + 80, y, 0, 0);
                    break;
                }
            }
        }
        break;
    }
}

void begin_warp(uint16_t level_index)
{
   // rs2.warp_selected_level = levels_table[rs2.menu_selection_index];
    rs2.warp_selected_level = menus[rs2.menu_index].d.levels_table[level_index];
    GAME_pause_submenu_index = 0;
    GAME_unk_timer = 0x11;
    GAME_pause_menu_index = 5;
    GAME_gameState = PAUSED;
    *(int *)0x80069894 = 0xffff;
    *(int *)0x80069896 = 0xffff;

    if (rs2.warp_selected_level.type == HOMEWORLD)
    {
        switch (level_index)
        {
        case SUMMER_FOREST:
            GAME_world_id = IDOL_SPRINGS;
            break;
        case AUTUMN_PLAINS:
            GAME_world_id = SKELOS_BADLANDS;
            break;
        case WINTER_TUNDRA:
            GAME_world_id = DRAGON_SHORES;
            break;
        }
    }

    rs2.is_warping = 1;
}

void DrawLine(short x0, short y0, Color c0, short x1, short y1, Color c1)
{
    LINE_G2 *ptrPrimitive = (LINE_G2 *)GAME_GPUPackets_Next;
    ptrPrimitive->tag = 0x4000000;
    ptrPrimitive->code = 0x50;

    ptrPrimitive->x0 = x0;
    ptrPrimitive->y0 = y0;
    ptrPrimitive->r0 = c0.r;
    ptrPrimitive->g0 = c0.g;
    ptrPrimitive->b0 = c0.b;

    ptrPrimitive->x1 = x1;
    ptrPrimitive->y1 = y1;
    ptrPrimitive->r1 = c1.r;
    ptrPrimitive->g1 = c1.g;
    ptrPrimitive->b1 = c1.b;

    GAME_GPUPackets_Insert(ptrPrimitive);
    GAME_GPUPackets_Next = ptrPrimitive + 1;
}