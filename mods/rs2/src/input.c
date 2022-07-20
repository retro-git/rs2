#include "game.h"
#include "draw.h"
#include "rs2.h"
#include "menus.h"
#include "libcd.h"
#include "libc.h"
#include "spyro.h"
#include "common.h"
#include "vec3.h"
#include "gpu.h"

void read_input_hook()
{
    GAME_ReadInput();

    if (GAME_gameState != PLAYING) return;

    controller_u_t currentInput = GAME_inputStates[0].current;

    for (uint16_t i = 0; i < sizeof(rs2.button_holdtimes) / sizeof(int32_t); i++)
    {
        if (currentInput.i >> i & 1)
        {
            rs2.button_holdtimes[i]++;
        }
        else
        {
            rs2.button_holdtimes[i] = 0;
        }
    }

    if (currentInput.b.l2 && currentInput.b.r2)
    {
        if (rs2.button_holdtimes[START] == 1)
        {
           /* for (uint16_t i = 0; i < NUM_OPTIONS_MENU2_RESPAWN; i++)
            {
                if (menus[OPTIONS_MENU2_RESPAWN].d.options_table[i].type == OPTION_TOGGLE)
                {
                    OptionToggleData *data = menus[OPTIONS_MENU2_RESPAWN].d.options_table[i].d.option_toggle_data;
                    if (data->toggled)
                    {
                        data->execute();
                    }
                }
            }*/
            GAME_pause_submenu_index = 0;
            GAME_unk_timer = 0;
            GAME_gameState = TRANSITION_LOAD_TO_PLAYING;
            *(int16_t*)0x800698f0 = 0;
        }
        else if (rs2.button_holdtimes[TRIANGLE] == 1)
        {
            rs2.savestate.position = GAME_spyro.position;
            rs2.savestate.rotation = GAME_spyro.eulerRotations;
            rs2.savestate.cam_rotation = GAME_cam_rotation;
            rs2.savestate.cam_position = GAME_cam_position;

            add_draw_command(DRAW_TEXT_TIMEOUT, &(draw_text_timeout_data_t){
                                                    .text = "Saved",
                                                    .x = 80,
                                                    .y = 80,
                                                    .col = 4,
                                                    .cur_time = 0,
                                                    .start_time = 0,
                                                    .end_time = 60,
                                                });
        }
        else if (rs2.button_holdtimes[CIRCLE] == 1)
        {
            GAME_spyro.position = rs2.savestate.position;
            GAME_spyro.eulerRotations = rs2.savestate.rotation;
            GAME_cam_rotation = rs2.savestate.cam_rotation;
            GAME_cam_position = rs2.savestate.cam_position;
        }
    }

    if (rs2.button_holdtimes[R3] == 1)
    {
        rs2.menu_enabled = !rs2.menu_enabled;
    }

    if (rs2.button_holdtimes[L3] == 1 && !rs2.frame_advance)
    {
        rs2.frame_advance = !rs2.frame_advance;
    }
    else if (rs2.button_holdtimes[SELECT] == 1)
    {
        rs2.frame_advance = !rs2.frame_advance;
        GAME_inputStates[0].pressed.b.select = 0;
    }

    if (rs2.menu_enabled)
    {
        MenuData *menu = &menus[rs2.menu_index];
        if (rs2.button_holdtimes[R2] == 1)
        {
            rs2.menu_index = (rs2.menu_index + 1) % (NUM_MENUS);
        }
        else if (rs2.button_holdtimes[L2] == 1)
        {
            rs2.menu_index = (rs2.menu_index + (NUM_MENUS - 1)) % (NUM_MENUS);
        }
        else if (rs2.button_holdtimes[DUP] && rs2.button_holdtimes[DUP] % 2 == 0)
        {
            menu->menu_selection_index == 0 ? menu->menu_selection_index = menus[rs2.menu_index].num_options - 1 : menu->menu_selection_index--;
        }
        else if (rs2.button_holdtimes[DDOWN] && rs2.button_holdtimes[DDOWN] % 2 == 0)
        {
            menu->menu_selection_index == menus[rs2.menu_index].num_options - 1 ? menu->menu_selection_index = 0 : menu->menu_selection_index++;
        }

        if (rs2.button_holdtimes[CROSS] == 1)
        {
            switch (menu->type)
            {
            case MENU_TYPE_TELEPORT:
                rs2.menu_enabled = 0;
                begin_warp(menu->menu_selection_index);
                break;

            case MENU_TYPE_OPTIONS:
                switch (menu->d.options_table[menu->menu_selection_index].type)
                {
                case OPTION_TOGGLE:
                    menu->d.options_table[menu->menu_selection_index].d.option_toggle_data->toggled = !menu->d.options_table[menu->menu_selection_index].d.option_toggle_data->toggled;
                    break;
                case OPTION_ONESHOT:
                    menu->d.options_table[menu->menu_selection_index].d.option_oneshot_data->execute();
                }
                break;
            }
        }

        if ((rs2.button_holdtimes[DRIGHT] && rs2.button_holdtimes[DRIGHT] % 2 == 0) || (rs2.button_holdtimes[DLEFT] && rs2.button_holdtimes[DLEFT] % 2 == 0))
        {
            if (menu->type == MENU_TYPE_OPTIONS)
            {
                OptionData *option = &menu->d.options_table[menu->menu_selection_index];
                switch (option->type)
                {
                case OPTION_NUMBER:
                    option->d.option_number_data->number = currentInput.b.dright ? option->d.option_number_data->number + 1 : option->d.option_number_data->number - 1;
                    break;
                }
            }
        }
    }
}