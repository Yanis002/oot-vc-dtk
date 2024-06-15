#ifndef REVOSDK_MEM_ALLOCATOR_H
#define REVOSDK_MEM_ALLOCATOR_H

#include "dolphin/types.h"
#include "revolution/mem/mem_heapCommon.h"

typedef void (*MEMAllocatorHeapAllocFunc)(void);
typedef void (*MEMAllocatorHeapFreeFunc)(void);

struct MEMAllocatorFuncs
{
    MEMAllocatorHeapAllocFunc mHeapAllocFunc; // at 0x0
    MEMAllocatorHeapFreeFunc mHeapFreeFunc; // at 0x4
};

struct MEMAllocator
{
    struct MEMAllocatorFuncs * mAllocFuncs; // at 0x0
    struct MEMiHeapHead *mHeapHandle; // at 0x4
    s32 WORD_0x8;
    s32 WORD_0xC;
};

void * MEMAllocFromAllocator(struct MEMAllocator *, s32);
void MEMFreeToAllocator(struct MEMAllocator *, void *);

void MEMInitAllocatorForFrmHeap(struct MEMAllocator *, struct MEMiHeapHead *, s32);

#endif
