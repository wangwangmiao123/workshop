//
// Created by lesiamis on 3/7/17.
//

/* mipsframe.c */
#include "frame.h"


struct F_access_
{
    enum {inFrame, inReg} kind;
    union {
    int offset; /* InFrame */
        Temp_temp reg; /* InReg */
    } u;
};

static F_access InFrame(int offset);
static F_access InReg(Temp_temp reg);