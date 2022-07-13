#ifndef GAME_LIBCD_H
#define GAME_LIBCD_H

typedef void (*CdlCB)(unsigned char,unsigned char *);

#define CdlSetloc 0x02

/*
    Structs
*/

typedef struct {
	unsigned char minute;		/* minute (BCD) */
	unsigned char second;		/* second (BCD) */
	unsigned char sector;		/* sector (BCD) */
	unsigned char track;		/* track (void) */
} CdlLOC;

/*
    Functions
*/

int LIBCD_CdRead(int sectors, unsigned int * buf, int mode);
int LIBCD_CdInit();
CdlCB LIBCD_CdReadCallback(CdlCB func);
int LIBCD_CdStatus(void);
int LIBCD_CdSync(int mode, unsigned char * result);
int LIBCD_CdControl(unsigned char com, unsigned char * param, unsigned char * result);
int LIBCD_CdControlB(unsigned char com, unsigned char * param, unsigned char * result);
void * LIBCD_CdDataCallback(void (*func)());
CdlLOC * LIBCD_CdIntToPos(int i, CdlLOC * p);
int LIBCD_CdSync_(int mode, unsigned char * result);

#endif