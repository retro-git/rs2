#ifndef MENUS_H
#define MENUS_H

#include "game.h"
#include "options.h"

typedef void (*option_execute_func)(void);

typedef enum {
    TELEPORT_MENU = 0,
    OPTIONS_MENU1 = 1,
} MENUS;

typedef enum {
    MENU1_INPUT_DISPLAY = 0,
} MENU1_OPTIONS;

typedef struct {
    char* name;
    int load_level_id;
    LevelType type;
} LevelData;

typedef enum {
    OPTION_TOGGLE = 0,
    OPTION_NUMBER = 1,
} OPTION_TYPE;

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

extern MenuData menus[2];

#endif