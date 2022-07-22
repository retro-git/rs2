#include "rs2.h"
#include "game.h"
#include "draw.h"
#include "hook.h"
#include "libcd.h"
#include "libc.h"
#include "spyro.h"
#include "common.h"
#include "input.h"

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

void init_effects()
{
    // skip intro and load into glimmer
    if (GAME_gameState == 0xb)
    {
        *(uint8_t *)(0x80066f90) = 0xb;
        *(uint8_t *)(0x800698f4) = 3;
        *(uint8_t *)(0x80067e0c) = 5;
        GAME_num_lives = 99;
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
    // remove exit level cutscenes
    *(uint8_t *)0x80066ff4 = 0;
}

void UpdateGame_Normal_hook()
{
    if (!rs2.menu_enabled && !rs2.frame_advance)
    {
        GAME_UpdateGame_Normal();
    }
    else if (!rs2.menu_enabled && rs2.frame_advance && (rs2.button_holdtimes[L3] == 1 || rs2.button_holdtimes[L3] > 5))
    {
        GAME_inputStates[0].pressed.i = GAME_inputStates[0].current.i;
        GAME_UpdateGame_Normal();
    }
}

#define NUM_OVERLAY_FILES 2

typedef struct
{
    uint8_t loading;
    uint32_t* hook_loc;
    int32_t* hook_func;
} OverlayFile;

OverlayFile files[NUM_OVERLAY_FILES] = {
    {0, 0x80015808, draw_hook},
    {0, 0x8001b150, read_input_hook},
};

void read_cb(unsigned char status, unsigned char *result)
{
    //LIBC_printf("loaded file %d\n", rs2.cur_init_file);
    //patch_jump(files[rs2.cur_init_file].hook_loc, files[rs2.cur_init_file].hook_func);
    //patch_jump((int32_t *)0x80015808, (int32_t)rs2.cur_init_file == 0 ? draw_hook : read_input_hook);
    rs2.cur_init_file++;

    if (rs2.cur_init_file == NUM_OVERLAY_FILES)
    {
        //LIBC_printf("all files loaded\n");
        for (int i = 0; i < NUM_OVERLAY_FILES; i++) {
            patch_jump(files[i].hook_loc, files[i].hook_func); 
        }
        patch_jump((int32_t *)0x8001b190, (int32_t)UpdateGame_Normal_hook);
        LIBCD_CdReadCallback(rs2.read_callback);
        init_effects();
        rs2.initialised = 1;
    }
}

void init()
{
    //LIBC_printf("init %d\n", BUILD);

#if BUILD == 94424
    patch_jump((int32_t *)0x80015808, (int32_t)draw_hook);
    patch_jump((int32_t *)0x8001b150, (int32_t)read_input_hook);
    patch_jump((int32_t *)0x8001b190, (int32_t)UpdateGame_Normal_hook);
    rs2.initialised = 1;
#else
    if (files[rs2.cur_init_file].loading == 0)
    {
        CdlLOC loc;
        if (rs2.cur_init_file == 0) rs2.read_callback = LIBCD_CdReadCallback(read_cb);
        LIBCD_CdIntToPos(rs2.cur_init_file == 0 ? 229989 : 229991, &loc);
        //LIBCD_CdIntToPos(229989, &loc);

        char res;
        LIBCD_CdControlB(CdlSetloc, &loc, &res) == 0;
        LIBCD_CdRead(rs2.cur_init_file == 0 ? 2 : 1, (void *)rs2.cur_init_file == 0 ? &kernel_free_space_1 : &kernel_free_space_2, 0x80);
            //LIBC_printf("cdread failed\n");

        files[rs2.cur_init_file].loading = 1;
    }
#endif

    // rs2.initialised = 1;
}

void main_hook()
{
    GAME_RenderGame();

    if (!rs2.initialised)
    {
        init();
    }

    passive_effects();

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

LevelData levels_table[NUM_LEVELS] = {
    {(char *)0x800106e0, 0xa, HOMEWORLD},  // summer
    {(char *)0x80066ea0, 0xb, STAGE},      // glimmer
    {(char *)0x800106d0, 0xc, STAGE},      // idol
    {(char *)0x800106c4, 0xd, STAGE},      // colossus
    {(char *)0x800106b8, 0x15, STAGE},     // hurricos
    {(char *)0x800106a8, 0x16, STAGE},     // aquaria
    {(char *)0x8001069c, 0x17, STAGE},     // sunny beach
    {(char *)0x8001068c, 0x19, STAGE},     // ocean speedway
    {(char *)0x8001067c, 0x1a, BOSS},      // crush
    {(char *)0x8001066c, 0x1e, HOMEWORLD}, // ap
    {(char *)0x8001065c, 0x1f, STAGE},     // skelos
    {(char *)0x8001064c, 0x20, STAGE},     // crystal
    {(char *)0x8001063c, 0x21, STAGE},     // breeze
    {(char *)0x80066e98, 0x22, STAGE},     // zephyr
    {(char *)0x8001062c, 0x23, STAGE},     // metro
    {(char *)0x80066e90, 0x29, STAGE},     // scorch
    {(char *)0x80010620, 0x2a, STAGE},     // shady
    {(char *)0x80010614, 0x2b, STAGE},     // magma
    {(char *)0x80010604, 0x2c, STAGE},     // fracture
    {(char *)0x800105f4, 0x2d, STAGE},     // icy
    {(char *)0x800105e4, 0x2e, BOSS},      // gulp
    {(char *)0x800105d4, 0x32, HOMEWORLD}, // winter tundra
    {(char *)0x800105c4, 0x33, STAGE},     // mystic
    {(char *)0x800105b4, 0x34, STAGE},     // cloud
    {(char *)0x800105a4, 0x37, STAGE},     // canyon
    {(char *)0x80010594, 0x3d, STAGE},     // robotica
    {(char *)0x80010588, 0x3e, STAGE},     // metropolis
    {(char *)0x80010578, 0x41, STAGE},     // dragon shores
    {(char *)0x80010568, 0x42, BOSS},      // ripto
};