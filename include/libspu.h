#ifndef GAME_LIBSPU_H
#define GAME_LIBSPU_H

typedef struct {
    short left;
    short right;
} SpuVolume;

typedef struct {
    SpuVolume volume;
    int reverb;
    int mix;
} SpuExtAttr;

typedef struct {
    unsigned int mask;
    SpuVolume mvol;
    SpuVolume mvolmode;
    SpuVolume mvolx;
    SpuExtAttr cd;
    SpuExtAttr ext;
} SpuCommonAttr;

typedef struct {
    unsigned int voice;
    unsigned int mask;
    SpuVolume volume;
    SpuVolume volmode;
    SpuVolume volumex;
    unsigned short pitch;
    unsigned short note;
    unsigned short sample_note;
    short envx;
    unsigned int addr;
    unsigned int loop_addr;
    int a_mode;
    int s_mode;
    int r_mode;
    unsigned short ar;
    unsigned short dr;
    unsigned short sr;
    unsigned short rr;
    unsigned short sl;
    unsigned short adsr1;
    unsigned short adsr2;
} SpuVoiceAttr;

void LIBSPU_SsUtReverbOff();
unsigned int LIBSPU_SpuSetTransferStartAddr(unsigned int addr);
int LIBSPU_SpuSetTransferMode(int mode);
void LIBSPU_SpuSetCommonAttr(SpuCommonAttr * attr);
unsigned int LIBSPU_SpuRead(unsigned char * addr, unsigned int size);
int LIBSPU_SpuIsTransferCompleted(int flag);
void LIBSPU_SpuSetKey(int on_off, unsigned int voice_bit);
void LIBSPU_SpuSetVoiceAttr(SpuVoiceAttr * arg);
void LIBSPU_SpuStart();

#endif