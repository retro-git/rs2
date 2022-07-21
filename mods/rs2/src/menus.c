#include "menus.h"
#include "game.h"

LevelData levels_table[NUM_LEVELS]  = {
    { "Summer Forest", 0xa, HOMEWORLD },
    { "Glimmer", 0xb, STAGE },
    { "Idol Springs", 0xc, STAGE },
    { "Colossus", 0xd, STAGE },
    { "Hurricos", 0x15, STAGE },
    { "Aquaria Towers", 0x16, STAGE },
    { "Sunny Beach", 0x17, STAGE },
    { "Ocean Speedway", 0x19, STAGE },
    { "Crush", 0x1a, BOSS },
    { "Autumn Plains", 0x1e, HOMEWORLD },
    { "Skelos Badlands", 0x1f, STAGE },
    { "Crystal Glacier", 0x20, STAGE },
    { "Breeze Harbor", 0x21, STAGE },
    { "Zephyr", 0x22, STAGE },
    { "Metro Speedway", 0x23, STAGE },
    { "Scorch", 0x29, STAGE },
    { "Shady Oasis", 0x2a, STAGE },
    { "Magma Cone", 0x2b, STAGE },
    { "Fracture Hills", 0x2c, STAGE },
    { "Icy Speedway", 0x2d, STAGE },
    { "Gulp", 0x2e, BOSS },
    { "Winter Tundra", 0x32, HOMEWORLD },
    { "Mystic Marsh", 0x33, STAGE },
    { "Cloud Temples", 0x34, STAGE },
    { "Canyon Speedway", 0x37, STAGE },
    { "Robotica Farms", 0x3d, STAGE },
    { "Metropolis", 0x3e, STAGE },
    { "Dragon Shores", 0x41, STAGE },
    { "Ripto", 0x42, BOSS },
};

//const char* zero = "zero";
//const char* one = "one";
//const char* two = "two";

const char* testoptions[3] = {
    "zero",
    "one",
    "two",
};

OptionData menu1_options_table[NUM_OPTIONS_MENU1] = {
    { "Input Display", OPTION_TOGGLE, &(OptionToggleData){ .toggled = 0, .execute = option_input_display_execute } },
    { "Satyrless Tools", OPTION_TOGGLE, &(OptionToggleData){ .toggled = 0, .execute = option_satyrless_tools_execute } },
    { "Instant Fish", OPTION_TOGGLE, &(OptionToggleData){ .toggled = 0, .execute = option_instant_fish_execute } },
    { "Powerups Enabled", OPTION_TOGGLE, &(OptionToggleData){ .toggled = 0, .execute = option_powerups_alwayson_execute }},
    { "Lock Life Count", OPTION_ONESHOT, &(OptionOneShotData){ .execute = option_infinite_lives_execute }},
    { "Toggle Zombie", OPTION_ONESHOT, &(OptionOneShotData){ .execute = option_toggle_zombie_execute }},
    { "Toggle Sparx ER", OPTION_ONESHOT, &(OptionOneShotData){ .execute = option_toggle_sparx_extended_range_execute }},
    { "Toggle Fireball", OPTION_ONESHOT, &(OptionOneShotData){ .execute = option_toggle_fireball_execute } },
    { "Moon Jump Speed", OPTION_NUMBER, &(OptionNumberData){ .number = 20, .min = 6, .max = 100, .names = {0} } },
    { "test", OPTION_NUMBER, &(OptionNumberData){ .number = 0, .min = 0, .max = 2, .names = {testoptions} } },
};

OptionData menu2_respawn_options_table[NUM_OPTIONS_MENU2_RESPAWN] = {
    { "Reset Checkpoint", OPTION_TOGGLE, &(OptionToggleData){ .toggled = 0, .execute = option_reset_checkpoint_execute } },
    { "Reset Gems", OPTION_TOGGLE, &(OptionToggleData){ .toggled = 0, .execute = option_reset_gems_execute } },
    { "Reset Orbs", OPTION_TOGGLE, &(OptionToggleData){ .toggled = 0, .execute = option_reset_orbs_execute } },
    { "Reset Talismans", OPTION_TOGGLE, &(OptionToggleData){ .toggled = 0, .execute = option_reset_eol_portals_execute } },
};

MenuData menus[NUM_MENUS] = {
    {
        MENU_TYPE_TELEPORT, 
        sizeof(levels_table) / sizeof(LevelData),
        0,
        &levels_table,
    },
    {
        MENU_TYPE_OPTIONS,
        NUM_OPTIONS_MENU1,
        0,
        &menu1_options_table,
    },
    {
        MENU_TYPE_OPTIONS,
        NUM_OPTIONS_MENU2_RESPAWN,
        0,
        &menu2_respawn_options_table,
    }
};