#include "menus.h"
#include "game.h"
#include "input.h"
#include "rs2.h"
#include "draw.h"

// const char* zero = "zero";
// const char* one = "one";
// const char* two = "two";

// const char* testoptions[3] = {
//     "zero",
//     "one",
//     "two",
// };

OptionData menu1_options_table[NUM_OPTIONS_MENU1] = {
    {"Input View", 0, 0, option_input_display_execute},
    {"Satyr Tools", 0, 0, option_satyrless_tools_execute},
    {"AutoFish", 0, 0, option_instant_fish_execute},
    {"Powerups", 0, 0, option_powerups_alwayson_execute},
    {"Lock Lives", 1, 0, option_infinite_lives_execute},
    {"Zombie", 0, 0, option_toggle_zombie_execute},
    {"Sparx ER", 1, 0, option_toggle_sparx_extended_range_execute},
    {"Fireball", 1, 0, option_toggle_fireball_execute},
    // { "Moon Jump Speed", OPTION_NUMBER, &(OptionNumberData){ .number = 20, .min = 6, .max = 100, .names = {0} } },
    // { "test", OPTION_NUMBER, &(OptionNumberData){ .number = 0, .min = 0, .max = 2, .names = {testoptions} } },
};

OptionData menu2_respawn_options_table[NUM_OPTIONS_MENU2_RESPAWN] = {
    {"Checkpoint", 0, 0, option_reset_checkpoint_execute},
    {"Gem", 0, 0, option_reset_gems_execute},
    {"Orb", 0, 0, option_reset_orbs_execute},
    {"Talisman", 0, 0, option_reset_eol_portals_execute},
};

MenuData menus[NUM_MENUS] = {
    {
        MENU_TYPE_TELEPORT,
        "Teleport",
        sizeof(levels_table) / sizeof(LevelData),
        0,
        levels_table,
    },
    {
        MENU_TYPE_OPTIONS,
        "General",
        NUM_OPTIONS_MENU1,
        0,
        (LevelData *)menu1_options_table,
    },
    {
        MENU_TYPE_OPTIONS,
        "Reset On Respawn - L2 R2 START",
        NUM_OPTIONS_MENU2_RESPAWN,
        0,
        (LevelData *)menu2_respawn_options_table,
    }};