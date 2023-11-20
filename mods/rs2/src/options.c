#include "options.h"
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

// #define OPTION_TOGGLE_ENABLED(menu, option) (menus[menu].d.options_table[option].d.option_toggle_data->toggled)

// void oneshot_toggle_message(char toggled, uint32_t option_index)
// {
//     add_draw_command(DRAW_TEXT_TIMEOUT, &(draw_text_timeout_data_t){
//                                             .text = "DONE",
//                                             .x = MENU_X_COORD_VALUE,
//                                             .y = MENU_Y_COORD(option_index),
//                                             .col = toggled ? TEXTCOL_GREEN : TEXTCOL_RED,
//                                             .cur_time = 0,
//                                             .start_time = 0,
//                                             .end_time = 15,
//                                             .gameplay_should_draw = 0,
//                                         });
// }

void option_instant_fish_execute()
{
    if (GAME_world_id == IDOL_SPRINGS)
    {
        GAME_idol_fish_status = 0xa;
    }
}

void option_reset_checkpoint_execute()
{
    GAME_zoe_checkpoint_active = 0;
}

void option_powerups_alwayson_execute() {
    GAME_enemies_killed_since_death = 0x30;
}

void option_toggle_zombie_execute() {
    GAME_sparx_health = -1;
    // oneshot_toggle_message(GAME_sparx_health == -1, MENU1_ZOMBIE);
   // GAME_sparx_health >= 0 ? GAME_sparx_health = -1 : GAME_sparx_health = 3;
}

void option_reset_gems_execute()
{
    LIBC_bzero(&GAME_gems_collected_status, 0x356);
}

void option_reset_orbs_execute()
{
    LIBC_bzero(&GAME_orbs_collected_status, 0x1a);
}

void option_reset_eol_portals_execute() {
    LIBC_bzero(&GAME_end_level_portals_status, 0x1c);
    GAME_num_talismans = 0;
}

void option_infinite_lives_execute() {
    *(int16_t*)0x80032fe4 ^= -1;
    // oneshot_toggle_message(*(int16_t*)0x80032fe4 == 0, MENU1_LOCK_LIVES);
}

void option_satyrless_tools_execute()
{
    if (GAME_world_id == FRACTURE_HILLS)
    {
        if (GAME_inputStates[0].current.b.l2 && GAME_inputStates[0].current.b.r2 && rs2.button_holdtimes[CROSS] == 1)
        {
            GAME_fracture_end_faun_state = 1;
        }
        char *text = GAME_fracture_end_faun_state == 1 ? "Unloaded" : "Loaded";
        GAME_DrawText(text, SCREEN_LEFT + 1, SCREEN_TOP + 1, GAME_fracture_end_faun_state == 1 ? TEXTCOL_GREEN : TEXTCOL_RED, 0);
    }
}

void option_toggle_fireball_execute()
{
    GAME_fireball_state = !GAME_fireball_state;
    // oneshot_toggle_message(GAME_fireball_state, MENU1_FIREBALL);
}

void option_toggle_sparx_extended_range_execute()
{
    GAME_sparx_extended_range_state = !GAME_sparx_extended_range_state;
    // oneshot_toggle_message(GAME_sparx_extended_range_state, MENU1_SPARX_ER);
}

void option_input_display_execute()
{
    InputState *currentInput = &GAME_inputStates[0];

    GAME_GPUChangeTexPage(0xa8);

    struct Button {
        uint8_t x1, x2, y1, y2;
        uint8_t identifier;
    };

    struct Button buttons[] = {
        {4, 16, FRAME_HEIGHT - 23, FRAME_HEIGHT - 23 + 4, L2},
        {2, 18, FRAME_HEIGHT - 23 + 5, FRAME_HEIGHT - 23 + 9, L1},
        {20 + 4, 20 + 16, FRAME_HEIGHT - 23, FRAME_HEIGHT - 23 + 4, R2},
        {20 + 2, 20 + 18, FRAME_HEIGHT - 23 + 5, FRAME_HEIGHT - 23 + 9, R1},
        {7, 13, FRAME_HEIGHT - 23 + 10, FRAME_HEIGHT - 23 + 14, DUP},
        {13, 19, FRAME_HEIGHT - 23 + 14, FRAME_HEIGHT - 23 + 18, DRIGHT},
        {7, 13, FRAME_HEIGHT - 23 + 18, FRAME_HEIGHT - 23 + 22, DDOWN},
        {1, 7, FRAME_HEIGHT - 23 + 14, FRAME_HEIGHT - 23 + 18, DLEFT},
        {20 + 7, 20 + 13, FRAME_HEIGHT - 23 + 10, FRAME_HEIGHT - 23 + 14, TRIANGLE},
        {20 + 13, 20 + 19, FRAME_HEIGHT - 23 + 14, FRAME_HEIGHT - 23 + 18, CIRCLE},
        {20 + 7, 20 + 13, FRAME_HEIGHT - 23 + 18, FRAME_HEIGHT - 23 + 22, CROSS},
        {20 + 1, 20 + 7, FRAME_HEIGHT - 23 + 14, FRAME_HEIGHT - 23 + 18, SQUARE}
    };

    // *((uint8_t*)0x8004d2dc) = 0;
    // *((uint8_t*)0x8004d2dd) = 0x2a;
    // *((uint8_t*)0x8004d2e0) = 0;
    // *((uint8_t*)0x8004d2e1) = 0;

    for (int i = 0; i < sizeof(buttons) / sizeof(buttons[0]); i++) {
        if (rs2.button_holdtimes[buttons[i].identifier] > 0) {
            GAME_DrawOpaqueBlackRect(buttons[i].x1, buttons[i].x2, buttons[i].y1, buttons[i].y2);
        }
        else {
            GAME_DrawSTBlackRect(buttons[i].x1, buttons[i].x2, buttons[i].y1, buttons[i].y2);
        }
    }

    // sticks:
    
    // Color colorPressed;
    // colorPressed.r = 0xff;
    // colorPressed.g = 0xff;
    // colorPressed.b = 0xff;
    // uint8_t stickX = currentInput->rightStickAnalogX;
    // uint8_t stickY = currentInput->rightStickAnalogY;
    // uint8_t offset = 30;
    // for (uint8_t j = 0; j < 2; j++) {
    //     GAME_DrawShadowedOrangeLine(offset, FRAME_HEIGHT - 23 + 16, offset + (stickX - 0x7f) * 9 / 0x80, FRAME_HEIGHT - 23 + 16 + (stickY - 0x7f) * 6 / 0x80);
    //     offset = 10;
    //     stickX = currentInput->leftStickAnalogX;
    //     stickY = currentInput->leftStickAnalogY;
    // }
}