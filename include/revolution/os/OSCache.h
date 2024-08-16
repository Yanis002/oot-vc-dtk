#ifndef REVOSDK_OS_CACHE
#define REVOSDK_OS_CACHE

#include "dolphin/types.h"

void LCLoadBlocks(void*, void*, u32);
void LCStoreBlocks(void*, void*, u32);
void LCStoreData(void*, void*, u32);

void LCEnable(void);
void LCDisable(void);

u32 LCQueueLength(void);

void DCInvalidateRange(void*, u32);
void DCFlushRange(void*, u32);
void DCFlushRangeNoSync(void*, u32);
void DCStoreRange(void*, u32);
void DCStoreRangeNoSync(void*, u32);

#endif