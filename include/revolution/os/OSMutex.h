#ifndef REVOSDK_OS_MUTEX
#define REVOSDK_OS_MUTEX

#include "revolution/os/OSThread.h"
#include "revolution/types.h"

typedef struct OSMutex {
    OSThreadQueue queue; // at 0x0
    OSThread* thread; // at 0x8
    s32 lock; // at 0xC
    struct OSMutex* next; // at 0x10
    struct OSMutex* prev; // at 0x14
} OSMutex;

typedef struct OSMutexQueue {
    OSMutex* head; // at 0x0
    OSMutex* tail; // at 0x4
} OSMutexQueue;

void OSLockMutex(struct OSMutex*);
void OSInitMutex(struct OSMutex*);
void OSUnlockMutex(struct OSMutex*);

#endif