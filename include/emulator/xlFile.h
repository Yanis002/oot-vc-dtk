#ifndef _XL_FILE_H
#define _XL_FILE_H

#include "revolution/types.h"

bool xlFileGetSize(s32* pnSize, char* szFileName);
bool xlFileReadAll(char* szFileName, void** pTarget);

#endif
