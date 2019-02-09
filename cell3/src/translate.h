//
// Created by lesiamis on 3/7/17.
//
// To Solve Nested Structure

#ifndef POOL_TRANSLATE_H
#define POOL_TRANSLATE_H

#include "frame.h"

struct Tr_access_ {Tr_level level; F_access access;};

typedef struct Tr_access_ *Tr_access;
typedef ··· Tr_accessList ···
Tr_accessList Tr_AccessList(Tr_access head,
                            Tr_accessList tail);
Tr_level Tr_outermost(void);
Tr_level Tr_newLevel(Tr_level parent, Temp_label name,
                     U_boolList formals);
Tr_accessList Tr_formals(Tr_level level);
Tr_access Tr_allocLocal(Tr_level level, bool escape)

F_frame frame = F_newFrame();



#endif //POOL_TRANSLATE_H
