#ifndef MENUS_H
#define MENUS_H

#include "game.h"

typedef struct {
    char* name;
    int load_level_id;
    LevelType type;
} LevelData;

typedef struct {
    char* name;
} OptionData;

typedef enum {
    TELEPORT_MENU = 0,
    OPTIONS_MENU = 1,
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