#include "rs2.h"
#include "game.h"
#include "draw.h"

struct rs2 rs2 = {0};

void patch_jump(int32_t *overwrite_loc, int32_t jump_loc)
{
    int32_t instr = ((jump_loc & 0b00000011111111111111111111111111) >> 2) | 0b0001100000000000000000000000000;
    *overwrite_loc = instr;
}

void read_cb(unsigned char status, unsigned char *result)
{
    patch_jump((int32_t *)0x80015808, (int32_t)draw_hook);
    rs2.initialised = 1;
    LIBC_printf("initialised %d\n", BUILD);
    LIBCD_CdReadCallback(rs2.read_callback);
}

void init()
{
#if BUILD == 94424
    // patch_jump((int32_t *)0x80015808, (int32_t)draw_hook);
    rs2.initialised = 1;
    LIBC_printf("init %d\n", BUILD);
#else
    CdlLOC loc;
    LIBCD_CdIntToPos(229989, &loc);

    char res;
    LIBCD_CdControlB(CdlSetloc, (unsigned char*)&loc, &res);

    rs2.read_callback = LIBCD_CdReadCallback(read_cb);
    LIBCD_CdRead(1, (void*)&kernel_free_space_1, 0x80);
#endif
}

void main_hook()
{
    GAME_FUN_800156fc();

    if (rs2.initialised != 1 /*&& GAME_gameState == 0xb*/)
    {
        init();
    }

    if (rs2.initialised)
    {
        if (rs2.is_warping)
        {
            handle_warp();
        }

        if (GAME_gameState == PLAYING)
        {
            handle_input();
        }
    }
}

void handle_input()
{
    for (uint16_t i = 0; i < sizeof(rs2.button_holdtimes) / sizeof(int32_t); i++)
    {
        if (GAME_input.i >> i & 1)
        {
            rs2.button_holdtimes[i]++;
        }
        else
        {
            rs2.button_holdtimes[i] = 0;
        }
    }

    if (GAME_input.b.l2 && GAME_input.b.r2)
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
    GAME_level_load_id = rs2.warp_selected_level.load_level_id;

    if (GAME_gameState == PLAYING)
    {
        rs2.is_warping = 0;
    }
    else if (GAME_gameState == LOADING_LEVELS && rs2.warp_selected_level.type == BOSS && (unk_cam_bossfix == 0x7404 || unk_cam_bossfix == 0xc9b3 || unk_cam_bossfix == 0x11a75)) // crush, gulp, ripto starting cams respectively
    {
        GAME_unk_timer = 0;
        GAME_gameState = TRANSITION_LOAD_TO_PLAYING;
        *(int16_t *)0x800698F0 = 0;
        GAME_pause_submenu_index = 0;
    }
    else if (GAME_gameState == LOADING_LEVELS && rs2.warp_selected_level.type == HOMEWORLD && (unk_cam_homefix == 0xFFFFFDEF || unk_cam_homefix == 0x0402 || unk_cam_homefix == 0x0401)) // home world starting cams respectively
    {
        GAME_gameState = TRANSITION_LOAD_TO_PLAYING;
        GAME_pause_submenu_index = 0;
        *(int32_t *)0x800698f0 = 0;
        GAME_unk_timer = 0;
    }
}