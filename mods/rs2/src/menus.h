#ifndef MENUS_H
#define MENUS_H

#include "game.h"
#include "options.h"

#define NUM_MENUS 3
#define NUM_LEVELS 29
#define NUM_OPTIONS_MENU1 7
#define NUM_OPTIONS_MENU2_RESPAWN 2

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
    MENU1_ZOMBIE = 4,
    MENU1_SPARX_ER = 5,
    MENU1_FIREBALL = 6,
} MENU1_OPTIONS;

typedef enum {
    //MENU2_RESPAWN_RESET_CHECKPOINT = 0,
    MENU2_RESPAWN_RESET_GEMS = 0,
    MENU2_RESPAWN_RESET_ORBS = 1,
} MENU2_RESPAWN_OPTIONS;

typedef struct {
    char* name;
    int load_level_id;
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

typedef struct {
    int16_t toggled;
    option_execute_func execute;
} OptionToggleData;

typedef struct {
    int32_t number;
} OptionNumberData;

typedef struct {
    char* name;
    OPTION_TYPE type;
    union {
        OptionToggleData* option_toggle_data;
        OptionNumberData* option_number_data;
        OptionOneShotData* option_oneshot_data;
    } d;
} OptionData;

typedef enum {
    MENU_TYPE_TELEPORT = 0,
    MENU_TYPE_OPTIONS = 1,
} MENU_TYPE;

typedef struct {
    MENU_TYPE type;
    uint16_t num_options;
    uint16_t menu_selection_index;
    union {
        LevelData* levels_table;
        OptionData* options_table;
    } d;
} MenuData;

extern MenuData menus[NUM_MENUS];

#endif