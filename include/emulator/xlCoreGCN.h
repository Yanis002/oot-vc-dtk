#ifndef _XL_CORE_GCN_H
#define _XL_CORE_GCN_H

#include "revolution/types.h"
#include "revolution/gx.h"

extern GXRenderModeObj* rmode;

bool xlCoreBeforeRender(void);
bool xlCoreHiResolution(void);
bool xlCoreGetArgument(s32 iArgument, char** pszArgument);
s32 xlCoreGetArgumentCount(void);
bool xlCoreReset(void);
void xlExit(void);

#endif
