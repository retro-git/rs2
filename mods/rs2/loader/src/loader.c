#include <sys/types.h>
#include <stdio.h>
#include <libgte.h>
#include <libetc.h>
#include <libgpu.h>
#include <libapi.h>
#include <libds.h>
#include <libcd.h>

// CD specifics
#define CD_SECTOR_SIZE 2048
// Converting bytes to sectors SECTOR_SIZE is defined in words, aka int
#define BtoS(len) ( ( len + CD_SECTOR_SIZE - 1 ) / CD_SECTOR_SIZE ) 

#define VMODE 0                 // Video Mode : 0 : NTSC, 1: PAL
#define SCREENXRES 320          // Screen width
#define SCREENYRES 240          // Screen height
#define CENTERX SCREENXRES/2    // Center of screen on x 
#define CENTERY SCREENYRES/2    // Center of screen on y
#define MARGINX 0                // margins for text display
#define MARGINY 32
#define FONTSIZE 8 * 7           // Text Field Height

DISPENV disp[2];                 // Double buffered DISPENV and DRAWENV
DRAWENV draw[2];
short db = 0;                      // index of which buffer is used, values 0, 1

int i = 0;
void init(void)
{
    ResetGraph(0);                 // Initialize drawing engine with a complete reset (0)
    SetDefDispEnv(&disp[0], 0, 0         , SCREENXRES, SCREENYRES);     // Set display area for both &disp[0] and &disp[1]
    SetDefDispEnv(&disp[1], 0, SCREENYRES, SCREENXRES, SCREENYRES);     // &disp[0] is on top  of &disp[1]
    SetDefDrawEnv(&draw[0], 0, SCREENYRES, SCREENXRES, SCREENYRES);     // Set draw for both &draw[0] and &draw[1]
    SetDefDrawEnv(&draw[1], 0, 0         , SCREENXRES, SCREENYRES);     // &draw[0] is below &draw[1]
    if (VMODE)                  // PAL
    {
        SetVideoMode(MODE_PAL);
        disp[0].screen.y += 8;  // add offset : 240 + 8 + 8 = 256
        disp[1].screen.y += 8;
        }
    SetDispMask(1);                 // Display on screen    
    setRGB0(&draw[0], 50, 50, 50); // set color for first draw area
    setRGB0(&draw[1], 50, 50, 50); // set color for second draw area
    draw[0].isbg = 1;               // set mask for draw areas. 1 means repainting the area with the RGB color each frame 
    draw[1].isbg = 1;
    PutDispEnv(&disp[db]);          // set the disp and draw environnments
    PutDrawEnv(&draw[db]);
    FntLoad(960, 0);                // Load font to vram at 960,0(+128)
    FntOpen(MARGINX, SCREENYRES - MARGINY - FONTSIZE, SCREENXRES - MARGINX * 2, FONTSIZE, 0, 280 ); // FntOpen(x, y, width, height,  black_bg, max. nbr. chars
}

void display(void)
{
    DrawSync(0);                    // Wait for all drawing to terminate
    VSync(0);                       // Wait for the next vertical blank
    PutDispEnv(&disp[db]);          // set alternate disp and draw environnments
    PutDrawEnv(&draw[db]);  
    db = !db;                       // flip db value (0 or 1)
}

void inject(char* file_name, u_char* dst) {
    CdlFILE filePos;
    if (CdSearchFile(&filePos, file_name) == 0) {
        printf("File %s not found\n", file_name);
    }
    else {
        printf("File %s found\n", file_name);
    }
    CdControlB(CdlSetloc, (u_char *)&filePos.pos, 0);
    CdRead(BtoS(filePos.size), dst, CdlModeSpeed);
    CdReadSync(0, 0);
}

int main(void)
{
    init();                         // init display

    printf("LOADING %d\n", VERSION);

    u_char* kernel_free_space_1;
    u_char* kernel_free_space_2;

    if (VERSION == 2) {
        kernel_free_space_1 = (u_char*)0x8000A000;
        kernel_free_space_2 = (u_char*)0x8000c400;
    }
    else if (VERSION == 3) {
        kernel_free_space_1 = (u_char*)0x800096A8;
        kernel_free_space_2 = (u_char*)0x80007526;
    }

    CdInit();
    inject("\\DRAW.BIN;1", kernel_free_space_1);
    inject("\\INPUT.BIN;1", kernel_free_space_2);

    while (1)                       // infinite loop
    {   
        i += 1;
       // printf("%d\n", i);
        FntPrint("rs2-0.0.1-alpha RETRO PRODUCTIONS");  // Send string to print stream
        FntFlush(-1);               // Draw printe stream
        display();                  // Execute display()
        if (i == 100) {
            break;
        }
    }

    ResetGraph(0);
    //CardStop();
    //PadStop();
    StopCallback();
    _96_init();
    LoadExec("cdrom:\\SCUS_944.25;1", 0x801FFFF0, 0);

    return 0;
}
