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

void init()
{
    // skip intro and load into glimmer
    if (GAME_gameState == 0xb)
    {
        *(uint8_t *)(0x80066f90) = 0xb;
        *(uint8_t *)(0x800698f4) = 3;
        *(uint8_t *)(0x80067e0c) = 5;
        rs2.initialised = true;
    }
}

void passive_effects()
{
    if (GAME_gameState != PLAYING)
        return;
    // moneybags costs 0 gems
    for (int i = 0; i < sizeof(GAME_moneybags_paywalls) / sizeof(MoneybagsPaywall); i++)
    {
        GAME_moneybags_paywalls[i].cost = 0;
    }
    // set flags for enter level cutscene being watched
    for (int i = 0; i < 0x1d; i++)
    {
        *(uint8_t *)(0x8006b084 + i) = 1;
    }
    //remove exit level cutscenes
    *(uint8_t*)0x80066ff4 = 0;
}

void read_input_hook()
{
    GAME_ReadInput();

    if (!rs2.initialised) init();

    passive_effects();

    if (GAME_gameState != PLAYING)
        return;

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
            *(int16_t *)0x800698f0 = 0;
        }
        else if (rs2.button_holdtimes[SQUARE] == 1)
        {
            GAME_SaveCheckpoint((void *)0x80067414, &GAME_spyro.position, *(uint32_t *)0x80069ffe);
            add_draw_command(DRAW_TEXT_TIMEOUT, &(draw_text_timeout_data_t){
                                                    .text = "Checkpoint saved",
                                                    .x = SCREEN_LEFT + 10,
                                                    .y = SCREEN_BOTTOM - 15,
                                                    .col = TEXTCOL_WHITE,
                                                    .cur_time = 0,
                                                    .start_time = 0,
                                                    .end_time = 30,
                                                    .gameplay_should_draw = 1,
                                                });
        }
        else if (rs2.button_holdtimes[TRIANGLE] == 1)
        {
            rs2.savestate.position = GAME_spyro.position;
            rs2.savestate.rotation = GAME_spyro.eulerRotations;
            rs2.savestate.cam_rotation = GAME_cam_rotation;
            rs2.savestate.cam_position = GAME_cam_position;

            add_draw_command(DRAW_TEXT_TIMEOUT, &(draw_text_timeout_data_t){
                                                    .text = "Position saved",
                                                    .x = SCREEN_LEFT + 10,
                                                    .y = SCREEN_BOTTOM - 15,
                                                    .col = TEXTCOL_WHITE,
                                                    .cur_time = 0,
                                                    .start_time = 0,
                                                    .end_time = 30,
                                                    .gameplay_should_draw = 1,
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
        if (rs2.menu_enabled) {
            char* msg = messages[LIBC_rand() % NUM_MESSAGES];
            add_draw_command(DRAW_TEXT_TIMEOUT, &(draw_text_timeout_data_t){
                                                    .text = msg,
                                                    .x = FRAME_WIDTH / 2 - (GAME_GetTextWidth(msg) / 2),
                                                    .y = SCREEN_BOTTOM - 15,
                                                    .col = LIBC_rand() % 5,
                                                    .cur_time = 0,
                                                    .start_time = 0,
                                                    .end_time = 30,
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
            *(int16_t *)0x8006A08D = menus[OPTIONS_MENU1].d.options_table[MENU1_MOONJUMP_SPEED].d.option_number_data->number;
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
        switch(menu->type) {
            case MENU_TYPE_TELEPORT:
                if (rs2.button_holdtimes[DLEFT] && rs2.button_holdtimes[DLEFT] % 3 == 0) {
                    menu->menu_selection_index = menu->menu_selection_index >= 15 ? menu->menu_selection_index - 15 : menu->menu_selection_index + 15;
                    //menu->menu_selection_index = menu->menu_selection_index >=15 ? menu->menu_selection_index - 15 : menu->menu_selection_index;
                } 
                else if (rs2.button_holdtimes[DRIGHT] && rs2.button_holdtimes[DRIGHT] % 3 == 0) {
                    menu->menu_selection_index = menu->menu_selection_index < 15 ? menu->menu_selection_index + 15 : menu->menu_selection_index - 15;
                    //menu->menu_selection_index = menu->menu_selection_index < 15 ? menu->menu_selection_index + 15 : menu->menu_selection_index;
                }
                if (menu->menu_selection_index >= menus[rs2.menu_index].num_options) {
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
                switch (menu->d.options_table[menu->menu_selection_index].type)
                {
                case OPTION_TOGGLE:
                    menu->d.options_table[menu->menu_selection_index].d.option_toggle_data->toggled = !menu->d.options_table[menu->menu_selection_index].d.option_toggle_data->toggled;
                    break;
                case OPTION_ONESHOT:
                    menu->d.options_table[menu->menu_selection_index].d.option_oneshot_data->execute();
                    break;
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
                    OptionNumberData *data = option->d.option_number_data;
                    int16_t diff = currentInput.b.dright ? 1 : -1;
                    if (data->number + diff < data->min)
                        data->number = data->max;
                    else if (data->number + diff > data->max)
                        data->number = data->min;
                    else
                        data->number += diff;
                    break;
                }
            }
        }
    }
}

LevelData levels_table[NUM_LEVELS]  = {
    { "Summer Forest", 0xa, HOMEWORLD },
    { "Glimmer", 0xb, STAGE },
    { "Idol Springs", 0xc, STAGE },
    { "Colossus", 0xd, STAGE },
    { "Hurricos", 0x15, STAGE },
    { "Aquaria Towers", 0x16, STAGE },
    { "Sunny Beach", 0x17, STAGE },
    { "Ocean Speedway", 0x19, STAGE },
    { "Crush", 0x1a, BOSS },
    { "Autumn Plains", 0x1e, HOMEWORLD },
    { "Skelos Badlands", 0x1f, STAGE },
    { "Crystal Glacier", 0x20, STAGE },
    { "Breeze Harbor", 0x21, STAGE },
    { "Zephyr", 0x22, STAGE },
    { "Metro Speedway", 0x23, STAGE },
    { "Scorch", 0x29, STAGE },
    { "Shady Oasis", 0x2a, STAGE },
    { "Magma Cone", 0x2b, STAGE },
    { "Fracture Hills", 0x2c, STAGE },
    { "Icy Speedway", 0x2d, STAGE },
    { "Gulp", 0x2e, BOSS },
    { "Winter Tundra", 0x32, HOMEWORLD },
    { "Mystic Marsh", 0x33, STAGE },
    { "Cloud Temples", 0x34, STAGE },
    { "Canyon Speedway", 0x37, STAGE },
    { "Robotica Farms", 0x3d, STAGE },
    { "Metropolis", 0x3e, STAGE },
    { "Dragon Shores", 0x41, STAGE },
    { "Ripto", 0x42, BOSS },
};