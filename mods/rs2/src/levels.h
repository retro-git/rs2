#ifndef LEVELS_H
#define LEVELS_H

#include "../../../include/common.h"

typedef struct {
    char* name;
    int load_level_id;
    level_type type;
} level_data;

extern level_data levels_table[29];

#endif