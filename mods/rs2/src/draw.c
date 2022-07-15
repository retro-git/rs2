#include "game.h"
#include "draw.h"
#include "rs2.h"
#include "levels.h"
#include "libcd.h"
#include "libc.h"
#include "spyro.h"
#include "common.h"
#include "vec3.h"

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
                memcpy((void*)&draw_commands[i].d.draw_text_timeout_data, data, sizeof(draw_text_timeout_data_t));
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

    if (rs2.menu_enabled)
        draw_menu();
}

void draw_menu()
{
    char buffer[32];
    for (uint16_t i = 0; i < sizeof(levels_table) / sizeof(LevelData); i++)
    {
        LIBC_sprintf(buffer, "%s", levels_table[i].name);
        GAME_DrawText(buffer, i <= 14 ? 100 : 300, 40 + 10 * (i % 15), i == rs2.menu_selection_index ? 1 : 0, 0);
    }

    if (rs2.button_holdtimes[DUP] && rs2.button_holdtimes[DUP] % 2 == 0)
    {
        rs2.menu_selection_index == 0 ? rs2.menu_selection_index = sizeof(levels_table) / sizeof(LevelData) - 1 : rs2.menu_selection_index--;
    }
    else if (rs2.button_holdtimes[DDOWN] && rs2.button_holdtimes[DDOWN] % 2 == 0)
    {
        rs2.menu_selection_index == sizeof(levels_table) / sizeof(LevelData) - 1 ? rs2.menu_selection_index = 0 : rs2.menu_selection_index++;
    }
    else if (GAME_input.b.select)
    {
        rs2.menu_enabled = 0;
        begin_warp();
    }
}

void begin_warp()
{
    rs2.warp_selected_level = levels_table[rs2.menu_selection_index];
    GAME_pause_submenu_index = 0;
    GAME_unk_timer = 0x11;
    GAME_pause_menu_index = 5;
    GAME_gameState = PAUSED;
    *(int *)0x80069894 = 0xffff;
    *(int *)0x80069896 = 0xffff;

    if (rs2.warp_selected_level.type == HOMEWORLD)
    {
        switch (rs2.menu_selection_index)
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