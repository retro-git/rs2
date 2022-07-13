#ifndef GAME_KERNEL_H
#define GAME_KERNEL_H

#include <stdint.h>

/*
    Structs
*/

typedef struct {
    uint32_t ra, sp, s8, s0, s1, s2, s3, s4, s5, s6, s7, gp;
} JmpBuf;

typedef struct {
    HandlerInfo * next;
    void (*handler)(int);
    int (*verifier)();
    uint32_t padding;
} HandlerInfo;

/*
    Functions here are direct calls to the PSX kernel functions
    https://psx-spx.consoledev.net/kernelbios/#bios-function-summary
*/

void LIBK_LoadExec(char * name, unsigned int stackAddr, unsigned int stackSize);
void LIBK_InitHeap(unsigned int * head, unsigned int size);
void LIBK_setExceptionJmpBuf(JmpBuf * jmpBup);

void LIBK_ExitCriticalSection();
void LIBK_ReturnFromException();
void LIBK_ChangeClearPAD(int val);
int LIBK_ChangeClearRCnt(int timer, int flag);
void LIBK_EnterCriticalSection();
void LIBK_ResetEntryInt();

int LIBK_GPU_cw(unsigned int cmd);
void LIBK_DeliverEvent(unsigned int class, unsigned int spec);

int LIBK_open(char * devname, int flag);
int LIBK_close(int fd);
unsigned int LIBK_lseek(int fd, unsigned int offset, int flag);
int LIBK_read(int fd, char * buf, int n);
int LIBK_write(int fd, char * buf, int n);

HandlerInfo * LIBK_sysDeqIntRP(int priority, HandlerInfo * handler);
int LIBK_sysEnqIntRP(int priority, HandlerInfo * handler);

int LIBK_TestEvent(unsigned int event);
unsigned int LIBK_OpenEvent(unsigned int class, unsigned int spec, unsigned int mode, void (*handler)());
int LIBK_EnableEvent(unsigned int event);
int LIBK_CloseEvent(unsigned int event);

#endif