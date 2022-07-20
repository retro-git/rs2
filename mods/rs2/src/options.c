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

#define OPTION_TOGGLE_ENABLED(menu, option) (menus[menu].d.options_table[option].d.option_toggle_data->toggled)

/*void option_infinite_lives_execute() {
    if (OPTION_TOGGLE_ENABLED(OPTIONS_MENU1, MENU1_INFINITE_LIVES)) {
        GAME_num_lives = 99;
    }
}*/

void oneshot_toggle_message(char toggled, uint32_t option_index)
{
    add_draw_command(DRAW_TEXT_TIMEOUT, &(draw_text_timeout_data_t){
                                            .text = "DONE",
                                            .x = MENU_X_COORD_VALUE,
                                            .y = MENU_Y_COORD(option_index),
                                            .col = toggled ? TEXTCOL_GREEN : TEXTCOL_RED,
                                            .cur_time = 0,
                                            .start_time = 0,
                                            .end_time = 15,
                                        });
}

void option_instant_fish_execute()
{
    if (GAME_world_id == IDOL_SPRINGS)
    {
        GAME_idol_fish_status = 0xa;
    }
}

// void option_reset_checkpoint_execute()
// {
//     GAME_zoe_checkpoint_active = 0;
// }

void option_powerups_alwayson_execute() {
    GAME_enemies_killed_since_death = 0x30;
}

void option_reset_gems_execute()
{
    LIBC_bzero(&GAME_gems_collected_status, 0x356);
}

void option_reset_orbs_execute()
{
    LIBC_bzero(&GAME_orbs_collected_status, 0x1a);
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
    oneshot_toggle_message(GAME_fireball_state, MENU1_FIREBALL);
}

void option_toggle_sparx_extended_range_execute()
{
    GAME_sparx_extended_range_state = !GAME_sparx_extended_range_state;
    oneshot_toggle_message(GAME_sparx_extended_range_state, MENU1_SPARX_ER);
}

void option_input_display_execute()
{
    InputState *currentInput = &GAME_inputStates[0];

    Color colorUnpressed;
    colorUnpressed.r = 0x20;
    colorUnpressed.g = 0x20;
    colorUnpressed.b = 0x20;

    Color colorPressed;
    colorPressed.r = 0x80;
    colorPressed.g = 0x80;
    colorPressed.b = 0x80;

    GAME_GPUChangeTexPage(0xa8);
    DrawRectST(4, 16, FRAME_HEIGHT - 23, FRAME_HEIGHT - 23 + 4, currentInput->current.b.l2 ? colorPressed : colorUnpressed);
    DrawRectST(2, 18, FRAME_HEIGHT - 23 + 5, FRAME_HEIGHT - 23 + 9, currentInput->current.b.l1 ? colorPressed : colorUnpressed);

    DrawRectST(20 + 4, 20 + 16, FRAME_HEIGHT - 23, FRAME_HEIGHT - 23 + 4, currentInput->current.b.r2 ? colorPressed : colorUnpressed);
    DrawRectST(20 + 2, 20 + 18, FRAME_HEIGHT - 23 + 5, FRAME_HEIGHT - 23 + 9, currentInput->current.b.r1 ? colorPressed : colorUnpressed);

    DrawRectST(7, 13, FRAME_HEIGHT - 23 + 10, FRAME_HEIGHT - 23 + 14, currentInput->current.b.dup ? colorPressed : colorUnpressed);
    DrawRectST(13, 19, FRAME_HEIGHT - 23 + 14, FRAME_HEIGHT - 23 + 18, currentInput->current.b.dright ? colorPressed : colorUnpressed);
    DrawRectST(7, 13, FRAME_HEIGHT - 23 + 18, FRAME_HEIGHT - 23 + 22, currentInput->current.b.ddown ? colorPressed : colorUnpressed);
    DrawRectST(1, 7, FRAME_HEIGHT - 23 + 14, FRAME_HEIGHT - 23 + 18, currentInput->current.b.dleft ? colorPressed : colorUnpressed);

    colorPressed.r = 0x00;
    colorPressed.g = 0x80;
    colorPressed.b = 0x00;
    DrawRectST(20 + 7, 20 + 13, FRAME_HEIGHT - 23 + 10, FRAME_HEIGHT - 23 + 14, currentInput->current.b.triangle ? colorPressed : colorUnpressed);
    colorPressed.r = 0x80;
    colorPressed.g = 0x00;
    colorPressed.b = 0x00;
    DrawRectST(20 + 13, 20 + 19, FRAME_HEIGHT - 23 + 14, FRAME_HEIGHT - 23 + 18, currentInput->current.b.circle ? colorPressed : colorUnpressed);
    colorPressed.r = 0x00;
    colorPressed.g = 0x20;
    colorPressed.b = 0x80;
    DrawRectST(20 + 7, 20 + 13, FRAME_HEIGHT - 23 + 18, FRAME_HEIGHT - 23 + 22, currentInput->current.b.cross ? colorPressed : colorUnpressed);
    colorPressed.r = 0x80;
    colorPressed.g = 0x00;
    colorPressed.b = 0x80;
    DrawRectST(20 + 1, 20 + 7, FRAME_HEIGHT - 23 + 14, FRAME_HEIGHT - 23 + 18, currentInput->current.b.square ? colorPressed : colorUnpressed);

    colorUnpressed.r = 0x20;
    colorUnpressed.g = 0x20;
    colorUnpressed.b = 0x20;
    colorPressed.r = 0xff;
    colorPressed.g = 0xff;
    colorPressed.b = 0xff;
    DrawLine(
        20 + 10, FRAME_HEIGHT - 23 + 16, colorUnpressed,
        20 + 10 + (currentInput->rightStickAnalogX - 0x7f) * 9 / 0x80, FRAME_HEIGHT - 23 + 16 + (currentInput->rightStickAnalogY - 0x7f) * 6 / 0x80, colorPressed);
    DrawLine(
        10, FRAME_HEIGHT - 23 + 16, colorUnpressed,
        10 + (currentInput->leftStickAnalogX - 0x7f) * 9 / 0x80, FRAME_HEIGHT - 23 + 16 + (currentInput->leftStickAnalogY - 0x7f) * 6 / 0x80, colorPressed);
}