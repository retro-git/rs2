#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

#define CdlSetloc	0x02

typedef void (*CdlCB)(unsigned char,unsigned char *);

typedef struct {
	unsigned char minute;		/* minute (BCD) */
	unsigned char second;		/* second (BCD) */
	unsigned char sector;		/* sector (BCD) */
	unsigned char track;		/* track (void) */
} CdlLOC;

typedef struct
{
    int32_t x;
    int32_t z;
} vec2_t;

typedef struct
{
    float x;
    float z;
} vec2f_t;

typedef struct
{
    int16_t x;
    int16_t z;
    int16_t y;
} vec3s_t;

typedef struct
{
    int32_t x;
    int32_t z;
    int32_t y;
} vec3_t;

typedef struct
{
    float x;
    float z;
    float y;
} vec3f_t;

typedef struct
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
} controller_t;

typedef enum {
    SQUARE = 0x3e,
    CROSS = 0x3c,
    CIRCLE = 0x7b,
    TRIANGLE = 0x7d
} special_characters;

typedef enum {
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
} levels;

typedef enum {
    HOMEWORLD = 0,
    STAGE = 1,
    BOSS = 2,
} level_type;

typedef struct {
    vec3_t position;
    vec3_t rotation;
    vec3_t cam_position;
    vec3s_t cam_rotation;
} savestate_t;

CdlLOC *psyq_CdIntToPos(int i, CdlLOC *p);
int psyq_CdControl(unsigned char  com, unsigned char  *param, unsigned char  *result);
int psyq_CdControlB(unsigned char com, unsigned char *param, unsigned char *result);
int psyq_CdRead(int sectors, unsigned long *buf, int mode);
CdlCB psyq_CdReadCallback(CdlCB func);

int printf(char *format, ...);
int sprintf(char *str, char *format, ...);
void spyro_DrawText(char *text, int x, int y, int colorIndex, int *unk);
void spyro_FUN_800156fc(void);
void spyro_FUN_80013a14(unsigned int unk);

extern struct vec3 spyro_speed;
extern vec3_t spyro_player_position;
extern vec3_t spyro_player_rotation;
extern vec3_t spyro_cam_position;
extern vec3s_t spyro_cam_rotation;

extern int32_t spyro_game_state;
extern controller_t spyro_input_raw;
extern int32_t spyro_level_load_id;
extern int32_t spyro_world_id;
extern int32_t spyro_pause_menu_index;
extern uint16_t spyro_pause_submenu_index;
extern uint16_t spyro_unk_timer;

#endif