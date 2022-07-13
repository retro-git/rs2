#ifndef LEVELS_H
#define LEVELS_H

#include "../../../include/common.h"

typedef struct {
    char* name;
    int load_level_id;
    LevelType type;
} LevelData;

extern LevelData levels_table[29];

#endif