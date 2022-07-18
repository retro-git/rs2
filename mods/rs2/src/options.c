#include "options.h"
#include "game.h"
#include "draw.h"
#include "rs2.h"
#include "menus.h"
#include "libcd.h"
#include "libc.h"
#include "spyro.h"
#include "common.h"
#include "vec3.h"
#include "gpu.h"

void option_input_display_execute()
{
    // Input Display
    InputState *currentInput = &GAME_inputStates[0];

    Color colorUnpressed;
    colorUnpressed.r = 0x20;
    colorUnpressed.g = 0x20;
    colorUnpressed.b = 0x20;

    Color colorPressed;
    colorPressed.r = 0x80;
    colorPressed.g = 0x80;
    colorPressed.b = 0x80;

    GAME_GPUChangeTexPage(0xa8);
    DrawRectST(4, 16, FRAME_HEIGHT - 23, FRAME_HEIGHT - 23 + 4, currentInput->current.b.l2 ? colorPressed : colorUnpressed);
    DrawRectST(2, 18, FRAME_HEIGHT - 23 + 5, FRAME_HEIGHT - 23 + 9, currentInput->current.b.l1 ? colorPressed : colorUnpressed);

    DrawRectST(20 + 4, 20 + 16, FRAME_HEIGHT - 23, FRAME_HEIGHT - 23 + 4, currentInput->current.b.r2 ? colorPressed : colorUnpressed);
    DrawRectST(20 + 2, 20 + 18, FRAME_HEIGHT - 23 + 5, FRAME_HEIGHT - 23 + 9, currentInput->current.b.r1 ? colorPressed : colorUnpressed);

    DrawRectST(7, 13, FRAME_HEIGHT - 23 + 10, FRAME_HEIGHT - 23 + 14, currentInput->current.b.dup ? colorPressed : colorUnpressed);
    DrawRectST(13, 19, FRAME_HEIGHT - 23 + 14, FRAME_HEIGHT - 23 + 18, currentInput->current.b.dright ? colorPressed : colorUnpressed);
    DrawRectST(7, 13, FRAME_HEIGHT - 23 + 18, FRAME_HEIGHT - 23 + 22, currentInput->current.b.ddown ? colorPressed : colorUnpressed);
    DrawRectST(1, 7, FRAME_HEIGHT - 23 + 14, FRAME_HEIGHT - 23 + 18, currentInput->current.b.dleft ? colorPressed : colorUnpressed);

    colorPressed.r = 0x00;
    colorPressed.g = 0x80;
    colorPressed.b = 0x00;
    DrawRectST(20 + 7, 20 + 13, FRAME_HEIGHT - 23 + 10, FRAME_HEIGHT - 23 + 14, currentInput->current.b.triangle ? colorPressed : colorUnpressed);
    colorPressed.r = 0x80;
    colorPressed.g = 0x00;
    colorPressed.b = 0x00;
    DrawRectST(20 + 13, 20 + 19, FRAME_HEIGHT - 23 + 14, FRAME_HEIGHT - 23 + 18, currentInput->current.b.circle ? colorPressed : colorUnpressed);
    colorPressed.r = 0x00;
    colorPressed.g = 0x20;
    colorPressed.b = 0x80;
    DrawRectST(20 + 7, 20 + 13, FRAME_HEIGHT - 23 + 18, FRAME_HEIGHT - 23 + 22, currentInput->current.b.cross ? colorPressed : colorUnpressed);
    colorPressed.r = 0x80;
    colorPressed.g = 0x00;
    colorPressed.b = 0x80;
    DrawRectST(20 + 1, 20 + 7, FRAME_HEIGHT - 23 + 14, FRAME_HEIGHT - 23 + 18, currentInput->current.b.square ? colorPressed : colorUnpressed);

    colorUnpressed.r = 0x20;
    colorUnpressed.g = 0x20;
    colorUnpressed.b = 0x20;
    colorPressed.r = 0xff;
    colorPressed.g = 0xff;
    colorPressed.b = 0xff;
    DrawLine(
        20 + 10, FRAME_HEIGHT - 23 + 16, colorUnpressed,
        20 + 10 + (currentInput->rightStickAnalogX - 0x7f) * 9 / 0x80, FRAME_HEIGHT - 23 + 16 + (currentInput->rightStickAnalogY - 0x7f) * 6 / 0x80, colorPressed);
    DrawLine(
        10, FRAME_HEIGHT - 23 + 16, colorUnpressed,
        10 + (currentInput->leftStickAnalogX - 0x7f) * 9 / 0x80, FRAME_HEIGHT - 23 + 16 + (currentInput->leftStickAnalogY - 0x7f) * 6 / 0x80, colorPressed);
}