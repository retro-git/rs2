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
        if (rs2.button_holdtimes[TRIANGLE] == 1)
        {
            rs2.savestate.position = GAME_spyro.position;
            rs2.savestate.rotation = GAME_spyro.eulerRotations;
            rs2.savestate.cam_rotation = GAME_cam_rotation;
            rs2.savestate.cam_position = GAME_cam_position;

            add_draw_command(DRAW_TEXT_TIMEOUT, &(draw_text_timeout_data_t){
                                                    .text = "Saved State",
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

            add_draw_command(DRAW_TEXT_TIMEOUT, &(draw_text_timeout_data_t){
                                                    .text = "Loaded State",
                                                    .x = 80,
                                                    .y = 80,
                                                    .col = 4,
                                                    .cur_time = 0,
                                                    .start_time = 0,
                                                    .end_time = 60,
                                                });
        }
    }

    if (rs2.button_holdtimes[R3] == 1)
    {
        rs2.menu_enabled = !rs2.menu_enabled;
    }

    if (rs2.menu_enabled)
    {
        MenuData* menu = &menus[rs2.menu_index];
        if (rs2.button_holdtimes[R2] == 1)
        {
            //LIBC_printf("%d\n", rs2.menu_index);
            LIBC_printf("hello");
            LIBC_printf("%d\n", (rs2.menu_index + 1) % (sizeof(menus) / sizeof(MenuData)));
            LIBC_printf("%d\n", sizeof(menus) / sizeof(MenuData));
            rs2.menu_index = (rs2.menu_index + 1) % (sizeof(menus) / sizeof(MenuData));
        }
        else if (rs2.button_holdtimes[L2] == 1)
        {
            rs2.menu_index = (rs2.menu_index + (sizeof(menus) / sizeof(MenuData) - 1)) % (sizeof(menus) / sizeof(MenuData));
        }
        else if (rs2.button_holdtimes[DUP] && rs2.button_holdtimes[DUP] % 2 == 0)
        {
            menu->menu_selection_index == 0 ? menu->menu_selection_index = menus[rs2.menu_index].num_options - 1 : menu->menu_selection_index--;
        }
        else if (rs2.button_holdtimes[DDOWN] && rs2.button_holdtimes[DDOWN] % 2 == 0)
        {
            menu->menu_selection_index == menus[rs2.menu_index].num_options - 1 ? menu->menu_selection_index = 0 : menu->menu_selection_index++;
        }
        else if (currentInput.b.select)
        {
            rs2.menu_enabled = 0;
            begin_warp();
        }
    }
}