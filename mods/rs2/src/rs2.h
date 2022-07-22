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
    uint8_t state_saved;
} savestate_t;

struct rs2 {
    uint16_t cur_init_file;
    uint16_t initialised;
    uint16_t initialised_input_hook;
    uint16_t menu_index;
    uint16_t menu_enabled;
    uint16_t is_warping;
    uint16_t frame_advance;
    LevelData warp_selected_level;
    savestate_t savestate;
    CdlCB read_callback;
    int32_t button_holdtimes[16];
};

extern struct rs2 rs2;

void UpdateGame_Normal_hook();
int rand_hook_trampoline();
void handle_input();
void patch_jump(int32_t *overwrite_loc, int32_t jump_loc);
void read_cb(unsigned char status, unsigned char *result);
void handle_warp();

#endif