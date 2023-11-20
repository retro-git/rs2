#include "loader.h"

// CD specifics
#define CD_SECTOR_SIZE 2048
// Converting bytes to sectors SECTOR_SIZE is defined in words, aka int
#define BtoS(len) ((len + CD_SECTOR_SIZE - 1) / CD_SECTOR_SIZE)

#define VMODE 0                // Video Mode : 0 : NTSC, 1: PAL
#define SCREENXRES 320         // Screen width
#define SCREENYRES 240         // Screen height
#define CENTERX SCREENXRES / 2 // Center of screen on x
#define CENTERY SCREENYRES / 2 // Center of screen on y
#define MARGINX 0              // margins for text display
#define MARGINY 32
#define FONTSIZE 8 * 7 // Text Field Height
#define OTLEN 8        // Ordering table length

DISPENV disp[2]; // Double buffered DISPENV and DRAWENV
DRAWENV draw[2];
short db = 0; // index of which buffer is used, values 0, 1

u_long ot[2][OTLEN];
char primbuff[2][32768];
char *nextpri = primbuff[0];

int err = 0;

int i = 0;
void init(void)
{
    ResetGraph(0);
    SetDefDispEnv(&disp[0], 0, 0, SCREENXRES, SCREENYRES);
    SetDefDispEnv(&disp[1], 0, SCREENYRES, SCREENXRES, SCREENYRES);
    SetDefDrawEnv(&draw[0], 0, SCREENYRES, SCREENXRES, SCREENYRES);
    SetDefDrawEnv(&draw[1], 0, 0, SCREENXRES, SCREENYRES);
    if (VMODE)
    {
        SetVideoMode(MODE_PAL);
        disp[0].screen.y += 8;
        disp[1].screen.y += 8;
    }
    SetDispMask(1);
    setRGB0(&draw[0], 0, 0, 0);
    setRGB0(&draw[1], 0, 0, 0);
    draw[0].isbg = 1;
    draw[1].isbg = 1;
    PutDispEnv(&disp[db]);
    PutDrawEnv(&draw[db]);
    FntLoad(960, 0);
    FntOpen(CENTERX - 85, 15, SCREENXRES - MARGINX * 2, FONTSIZE, 0, 280);
}

void display(void)
{
    DrawSync(0);
    VSync(0);              // Wait for the next vertical blank
    PutDispEnv(&disp[db]); // set alternate disp and draw environnments
    PutDrawEnv(&draw[db]);
    DrawOTag(&ot[db][OTLEN - 1]);
    db = !db; // flip db value (0 or 1)
    nextpri = primbuff[db];
}

void LoadTexture(u_long *tim, TIM_IMAGE *tparam)
{
    OpenTIM(tim);
    ReadTIM(tparam);

    LoadImage(tparam->prect, tparam->paddr);
    DrawSync(0);

    if (tparam->mode & 0x8)
    {
        LoadImage(tparam->crect, tparam->caddr);
        DrawSync(0);
    }
}

void error(char* func_name, char* file_name) {
    char buffer[64];
    sprintf(buffer, "%s failed - %s", func_name, file_name);
    FntPrint(buffer);
    FntFlush(-1);
    display();
    err = 1;
}

void inject(char *file_name, u_char *dst)
{
    CdlFILE filePos;
    if (CdSearchFile(&filePos, file_name) == 0)
    {
        error("CdSearchFile", file_name);
        return;
    }
    printf("Found file '%s' with size: %lu\n", file_name, filePos.size);
    printf("Minute: %d\n", filePos.pos.minute);
    printf("Second: %d\n", filePos.pos.second);
    printf("Sector: %d\n", filePos.pos.sector);
    printf("Track: %d\n", filePos.pos.track);
    if (CdControl(CdlSetloc, (u_char *)&filePos.pos, 0) == 0)
    {
        error("CdControl", file_name);
        return;
    }
    if (CdRead(BtoS(filePos.size), dst, CdlModeSpeed) == 0)
    {
        error("CdRead", file_name);
        return;
    }
    if (CdReadSync(0, 0) == -1)
    {
        error("CdReadSync", file_name);
        return;
    }
}

int main(void)
{
    SPRT *sprt_16b;
    DR_TPAGE *tpage_16b;

    init(); // init display

    printf("LOADING %d\n", VERSION);

    u_char *kernel_free_space_1;
    u_char *kernel_free_space_2;

    if (VERSION == 2)
    {
        kernel_free_space_1 = (u_char *)0x8000A000;
        kernel_free_space_2 = (u_char *)0x8000c400;
    }
    else if (VERSION == 3)
    {
        kernel_free_space_1 = (u_char *)0x800096A8;
        kernel_free_space_2 = (u_char *)0x80007526;
    }

    CdInit();
    inject("\\DRAW.BIN;1", kernel_free_space_1);
    inject("\\INPUT.BIN;1", kernel_free_space_2);

    LoadTexture(_binary____TIM_moneybags_tim_start, &tim_moneybags);

    // MoveImage(tim_moneybags.prect, 0, 0);

    while (1) // infinite loop
    {
        if (err != 1)
        {
            ClearOTagR(ot[db], OTLEN);
            sprt_16b = (SPRT *)nextpri;
            setSprt(sprt_16b);
            setRGB0(sprt_16b, 128, 128, 128);
            setXY0(sprt_16b, 0, 0);
            setWH(sprt_16b, 320, 240);
            setClut(sprt_16b, tim_moneybags.crect->x, tim_moneybags.crect->y);
            addPrim(ot[db], sprt_16b);
            nextpri += sizeof(SPRT);

            tpage_16b = (DR_TPAGE *)nextpri;
            setDrawTPage(tpage_16b, 0, 1,
                         getTPage(tim_moneybags.mode & 0x3, 0,
                                  tim_moneybags.prect->x, tim_moneybags.prect->y));

            addPrim(ot[db], tpage_16b);
            nextpri += sizeof(DR_TPAGE);

            // MoveImage(tim_moneybags.prect, 0, 0);
            // MoveImage(tim_moneybags.prect, 0, SCREENYRES);
            // printf("%d\n", i);
            FntPrint("rs2-1.0 - RETRO PRODUCTIONS"); // Send string to print stream
            FntFlush(-1);                          // Draw printe stream
            display();                             // Execute display()

            i += 1;
            if (i == 120)
            {
                break;
            }
        }
    }

    ResetGraph(0);
    // CardStop();
    // PadStop();
    StopCallback();
    _96_init();
    LoadExec("cdrom:\\SCUS_944.25;1", 0x801FFFF0, 0);

    return 0;
}
