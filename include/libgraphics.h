#ifndef GAME_LIBGRAPHICS_H
#define GAME_LIBGRAPHICS_H

/*
    Structs
*/

typedef struct {
    short m[3][3];
    long t[3];
} MATRIX;

typedef struct {
    int vx, vy, vz;
    int pad;
} VECTOR;

typedef struct {
    short x, y;
    short w, h;
} RECT;

typedef struct {
    unsigned int * tag;
    unsigned int code[15];
} DR_ENV;

typedef struct {
    unsigned int * tag;
    unsigned int code[2];
} DR_MODE;

typedef struct {
    RECT disp;
    RECT screen;
    unsigned char isinter;
    unsigned char isrgb24;
    unsigned char pad0, pad1;
} DISPENV;

typedef struct {
    RECT clip;
    short ofs[2];
    RECT tw;
    unsigned short tpage;
    unsigned char dtd;
    unsigned char dfe;
    unsigned char isbg;
    unsigned char r0, g0, b0;
    DR_ENV dr_env;
} DRAWENV;

/*
    Functions
*/

int LIBG_ResetGraph(int mode);
void LIBG_SetGraphDebug(int level);
int LIBG_DrawSync(int mode);
void LIBG_InitGeom();
int LIBG_VSync(int mode);
DRAWENV * LIBG_SetDefDrawEnv(DRAWENV * env, int x, int y, int w, int h);
DISPENV * LIBG_SetDefDispEnv(DISPENV * env, int x, int y, int w, int h);
void LIBG_SetDispMask(int mask);
int LIBG_ClearImage(RECT * rect, unsigned char r, unsigned char b, unsigned char g);
int LIBG_LoadImage(RECT * rect, unsigned int * p);
void LIBG_SetDrawMode(DR_MODE * p, int dfe, int dtd, int tpage, RECT * tw);
int LIBG_StoreImage(RECT * rect, unsigned int * p);
int LIBG_MoveImage(RECT * rect, int x, int y);
void LIBG_DrawOTag(unsigned int * ot);
DRAWENV * LIBG_PutDrawEnv(DRAWENV * env);
DISPENV * LIBG_PutDispEnv(DISPENV * env);
MATRIX * LIBG_MulMatrix0(MATRIX * m0, MATRIX * m1, MATRIX * dst);
MATRIX * LIBG_ScaleMatrix(MATRIX * m, VECTOR * v);

#endif