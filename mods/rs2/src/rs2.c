#include "rs2.h"
#include "../../../include/common.h"
#include "levels.h"

struct rs2 rs2 = {0};

void main_hook()
{
    spyro_FUN_800156fc();

    if (rs2.is_warping)
    {
        handle_warp();
    }

    if (spyro_game_state != 0)
        return;

    if (spyro_input_raw.l2 && spyro_input_raw.r2) {
        if (spyro_input_raw.triangle) {
            spyro_player_position = rs2.savestate.position;
            spyro_player_rotation = rs2.savestate.rotation;
            spyro_cam_rotation = rs2.savestate.cam_rotation;
            spyro_cam_position = rs2.savestate.cam_position;
        }
        else if (spyro_input_raw.square) {
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

    if (rs2.menu_enabled)
    {
        draw_menu();
    }
}

void draw_menu()
{
    char buffer[64];
    for (unsigned int i = 0; i < sizeof(levels_table) / sizeof(level_data); i++)
    {
        sprintf(buffer, "%s", levels_table[i].name);
        spyro_DrawText(buffer, i <= 14 ? 100 : 300, 40 + 10 * (i % 15), i == rs2.menu_selection_index ? 1 : 0, 0);
    }
    if (spyro_input_raw.r3)
    {
        rs2.menu_enabled = 0;
    }
    else if (spyro_input_raw.dup)
    {
        rs2.menu_selection_index == 0 ? rs2.menu_selection_index = sizeof(levels_table) / sizeof(level_data) - 1 : rs2.menu_selection_index--;
    }
    else if (spyro_input_raw.ddown)
    {
        rs2.menu_selection_index == sizeof(levels_table) / sizeof(level_data) - 1 ? rs2.menu_selection_index = 0 : rs2.menu_selection_index++;
    }
    else if (spyro_input_raw.select)
    {
        begin_warp();
    }
}

void begin_warp()
{
    rs2.warp_selected_level = levels_table[rs2.menu_selection_index];
    spyro_pause_submenu_index = 0;
    spyro_unk_timer = 0x11;
    spyro_pause_menu_index = 5;
    spyro_game_state = 4;
    *(int *)0x80069894 = 0xffff;
    *(int *)0x80069896 = 0xffff;

    if (rs2.warp_selected_level.type == HOMEWORLD) {
        switch (rs2.menu_selection_index) {
            case SUMMER_FOREST:
                spyro_world_id = IDOL_SPRINGS;
                break;
            case AUTUMN_PLAINS:
                spyro_world_id = SKELOS_BADLANDS;
                break;
            case WINTER_TUNDRA:
                spyro_world_id = DRAGON_SHORES;
                break;
        }
    }

    rs2.menu_enabled = 0;
    rs2.is_warping = 1;
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
    else if (spyro_game_state == 7 && rs2.warp_selected_level.type == BOSS && (unk_cam_bossfix == 0x7404 || unk_cam_bossfix == 0xc9b3 || unk_cam_bossfix == 0x11a75)) //crush, gulp, ripto starting cams respectively
    {
        spyro_unk_timer = 0;
        spyro_game_state = 3;
        *(int16_t *)0x800698F0 = 0;
        spyro_pause_submenu_index = 0;
    }
    else if (spyro_game_state == 7 && rs2.warp_selected_level.type == HOMEWORLD && (unk_cam_homefix == 0xFFFFFDEF || unk_cam_homefix == 0x0402 || unk_cam_homefix == 0x0401)) //home world starting cams respectively 
    {
        spyro_game_state = 3;
        spyro_pause_submenu_index = 0;
        *(int32_t*)0x800698f0 = 0;
        spyro_unk_timer = 0;
    }
}