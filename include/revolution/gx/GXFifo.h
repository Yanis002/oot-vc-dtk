#ifndef REVOSDK_GX_FIFO_H
#define REVOSDK_GX_FIFO_H
#include "GX.h"
#include "dolphin/types.h"

typedef struct _GXFifoObj {
    // TO-DO
    char UNK_0x0[0x80];
} GXFifoObj;

void GXGetGPStatus(u8*, u8*, u8*, u8*, u8*);

#endif