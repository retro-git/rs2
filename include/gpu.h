#ifndef GAME_GPU_H
#define GAME_GPU_H

#include "game.h"
#include "libgraphics.h"

typedef struct {
    DRAWENV drawenv;
    DISPENV dispenv;
    int unk;
} ScreenBuffer;

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