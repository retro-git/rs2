#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include "libcd.h"
#include "libc.h"
#include "spyro.h"
#include "vec3.h"
#include "libgraphics.h"
#include "common.h"

#define L2 0
#define R2 1
#define L1 2
#define R1 3
#define TRIANGLE 4
#define CIRCLE 5
#define CROSS 6
#define SQUARE 7
#define SELECT 8
#define L3 9
#define R3 10
#define START 11
#define DUP 12
#define DRIGHT 13
#define DDOWN 14
#define DLEFT 15

typedef union
{
    uint16_t i;
    struct
    {
        uint16_t l2 : 1;
        uint16_t r2 : 1;
        uint16_t l1 : 1;
        uint16_t r1 : 1;
        uint16_t triangle : 1;
        uint16_t circle : 1;
        uint16_t cross : 1;
        uint16_t square : 1;
        uint16_t select : 1;
        uint16_t l3 : 1;
        uint16_t r3 : 1;
        uint16_t start : 1;
        uint16_t dup : 1;
        uint16_t dright : 1;
        uint16_t ddown : 1;
        uint16_t dleft : 1;
    } b;
} controller_u_t;

typedef enum
{
    SQUARE_CHAR = 0x3e,
    CROSS_CHAR = 0x3c,
    CIRCLE_CHAR = 0x7b,
    TRIANGLE_CHAR = 0x7d
} SpecialChars;

typedef enum
{
    SUMMER_FOREST = 0,
    GLIMMER = 1,
    IDOL_SPRINGS = 2,
    COLOSSUS = 3,
    HURRICOS = 4,
    AQUARIA_TOWERS = 5,
    SUNNY_BEACH = 6,
    OCEAN_SPEEDWAY = 7,
    CRUSH = 8,
    AUTUMN_PLAINS = 9,
    SKELOS_BADLANDS = 10,
    CRYSTAL_GLACIER = 11,
    BREEZE_HARBOR = 12,
    ZEPHYR = 13,
    METRO_SPEEDWAY = 14,
    SCORCH = 15,
    SHADY_OASIS = 16,
    MAGMA_CONE = 17,
    FRACTURE_HILLS = 18,
    ICY_SPEEDWAY = 19,
    GULP = 20,
    WINTER_TUNDRA = 21,
    MYSTIC_MARSH = 22,
    CLOUD_TEMPLES = 23,
    CANYON_SPEEDWAY = 24,
    ROBOTICA_FARMS = 25,
    METROPOLIS = 26,
    DRAGON_SHORES = 27,
    RIPTO = 28,
} Levels;

typedef enum
{
    HOMEWORLD = 0,
    STAGE = 1,
    BOSS = 2,
} LevelType;

void GAME_FUN_800156fc(void);
void GAME_FUN_80013a14(unsigned int unk);

extern int32_t header_start;
extern int32_t kernel_free_space_1;

extern Vec3 GAME_cam_position;
extern Vec3s GAME_cam_rotation;
extern controller_u_t GAME_input;

extern int32_t GAME_level_load_id;
extern int32_t GAME_world_id;
extern int32_t GAME_pause_menu_index;
extern uint16_t GAME_pause_submenu_index;
extern uint16_t GAME_unk_timer;

#endif