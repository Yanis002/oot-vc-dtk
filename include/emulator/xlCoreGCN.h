#ifndef _XL_CORE_GCN_H
#define _XL_CORE_GCN_H

#include "revolution/gx.h"
#include "revolution/types.h"

bool xlCoreBeforeRender(void);
bool xlCoreHiResolution(void);
bool fn_8007FC84(void);
bool xlCoreGetArgument(s32 iArgument, char** pszArgument);
s32 xlCoreGetArgumentCount(void);
bool xlCoreReset(void);
void xlExit(void);

extern GXRenderModeObj* rmode;

#endif
