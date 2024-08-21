#ifndef _VIDEO_H
#define _VIDEO_H

#include "revolution/types.h"

typedef struct Video {
    /* 0x00 */ s32 unk_00;
} Video; // size = 0x4

bool videoEvent(Video* pVideo, s32 nEvent, void* pArgument);

extern _XL_OBJECTTYPE gClassVideo;

#endif
