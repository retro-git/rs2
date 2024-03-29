#ifndef MENUS_H
#define MENUS_H

#include "game.h"
#include "options.h"

#define NUM_MENUS 3
#define NUM_LEVELS 29
#define NUM_OPTIONS_MENU1 8
#define NUM_OPTIONS_MENU2_RESPAWN 4

typedef void (*option_execute_func)(void);

typedef enum {
    TELEPORT_MENU = 0,
    OPTIONS_MENU1 = 1,
    OPTIONS_MENU2_RESPAWN = 2,
} MENUS;

typedef enum {
    MENU1_INPUT_DISPLAY = 0,
    MENU1_SATYRLESS = 1,
    MENU1_INSTANT_FISH = 2,
    MENU1_POWERUPS_ALWAYSON = 3,
    MENU1_LOCK_LIVES = 4,
    MENU1_ZOMBIE = 5,
    MENU1_SPARX_ER = 6,
    MENU1_FIREBALL = 7,
    // MENU1_MOONJUMP_SPEED = 8,
} MENU1_OPTIONS;

typedef enum {
    MENU2_RESPAWN_RESET_CHECKPOINT = 0,
    MENU2_RESPAWN_RESET_GEMS = 1,
    MENU2_RESPAWN_RESET_ORBS = 2,
    MENU2_RESPAWN_RESET_TALISMANS = 3,
} MENU2_RESPAWN_OPTIONS;

typedef struct {
    char* name;
    int8_t load_level_id;
    LevelType type;
} LevelData;

typedef enum {
    OPTION_TOGGLE = 0,
    OPTION_NUMBER = 1,
    OPTION_ONESHOT = 2,
} OPTION_TYPE;

typedef struct {
    option_execute_func execute;
} OptionOneShotData;

// typedef struct {
//     bool toggled;
//     bool oneshot;
//     option_execute_func execute;
// } OptionToggleData;

typedef struct {
    int16_t number;
    int16_t min;
    int16_t max;
    const char** names;
} OptionNumberData;

typedef struct {
    char* name;
    bool oneshot;
    bool toggled;
    option_execute_func execute;
    // union {
    //     OptionToggleData* option_toggle_data;
    //     OptionNumberData* option_number_data;
    //     OptionOneShotData* option_oneshot_data;
    // } d;
} OptionData;

typedef enum {
    MENU_TYPE_TELEPORT = 0,
    MENU_TYPE_OPTIONS = 1,
} MENU_TYPE;

typedef struct {
    MENU_TYPE type;
    const char* title;
    uint8_t num_options;
    uint8_t menu_selection_index;
    union {
        LevelData* levels_table;
        OptionData* options_table;
    } d;
} MenuData;

extern MenuData menus[NUM_MENUS];

#endif