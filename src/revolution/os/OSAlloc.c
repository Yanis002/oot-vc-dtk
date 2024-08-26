#include "revolution/os.h"
#include "stdint.h"

typedef struct OSHeapDescriptor {
    s32 size; // at 0x0
    struct OSHeapCell* freeList; // at 0x4
    struct OSHeapCell* usedList; // at 0x8
} OSHeapDescriptor;

typedef struct OSHeapCell {
    struct OSHeapCell* prev; // at 0x0
    struct OSHeapCell* next; // at 0x4
    s32 size; // at 0x8
    struct OSHeapDescriptor* hd; // at 0xC
    s32 usedSize; // at 0x10
    char UNK_0x14[0x20 - 0x14];
} OSHeapCell;

volatile s32 __OSCurrHeap = -1;

static void* ArenaEnd = NULL;
static void* ArenaStart = NULL;
static s32 NumHeaps = 0;
static OSHeapDescriptor* HeapArray = NULL;

s32 OSSetCurrentHeap(s32 handle) {
    s32 old = __OSCurrHeap;
    __OSCurrHeap = handle;
    return old;
}

void* OSInitAlloc(void* start, void* end, s32 numHeaps) {
    u32 headerSize;
    int i;

    headerSize = numHeaps * sizeof(OSHeapDescriptor);
    HeapArray = (OSHeapDescriptor*)start;
    NumHeaps = numHeaps;

    for (i = 0; i < NumHeaps; i++) {
        OSHeapDescriptor* hd = &HeapArray[i];
        hd->size = -1;
        hd->usedList = NULL;
        hd->freeList = NULL;
    }

    __OSCurrHeap = -1;
    ArenaStart = ROUND_UP_PTR((u8*)HeapArray + headerSize, 32);
    ArenaEnd = ROUND_DOWN_PTR(end, 32);

    return ArenaStart;
}

s32 OSCreateHeap(void* start, void* end) {
    s32 handle;

    start = ROUND_UP_PTR(start, 32);
    end = ROUND_DOWN_PTR(end, 32);

    for (handle = 0; handle < NumHeaps; handle++) {
        OSHeapCell* cell = (OSHeapCell*)start;
        OSHeapDescriptor* hd = &HeapArray[handle];
        if (hd->size < 0) {
            hd->size = (uintptr_t)end - (uintptr_t)start;

            cell->prev = NULL;
            cell->next = NULL;
            cell->size = hd->size;

            hd->freeList = cell;
            hd->usedList = NULL;
            return handle;
        }
    }

    return -1;
}
