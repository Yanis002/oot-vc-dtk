#ifndef REVOSDK_GX_MISC_H
#define REVOSDK_GX_MISC_H
#include "dolphin/types.h"

typedef void (*GXDrawDoneCallback)(void);

void GXFlush(void);

void GXDrawDone(void);

void GXSetDrawDoneCallback(GXDrawDoneCallback);

#endif