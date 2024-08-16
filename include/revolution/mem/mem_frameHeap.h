#ifndef REVOSDK_MEM_FRAME_HEAP_H
#define REVOSDK_MEM_FRAME_HEAP_H

#include "dolphin/types.h"
#include "revolution/mem/mem_heapCommon.h"

MEMiHeapHead* MEMCreateFrmHeapEx(void*, u32, s32);
void MEMDestroyFrmHeap(struct MEMiHeapHead*);
void* MEMAllocFromFrmHeapEx(struct MEMiHeapHead*, u32, s32);
void MEMFreeToFrmHeap(struct MEMiHeapHead*, s32);
s32 MEMGetAllocatableSizeForFrmHeapEx(struct MEMiHeapHead*, s32);

s32 MEMRecordStateForFrmHeap(void*, u32);
void MEMFreeByStateToFrmHeap(void*, s32);
s32 MEMAdjustFrmHeap(struct MEMiHeapHead*);
s32 MEMResizeForMBlockFrmHeap(struct MEMiHeapHead*, void*, u32);

#endif
