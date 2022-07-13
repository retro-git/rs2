#ifndef GAME_CD_H
#define GAME_CD_H

/*
    Structs
*/

typedef struct {
    int WAD_sector; // 500, hardcoded
    int numSectorsReading;
    unsigned char controlParam;
    char unk[3]; // padding ???
    unsigned int * readBufferPtr;
    int isReading;
    int unk2;
    int unk3;
    int unk4;
} CDHandler;

/*
    Functions
*/

void GAME_CDLoadWADHeader();
void GAME_CDReadCallback_Func(int status);

/*
    Data
*/

extern CDHandler GAME_CDHandler;
extern int * GAME_CDWAD_headerBuffer;
extern int * GAME_CDReadBuffer;

#endif