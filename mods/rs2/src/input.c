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
#include "msg.h"

void read_input_hook()
{
    GAME_ReadInput();

    if (GAME_gameState != PLAYING)
        return;

    controller_u_t currentInput = GAME_inputStates[0].current;

    for (uint8_t i = 0; i < sizeof(rs2.button_holdtimes) / sizeof(uint16_t); i++)
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
            *(int16_t *)0x800698f0 = 0;
        }
        else if (rs2.button_holdtimes[SQUARE] == 1)
        {
            GAME_SaveCheckpoint((void *)0x80067414, &GAME_spyro.position, *(uint32_t *)0x80069ffe);
            savestate_draw_msg("Checkpoint saved");
        }
        else if (rs2.button_holdtimes[TRIANGLE] == 1)
        {
            rs2.savestate.state_saved = true;
            rs2.savestate.level = GAME_world_id;
            rs2.savestate.position = GAME_spyro.position;
            rs2.savestate.rotation = GAME_spyro.eulerRotations;
            rs2.savestate.cam_rotation = GAME_cam_rotation;
            rs2.savestate.cam_position = GAME_cam_position;
            savestate_draw_msg("Pos saved");
        }
        else if (rs2.button_holdtimes[CIRCLE] == 1 && rs2.savestate.level == GAME_world_id && rs2.savestate.state_saved)
        {
            GAME_spyro.position = rs2.savestate.position;
            GAME_spyro.eulerRotations = rs2.savestate.rotation;
            GAME_cam_rotation = rs2.savestate.cam_rotation;
            GAME_cam_position = rs2.savestate.cam_position;
        }
    }

    if (rs2.button_holdtimes[R3] == 1 || (currentInput.b.select && rs2.button_holdtimes[L2] == 1 || (currentInput.b.l2 && rs2.button_holdtimes[SELECT] == 1)))
    {
        rs2.menu_enabled = !rs2.menu_enabled;
        if (rs2.menu_enabled)
        {
            char *msg = messages[LIBC_rand() % NUM_MESSAGES];
            add_draw_command(DRAW_TEXT_TIMEOUT, &(draw_text_timeout_data_t){
                                                    .text = msg,
                                                    .x = FRAME_WIDTH / 2 - (GAME_GetTextWidth(msg) / 2),
                                                    .y = SCREEN_BOTTOM - 15,
                                                    .col = LIBC_rand() % 5,
                                                    .cur_time = 0,
                                                    .start_time = 0,
                                                    .end_time = 100,
                                                    .gameplay_should_draw = 0,
                                                });
        }
    }

    if (rs2.button_holdtimes[L3] == 1 && !rs2.frame_advance)
    {
        rs2.frame_advance = !rs2.frame_advance;
    }
    else if (rs2.button_holdtimes[SELECT] == 1 && rs2.frame_advance)
    {
        rs2.frame_advance = !rs2.frame_advance;
        GAME_inputStates[0].pressed.b.select = 0;
    }
    else if (currentInput.b.select)
    {
        if (currentInput.b.circle && GAME_world_id != DRAGON_SHORES)
        { // swim
            GAME_spyro.state = 0x29;
        }
        else if (currentInput.b.cross)
        { // moon jump
            // *(int16_t *)0x8006A08D = menus[OPTIONS_MENU1].d.options_table[MENU1_MOONJUMP_SPEED].d.option_number_data->number;
            *(int16_t *)0x8006A08D = 20;
        }
        else if (currentInput.b.triangle)
        { // normal
            GAME_spyro.state = 0;
        }
        /*else if (currentInput.b.square) { //supercharge
            GAME_spyro.state = 0x3d;
        }*/
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
        switch (menu->type)
        {
        case MENU_TYPE_TELEPORT:
            if (rs2.button_holdtimes[DLEFT] && rs2.button_holdtimes[DLEFT] % 3 == 0)
            {
                menu->menu_selection_index = menu->menu_selection_index >= 15 ? menu->menu_selection_index - 15 : menu->menu_selection_index + 15;
                // menu->menu_selection_index = menu->menu_selection_index >=15 ? menu->menu_selection_index - 15 : menu->menu_selection_index;
            }
            else if (rs2.button_holdtimes[DRIGHT] && rs2.button_holdtimes[DRIGHT] % 3 == 0)
            {
                menu->menu_selection_index = menu->menu_selection_index < 15 ? menu->menu_selection_index + 15 : menu->menu_selection_index - 15;
                // menu->menu_selection_index = menu->menu_selection_index < 15 ? menu->menu_selection_index + 15 : menu->menu_selection_index;
            }
            if (menu->menu_selection_index >= menus[rs2.menu_index].num_options)
            {
                menu->menu_selection_index = menus[rs2.menu_index].num_options - 1;
            }
            break;
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
                OptionData* data = &menu->d.options_table[menu->menu_selection_index];
                data->toggled = !data->toggled;
                if (data->oneshot) {
                    data->execute();
                }
                break;
            }
        }

        // if ((rs2.button_holdtimes[DRIGHT] && rs2.button_holdtimes[DRIGHT] % 2 == 0) || (rs2.button_holdtimes[DLEFT] && rs2.button_holdtimes[DLEFT] % 2 == 0))
        // {
        //     if (menu->type == MENU_TYPE_OPTIONS)
        //     {
        //         OptionData *option = &menu->d.options_table[menu->menu_selection_index];
        //         switch (option->type)
        //         {
        //         case OPTION_NUMBER:
        //             OptionNumberData *data = option->d.option_number_data;
        //             int16_t diff = currentInput.b.dright ? 1 : -1;
        //             if (data->number + diff < data->min)
        //                 data->number = data->max;
        //             else if (data->number + diff > data->max)
        //                 data->number = data->min;
        //             else
        //                 data->number += diff;
        //             break;
        //         }
        //     }
        // }
    }
}