#ifndef REVOSDK_MEM_HEAP_COMMON_H
#define REVOSDK_MEM_HEAP_COMMON_H

#include "dolphin/types.h"

struct MEMiHeapHead
{
	char UNK_0x0[0x1C];
	u8 * mEndAddress;
};

static u32 MEMGetHeapTotalSize(MEMiHeapHead * pHeap)
{
	return pHeap->mEndAddress - (u8 *)pHeap;
}

static u8 * MEMGetHeapEndAddress(struct MEMiHeapHead*pHeap)
{
	return pHeap->mEndAddress;
}

struct MEMiHeapHead * MEMFindContainHeap(const void *);


#endif