#ifndef _XL_TEXT_H
#define _XL_TEXT_H

#include "revolution/types.h"

s32 xlTextGetLength(char* szTextSource);
s32 xlTextCopy(char* acTextTarget, char* szTextSource);
s32 xlTextAppend(char* acTextTarget, char* szTextSource);

#endif
