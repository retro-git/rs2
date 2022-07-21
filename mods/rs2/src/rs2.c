#include "rs2.h"
#include "game.h"
#include "draw.h"
#include "hook.h"
#include "libcd.h"
#include "libc.h"
#include "spyro.h"
#include "common.h"

struct rs2 rs2 = {0};

/*int __attribute__((optimize("O0"))) rand_hook()
{
    return rand_hook_trampoline();
}

int rand_hook_trampoline()
{
    // int32_t jump_loc =  (0x80059d34 & 0b00000011111111111111111111111111) >> 2;
    int32_t jump_loc = 0x80059d34;
    asm volatile(".set noreorder");
    asm volatile("lui $v1, 0x41c6");
    asm volatile("lui $v0, 0x8007");
    asm volatile("j %0"
                 :
                 : "g"(jump_loc));
    asm volatile("nop");
}*/

void UpdateGame_Normal_hook() {
    if (!rs2.menu_enabled && !rs2.frame_advance) {
        GAME_UpdateGame_Normal();
    }
    else if (!rs2.menu_enabled && rs2.frame_advance && (rs2.button_holdtimes[L3] == 1 || rs2.button_holdtimes[L3] > 5)) {
        GAME_inputStates[0].pressed.i = GAME_inputStates[0].current.i;
        GAME_UpdateGame_Normal();
    }
}

void init() {
    for (int i = 0; i < sizeof(GAME_moneybags_paywalls) / sizeof(MoneybagsPaywall); i++) {
        GAME_moneybags_paywalls[i].cost = 0;
    }

    rs2.initialised = true;
}

void main_hook()
{
    GAME_RenderGame();

    if (!rs2.initialised) init();

    GAME_ripto_zoe_state = 0;

    if (rs2.is_warping)
    {
        handle_warp();
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