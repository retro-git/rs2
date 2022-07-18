#ifndef GAME_GPU_H
#define GAME_GPU_H

#include "common.h"
#include "libgraphics.h"

typedef struct {
    DRAWENV drawenv;
    DISPENV dispenv;
    int unk;
} ScreenBuffer;

typedef struct {
    unsigned long *tag;
    unsigned char r0; 
    unsigned char g0;
    unsigned char b0;
    unsigned char code;
    short x0, y0;
    unsigned char r1;
    unsigned char g1;
    unsigned char b1;
    unsigned char p1;
    short x1;
    short y1;
} LINE_G2;

typedef struct {
    unsigned long *tag;
    unsigned char r0, g0, b0;
    unsigned char code;
    short x0, y0;
    short x1, y1;
    short x2, y2;
} POLY_F3; 

typedef struct {
    unsigned long *tag;
    unsigned char r0, g0, b0;
    unsigned char code;
    short x0, y0;
    short x1, y1;
    short x2, y2;
    short x3, y3;
} POLY_F4;

/*
    Functions
*/

void GAME_GPUInitGraphics();
void GAME_GPUPackets_Insert(unsigned int * gpuCommand);
void GAME_GPUClearDoubleBuffer();
void GAME_GPUChangeTexPage(int texPage);

/*
    Data
*/

extern DISPENV * GAME_GPUCurrDrawenv;
extern LinkedList GAME_GPUPackets;
extern unsigned int * GAME_GPUPackets_Next;
extern ScreenBuffer GAME_GPUScreenBuffer[2]; // double buffer

#endif