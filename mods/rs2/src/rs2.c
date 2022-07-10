#include "rs2.h"
#include "../../../include/common.h"
#include "draw.h"

struct rs2 rs2 = {0};

void read_cb(unsigned char status, unsigned char *result)
{
    psyq_CdReadCallback(rs2.read_callback);

    int32_t *draw_hook_call_loc = (int32_t *)0x80015808;
    int32_t instr = ((((int)(draw_hook) & 0b00000011111111111111111111111111) >> 2) | 0b0001100000000000000000000000000);

    *draw_hook_call_loc = instr;

    /*char *draw_hook_call_loc = (char *)0x80015808;
    char *draw_hook_loc = draw_hook;

    draw_hook_call_loc[0] = 0x00;
    draw_hook_call_loc[1] = 0x28;
    draw_hook_call_loc[2] = 0x00;
    draw_hook_call_loc[3] = 0x0c;*/
}

void main_hook()
{
    spyro_FUN_800156fc();

    if (!rs2.initialised)
    {
        rs2.read_callback = psyq_CdReadCallback(read_cb);

        CdlLOC loc;
        psyq_CdIntToPos(229989, &loc);

        //loc.minute = dec2bcd_r(51);
        //loc.second = dec2bcd_r(8);
        //loc.sector = dec2bcd_r(40);

        char res;
        psyq_CdControl(CdlSetloc, &loc, &res);
        psyq_CdRead(1, (void *)0x8000A000, 0x80);

        rs2.initialised = 1;
    }

    if (rs2.is_warping)
    {
        handle_warp();
    }

    if (spyro_game_state != 0)
        return;

    if (spyro_input_raw.l2 && spyro_input_raw.r2)
    {
        if (spyro_input_raw.triangle)
        {
            spyro_player_position = rs2.savestate.position;
            spyro_player_rotation = rs2.savestate.rotation;
            spyro_cam_rotation = rs2.savestate.cam_rotation;
            spyro_cam_position = rs2.savestate.cam_position;
        }
        else if (spyro_input_raw.square)
        {
            rs2.savestate.position = spyro_player_position;
            rs2.savestate.rotation = spyro_player_rotation;
            rs2.savestate.cam_rotation = spyro_cam_rotation;
            rs2.savestate.cam_position = spyro_cam_position;
        }
    }

    if (spyro_input_raw.l3 && rs2.menu_enabled == 0)
    {
        rs2.menu_enabled = 1;
    }
}

void handle_warp()
{
    int32_t unk_cam_bossfix = *(int32_t *)0x80067eB0;
    int32_t unk_cam_homefix = *(int32_t *)0x80067F28;
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