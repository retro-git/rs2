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

    if (rs2.menu_enabled)
        draw_menu();

    for (uint16_t i = 0; i < sizeof(draw_commands) / sizeof(draw_command_t); i++)
    {
        switch (draw_commands[i].type)
        {
        case DRAW_TEXT_TIMEOUT:
            draw_text_timeout_data_t *data = &draw_commands[i].d.draw_text_timeout_data;
            if (data->cur_time > data->start_time && data->cur_time < data->end_time || data->start_time == 0 && data->end_time == 0)
            {
                if (!data->gameplay_should_draw && !rs2.menu_enabled) {
                    draw_commands[i].type = FREE_SLOT;
                    continue;
                };
                GAME_DrawText(data->text, data->x, data->y, data->col, 0);
            }
            if (data->start_time != data->end_time) data->cur_time += 1;
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

    for (uint16_t i = 0; i < NUM_MENUS; i++)
    {
        if (menus[i].type != MENU_TYPE_OPTIONS)
            continue;
        for (uint16_t j = 0; j < menus[i].num_options; j++)
        {
            if (menus[i].d.options_table[j].type == OPTION_TOGGLE)
            {
                OptionToggleData *data = menus[i].d.options_table[j].d.option_toggle_data;
                if (data->toggled)
                {
                    data->execute();
                }
            }
        }
    }

    /*for (int i = 0; ; i++) {
        Moby* current_moby = GAME_moby_array_pointer + (i * sizeof(Moby));

        if (current_moby->objectTypeID == 0x0078) {
            LIBC_printf("SPARX: %d  %d  %d\n", current_moby->position.x, current_moby->position.y, current_moby->position.z);
            break;
        }
    }*/
}

void draw_menu()
{
    char buffer[32];
    MenuData *menu = &menus[rs2.menu_index];

    GAME_DrawOutlinedBG(0 - 5, FRAME_WIDTH, SCREEN_TOP - 5, SCREEN_BOTTOM);
    GAME_DrawText(menu->title, FRAME_WIDTH / 2 - GAME_GetTextWidth(menu->title) / 2, SCREEN_TOP + 10, TEXTCOL_LIGHT_YELLOW, 0);

    switch (menu->type)
    {
    case MENU_TYPE_TELEPORT:
        LevelData *levels_table = menus[rs2.menu_index].d.levels_table;
        for (uint16_t i = 0; i < menu->num_options; i++)
        {
            LIBC_sprintf(buffer, "%s", levels_table[i].name);
            GAME_DrawText(buffer, i < MAX_MENU_ITEMS_PER_COL ? 100 : 300, MENU_Y_COORD(i), i == menu->menu_selection_index ? TEXTCOL_LIGHT_YELLOW : TEXTCOL_DARK_YELLOW, 0);
        }
        break;
    case MENU_TYPE_OPTIONS:
        OptionData *options_table = menus[rs2.menu_index].d.options_table;
        for (uint16_t i = 0; i < menu->num_options; i++)
        {
            OptionData *option = &options_table[i];
            LIBC_sprintf(buffer, "%s", options_table[i].name);

            GAME_DrawText(buffer, MENU_X_COORD, MENU_Y_COORD(i), i == menu->menu_selection_index ? TEXTCOL_LIGHT_YELLOW : TEXTCOL_DARK_YELLOW, 0);
            switch (option->type)
            {
            case OPTION_TOGGLE:
            {
                char *text = option->d.option_toggle_data->toggled ? "ON" : "OFF";
                GAME_DrawText(text, MENU_X_COORD_VALUE, MENU_Y_COORD(i), (option->d.option_toggle_data->toggled == 1 ? TEXTCOL_GREEN : TEXTCOL_RED), 0);
            }
            break;
            case OPTION_NUMBER:
            {
                OptionNumberData *data = option->d.option_number_data;
                data->names != 0 ? LIBC_sprintf(buffer, "%s", data->names[data->number]) : LIBC_sprintf(buffer, "%d", data->number);
                GAME_DrawText(buffer, MENU_X_COORD_VALUE, MENU_Y_COORD(i), TEXTCOL_DARK_YELLOW, 0);
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

void DrawRectST(short left, short right, short top, short bottom, Color color)
{
    POLY_F4 *ptrPrimitive = (POLY_F4 *)GAME_GPUPackets_Next;
    ptrPrimitive->tag = 0x5000000;
    ptrPrimitive->code = 0x2a;

    ptrPrimitive->x0 = left;
    ptrPrimitive->x2 = left;

    ptrPrimitive->x1 = right;
    ptrPrimitive->x3 = right;

    ptrPrimitive->y0 = top;
    ptrPrimitive->y1 = top;

    ptrPrimitive->y2 = bottom;
    ptrPrimitive->y3 = bottom;

    ptrPrimitive->r0 = color.r;
    ptrPrimitive->g0 = color.g;
    ptrPrimitive->b0 = color.b;

    GAME_GPUPackets_Insert(ptrPrimitive);
    GAME_GPUPackets_Next = ptrPrimitive + 1;
}