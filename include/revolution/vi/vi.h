#ifndef REVOSDK_VI_H
#define REVOSDK_VI_H

#include "dolphin/types.h"
#include "revolution/gx/GXFrameBuf.h"

typedef void (*VIPostRetraceCallback)(void);

enum VITvFormat {
    VI_NTSC,
    VI_PAL,
    VI_MPAL,
    VI_TV_FORMAT_3,
    VI_TV_FORMAT_4,
    VI_EURGB60
};

void VIInit(void);
void VISetBlack(s32);
void VIConfigure(GXRenderModeObj*);
void VIWaitForRetrace(void);

void VISetPostRetraceCallback(VIPostRetraceCallback);

void VIFlush(void);

void* VIGetCurrentFrameBuffer(void);

void VISetNextFrameBuffer(void*);

s32 VIGetRetraceCount(void);
enum VITvFormat VIGetTvFormat(void);

#endif