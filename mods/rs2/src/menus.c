#include "menus.h"
#include "game.h"
#include "input.h"
#include "rs2.h"
#include "draw.h"

//const char* zero = "zero";
//const char* one = "one";
//const char* two = "two";

// const char* testoptions[3] = {
//     "zero",
//     "one",
//     "two",
// };

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
   // { "test", OPTION_NUMBER, &(OptionNumberData){ .number = 0, .min = 0, .max = 2, .names = {testoptions} } },
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
        "Teleport Menu",
        sizeof(levels_table) / sizeof(LevelData),
        0,
        &levels_table,
    },
    {
        MENU_TYPE_OPTIONS,
        "General Options",
        NUM_OPTIONS_MENU1,
        0,
        &menu1_options_table,
    },
    {
        MENU_TYPE_OPTIONS,
        "Respawn Options",
        NUM_OPTIONS_MENU2_RESPAWN,
        0,
        &menu2_respawn_options_table,
    }
};