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

void option_test_execute()
{
    Color col = {0, 0, 0, 255};
    DrawLine(100, 50, col, 200, menus[OPTIONS_MENU1].d.options_table[MENU1_NUMBERTEST].d.option_number_data->number, col);
}