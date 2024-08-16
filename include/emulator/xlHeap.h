#ifndef _XL_HEAP_H
#define _XL_HEAP_H

#include "revolution/types.h"

#define HEAP_COUNT 2

extern s32 gnSizeHeap[HEAP_COUNT];

bool xlHeapTake(void** ppHeap, s32 nByteCount);
bool xlHeapFree(void** ppHeap);
bool xlHeapCompact(s32 iHeap);
bool xlHeapCopy(void* pHeapTarget, void* pHeapSource, s32 nByteCount);
bool xlHeapFill32(void* pHeap, s32 nByteCount, u32 nData);
bool xlHeapGetFree(s32* pnFreeBytes);
bool xlHeapSetup(void);
bool xlHeapReset(void);

#endif
