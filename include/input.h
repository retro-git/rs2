#ifndef GAME_INPUT_H
#define GAME_INPUT_H

#include <stdint.h>

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

extern controller_u_t GAME_input;

#endif