#ifndef GAME_SPYRO_H
#define GAME_SPYRO_H

#include "libgraphics.h"
#include "vec3.h"

/*
    Structs
    Credits to FranklyGD for his initial research on the Spyro data structures,
    which he documented here: https://github.com/FranklyGD/Spyro-Scope/
*/

typedef struct {
    Vec3 position;          // 0x0
    char unk0xC[0x24];      // 0xC
    MATRIX bodyMatrix;      // 0x30
    int state;              // 0x50
    char unk0x54[0x10];     // 0x54
    Vec3 eulerRotations;  // 0x64
    char unk0x70[0x24];     // 0x70
    Vec3 trueVelocity;      // 0x94
    Vec3 physicsVelocity;   // 0xA0
} Spyro;

/*
    Variables
*/

extern Spyro GAME_spyro;

#endif