#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

#include "vec3.h"

#define L2 0 //0x0
#define R2 1 //0x1
#define L1 2 //0x2
#define R1 3 //0x4
#define TRIANGLE 4 //0x8
#define CIRCLE 5 //0x10
#define CROSS 6 //0x20
#define SQUARE 7 //0x40
#define SELECT 8 //0x80
#define L3 9 //0x100
#define R3 10 //0x200
#define START 11 //0x400
#define DUP 12 //0x800
#define DRIGHT 13 //0x1000
#define DDOWN 14 //0x2000
#define DLEFT 15 //0x4000

#define FRAME_WIDTH 512
#define FRAME_HEIGHT 228

#define SCREEN_LEFT 0
#define SCREEN_RIGHT 512
#define SCREEN_TOP 12
#define SCREEN_BOTTOM 228

#define LINE_HEIGHT 12

typedef union
{
    uint32_t i;
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

typedef enum {
    TEXTCOL_GREEN = 4,
    TEXTCOL_RED = 5,
} DrawTextColors;

typedef struct {
    controller_u_t current;
    controller_u_t pressed;
    controller_u_t released;
    uint8_t rightStickAnalogX;
    uint8_t rightStickAnalogY;
    uint8_t leftStickAnalogX;
    uint8_t leftStickAnalogY;
} InputState;

typedef enum
{
    SQUARE_CHAR = 0x3e,
    CROSS_CHAR = 0x3c,
    CIRCLE_CHAR = 0x7b,
    TRIANGLE_CHAR = 0x7d
} SpecialChars;

typedef enum
{
    HOMEWORLD = 0,
    STAGE = 1,
    BOSS = 2,
} LevelType;

typedef struct {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
} Color;

enum BlendMode {
	BLENDMODE_MULTIPLY,
	BLENDMODE_ADD
};

int LIBC_rand();

void GAME_ReadInput(void);
void GAME_RenderGame(void);
void GAME_UpdateGame_Normal(void);
void GAME_FUN_80013a14(unsigned int unk);
void GAME_FUN_80077374(int unk);

extern int32_t header_start;
extern int32_t kernel_free_space_1;

extern Vec3 GAME_cam_position;
extern Vec3s GAME_cam_rotation;
extern InputState GAME_inputStates[5];
extern int32_t GAME_rng;

extern int32_t GAME_level_load_id;
extern int32_t GAME_world_id;
extern int32_t GAME_pause_menu_index;
extern uint16_t GAME_pause_submenu_index;
extern uint16_t GAME_unk_timer;

#endif