#include "rs2.h"
#include "../../../include/common.h"
#include "draw.h"

struct rs2 rs2 = {0};

void patch_jump(int32_t *overwrite_loc, int32_t jump_loc)
{
    int32_t instr = ((jump_loc & 0b00000011111111111111111111111111) >> 2) | 0b0001100000000000000000000000000;
    *overwrite_loc = instr;
}

void read_cb(unsigned char status, unsigned char *result)
{
    psyq_CdReadCallback(rs2.read_callback);
    patch_jump((int32_t *)0x80015808, (int32_t)draw_hook);
}

void init()
{
#if EMU_DEBUG == 1
    // patch_jump((int32_t *)0x80015808, (int32_t)draw_hook);
#else
    rs2.read_callback = psyq_CdReadCallback(read_cb);

    CdlLOC loc;
    psyq_CdIntToPos(229989, &loc);

    char res;
    psyq_CdControl(CdlSetloc, &loc, &res);
    psyq_CdRead(1, (void *)0x8000A000, 0x80);
#endif
    rs2.initialised = 1;
}

void main_hook()
{
    spyro_FUN_800156fc();

    if (!rs2.initialised)
        init();

    if (rs2.is_warping)
        handle_warp();

    if (spyro_game_state != 0)
        return;

    handle_input();
}

void handle_input()
{
    for (uint16_t i = 0; i < sizeof(rs2.button_holdtimes) / sizeof(int32_t); i++)
    {
        if (spyro_input_raw.i >> i & 1)
        {
            rs2.button_holdtimes[i]++;
        }
        else
        {
            rs2.button_holdtimes[i] = 0;
        }
    }

    if (spyro_input_raw.b.l2 && spyro_input_raw.b.r2)
    {
        if (rs2.button_holdtimes[TRIANGLE] == 1)
        {
            rs2.savestate.position = spyro_player_position;
            rs2.savestate.rotation = spyro_player_rotation;
            rs2.savestate.cam_rotation = spyro_cam_rotation;
            rs2.savestate.cam_position = spyro_cam_position;

            add_draw_command(DRAW_TEXT_TIMEOUT, &(draw_text_timeout_data_t){
                                                    .text = "Saved state",
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
            spyro_player_position = rs2.savestate.position;
            spyro_player_rotation = rs2.savestate.rotation;
            spyro_cam_rotation = rs2.savestate.cam_rotation;
            spyro_cam_position = rs2.savestate.cam_position;

            add_draw_command(DRAW_TEXT_TIMEOUT, &(draw_text_timeout_data_t){
                                                    .text = "Loaded state",
                                                    .x = 80,
                                                    .y = 80,
                                                    .col = 4,
                                                    .cur_time = 0,
                                                    .start_time = 0,
                                                    .end_time = 60,
                                                });
        }
    }
    else if (rs2.menu_enabled == 0 && rs2.button_holdtimes[R3] == 1)
    {
        rs2.menu_enabled = 1;
    }
    else if (rs2.menu_enabled && rs2.button_holdtimes[R3] == 1)
    {
        rs2.menu_enabled = 0;
    }
}

void handle_warp()
{
    uint32_t unk_cam_bossfix = *(uint32_t *)0x80067eB0;
    uint32_t unk_cam_homefix = *(uint32_t *)0x80067F28;
    spyro_level_load_id = rs2.warp_selected_level.load_level_id;

    if (spyro_game_state == 0)
    {
        rs2.is_warping = 0;
    }
    else if (spyro_game_state == 7 && rs2.warp_selected_level.type == BOSS && (unk_cam_bossfix == 0x7404 || unk_cam_bossfix == 0xc9b3 || unk_cam_bossfix == 0x11a75)) // crush, gulp, ripto starting cams respectively
    {
        spyro_unk_timer = 0;
        spyro_game_state = 3;
        *(int16_t *)0x800698F0 = 0;
        spyro_pause_submenu_index = 0;
    }
    else if (spyro_game_state == 7 && rs2.warp_selected_level.type == HOMEWORLD && (unk_cam_homefix == 0xFFFFFDEF || unk_cam_homefix == 0x0402 || unk_cam_homefix == 0x0401)) // home world starting cams respectively
    {
        spyro_game_state = 3;
        spyro_pause_submenu_index = 0;
        *(int32_t *)0x800698f0 = 0;
        spyro_unk_timer = 0;
    }
}