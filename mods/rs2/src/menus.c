#include "menus.h"
#include "game.h"

LevelData levels_table[29]  = {
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

OptionData options_table[4] = {
    { "Test1", OPTION_TOGGLE, 0 },
    { "Test2", OPTION_TOGGLE, 0 },
    { "Test3", OPTION_TOGGLE, 0 },
    { "Test4", OPTION_NUMBER, 0 },
};

MenuData menus[2] = {
    {
        TELEPORT_MENU, 
        sizeof(levels_table) / sizeof(LevelData),
        0,
        &levels_table,
    },
    {
        OPTIONS_MENU,
        sizeof(options_table) / sizeof(OptionData),
        0,
        &options_table,
    },
};