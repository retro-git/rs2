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

draw_command_t draw_commands[3] = {0};

void add_draw_command(DRAW_COMMAND_TYPE type, void *data)
{
    draw_command_t *command = draw_commands;
    for (uint8_t i = 0; i < sizeof(draw_commands) / sizeof(draw_command_t); i++, command++)
    {
        if (command->type == FREE_SLOT && type == DRAW_TEXT_TIMEOUT)
        {
            command->type = DRAW_TEXT_TIMEOUT;
            memcpy((void *)&command->d.draw_text_timeout_data, data, sizeof(draw_text_timeout_data_t));
            break;
        }
    }
}

void draw_hook(unsigned int unk)
{
    GAME_FUN_80013a14(unk);

    if (rs2.menu_enabled)
        draw_menu();

    for (uint8_t i = 0; i < sizeof(draw_commands) / sizeof(draw_command_t); i++)
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

    for (uint8_t i = 0; i < NUM_MENUS; i++)
    {
        if (menus[i].type != MENU_TYPE_OPTIONS)
            continue;
        for (uint8_t j = 0; j < menus[i].num_options; j++)
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

void draw_option(MenuData *menu, OptionData *option, uint16_t i, char *buffer) {
    LIBC_sprintf(buffer, "%s", option->name);
    GAME_DrawText(buffer, MENU_X_COORD, MENU_Y_COORD(i), i == menu->menu_selection_index ? TEXTCOL_LIGHT_YELLOW : TEXTCOL_DARK_YELLOW, 0);

    if (option->type == OPTION_TOGGLE) {
        char *text = option->d.option_toggle_data->toggled ? "ON" : "OFF";
        GAME_DrawText(text, MENU_X_COORD_VALUE, MENU_Y_COORD(i), (option->d.option_toggle_data->toggled == 1 ? TEXTCOL_GREEN : TEXTCOL_RED), 0);
    }
    // } else if (option->type == OPTION_NUMBER) {
    //     OptionNumberData *data = option->d.option_number_data;
    //     data->names != 0 ? LIBC_sprintf(buffer, "%s", data->names[data->number]) : LIBC_sprintf(buffer, "%d", data->number);
    //     GAME_DrawText(buffer, MENU_X_COORD_VALUE, MENU_Y_COORD(i), TEXTCOL_DARK_YELLOW, 0);
    // }
}

void draw_menu() {
    char buffer[32];
    MenuData *menu = &menus[rs2.menu_index];

    GAME_DrawOutlinedBG(0 - 5, FRAME_WIDTH, SCREEN_TOP - 5, SCREEN_BOTTOM);
    GAME_DrawText(menu->title, FRAME_WIDTH / 2 - GAME_GetTextWidth(menu->title) / 2, SCREEN_TOP + 10, TEXTCOL_LIGHT_YELLOW, 0);

    for (uint8_t i = 0; i < menu->num_options; i++) {
        switch (menu->type) {
            case MENU_TYPE_TELEPORT:
                LIBC_sprintf(buffer, "%s", menu->d.levels_table[i].name);
                GAME_DrawText(buffer, i < MAX_MENU_ITEMS_PER_COL ? 100 : 300, MENU_Y_COORD(i), i == menu->menu_selection_index ? TEXTCOL_LIGHT_YELLOW : TEXTCOL_DARK_YELLOW, 0);
                break;
            case MENU_TYPE_OPTIONS:
                draw_option(menu, &menu->d.options_table[i], i, buffer);
                break;
        }
    }
}

void begin_warp(uint8_t level_index)
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

LevelData levels_table[NUM_LEVELS] = {
    {(char *)0x800106e0, 0xa, HOMEWORLD},  // summer
    {(char *)0x80066ea0, 0xb, STAGE},      // glimmer
    {(char *)0x800106d0, 0xc, STAGE},      // idol
    {(char *)0x800106c4, 0xd, STAGE},      // colossus
    {(char *)0x800106b8, 0x15, STAGE},     // hurricos
    {(char *)0x800106a8, 0x16, STAGE},     // aquaria
    {(char *)0x8001069c, 0x17, STAGE},     // sunny beach
    {(char *)0x8001068c, 0x19, STAGE},     // ocean speedway
    {(char *)0x8001067c, 0x1a, BOSS},      // crush
    {(char *)0x8001066c, 0x1e, HOMEWORLD}, // ap
    {(char *)0x8001065c, 0x1f, STAGE},     // skelos
    {(char *)0x8001064c, 0x20, STAGE},     // crystal
    {(char *)0x8001063c, 0x21, STAGE},     // breeze
    {(char *)0x80066e98, 0x22, STAGE},     // zephyr
    {(char *)0x8001062c, 0x23, STAGE},     // metro
    {(char *)0x80066e90, 0x29, STAGE},     // scorch
    {(char *)0x80010620, 0x2a, STAGE},     // shady
    {(char *)0x80010614, 0x2b, STAGE},     // magma
    {(char *)0x80010604, 0x2c, STAGE},     // fracture
    {(char *)0x800105f4, 0x2d, STAGE},     // icy
    {(char *)0x800105e4, 0x2e, BOSS},      // gulp
    {(char *)0x800105d4, 0x32, HOMEWORLD}, // winter tundra
    {(char *)0x800105c4, 0x33, STAGE},     // mystic
    {(char *)0x800105b4, 0x34, STAGE},     // cloud
    {(char *)0x800105a4, 0x37, STAGE},     // canyon
    {(char *)0x80010594, 0x3d, STAGE},     // robotica
    {(char *)0x80010588, 0x3e, STAGE},     // metropolis
    {(char *)0x80010578, 0x41, STAGE},     // dragon shores
    {(char *)0x80010568, 0x42, BOSS},      // ripto
};