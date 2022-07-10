#include "../../../include/common.h"
#include "draw.h"
#include "rs2.h"
#include "levels.h"

void draw_hook(unsigned int unk)
{
    spyro_FUN_80013a14(unk);

    if (rs2.menu_enabled) draw_menu();
}

void draw_menu()
{
    char buffer[32];
    for (unsigned int i = 0; i < sizeof(levels_table) / sizeof(level_data); i++)
    {
        sprintf(buffer, "%s", levels_table[i].name);
        spyro_DrawText(buffer, i <= 14 ? 100 : 300, 40 + 10 * (i % 15), i == rs2.menu_selection_index ? 1 : 0, 0);
    }
    if (rs2.button_holdtimes[R3])
    {
        rs2.menu_enabled = 0;
    }
    else if (rs2.button_holdtimes[DUP] && rs2.button_holdtimes[DUP] % 2 == 0)
    {
        rs2.menu_selection_index == 0 ? rs2.menu_selection_index = sizeof(levels_table) / sizeof(level_data) - 1 : rs2.menu_selection_index--;
    }
    else if (rs2.button_holdtimes[DDOWN] && rs2.button_holdtimes[DDOWN] % 2 == 0)
    {
        rs2.menu_selection_index == sizeof(levels_table) / sizeof(level_data) - 1 ? rs2.menu_selection_index = 0 : rs2.menu_selection_index++;
    }
    else if (spyro_input_raw.b.select)
    {
        rs2.menu_enabled = 0;
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

    if (rs2.warp_selected_level.type == HOMEWORLD)
    {
        switch (rs2.menu_selection_index)
        {
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

    rs2.is_warping = 1;
}