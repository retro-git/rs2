#ifndef RS2_H
#define RS2_H

#include "menus.h"
#include "game.h"
#include "vec3.h"
#include "libcd.h"
#include "common.h"

typedef struct
{
    enum Levels level;
    Vec3 position;
    Vec3 rotation;
    Vec3 cam_position;
    Vec3s cam_rotation;
    bool state_saved;
} savestate_t;

struct rs2 {
    bool initialised;
    uint8_t menu_index;
    bool menu_enabled;
    bool is_warping;
    bool frame_advance;
    LevelData warp_selected_level;
    savestate_t savestate;
    uint16_t button_holdtimes[16];
};

extern struct rs2 rs2;
// extern LevelData levels_table[NUM_LEVELS];

#endif