#ifndef REVOSDK_OS_THREAD
#define REVOSDK_OS_THREAD

#include "revolution/types.h"
#include "revolution/os/OSContext.h"
#include "revolution/os/OSMutex.h"

#define OS_PRIORITY_MIN 0
#define OS_PRIORITY_MAX 31

#define OS_THREAD_STACK_MAGIC 0xDEADBABE

typedef enum {
    OS_THREAD_STATE_EXITED = 0,
    OS_THREAD_STATE_READY = 1,
    OS_THREAD_STATE_RUNNING = 2,
    OS_THREAD_STATE_SLEEPING = 4,
    OS_THREAD_STATE_MORIBUND = 8
} OSThreadState;

typedef enum { OS_THREAD_DETACHED = (1 << 0) } OSThreadFlag;

typedef struct OSThreadQueue {
    struct OSThread* head; // at 0x0
    struct OSThread* tail; // at 0x4
} OSThreadQueue;

typedef struct OSThread {
    OSContext context;
    u16 state;                   // at 0x2C8
    u16 flags;                   // at 0x2CA
    s32 suspend;                 // at 0x2CC
    s32 priority;                // at 0x2D0
    s32 base;                    // at 0x2D4
    u32 val;                     // at 0x2D8
    OSThreadQueue* queue;        // at 0x2DC
    struct OSThread* next;       // at 0x2E0
    struct OSThread* prev;       // at 0x2E4
    OSThreadQueue joinQueue;     // at 0x2E8
    OSMutex* mutex;       // at 0x2F0
    OSMutexQueue mutexQueue;     // at 0x2F4
    struct OSThread* nextActive; // at 0x2FC
    struct OSThread* prevActive; // at 0x300
    u32* stackBegin;             // at 0x304
    u32* stackEnd;               // at 0x308
    s32 error;                   // at 0x30C
    void* specific[2];           // at 0x310
} OSThread;

typedef s32 (* OSThreadFunc)(void *);
typedef void (* OSSwitchThreadFunc)(OSThread *, OSThread *);

void OSYieldThread(void);
bool OSCreateThread(struct OSThread *, OSThreadFunc, void *, void *, s32, s32, s32 prio);

bool OSJoinThread(struct OSThread *, s32);

void OSResumeThread(struct OSThread *);

void OSSleepThread(struct OSThreadQueue *);
void OSWakeupThread(struct OSThreadQueue *);
void OSInitThreadQueue(struct OSThreadQueue *);
OSThread * OSGetCurrentThread(void);

s32 OSSetSwitchThreadCallback(OSSwitchThreadFunc);
bool OSIsThreadTerminated(struct OSThread *);

void OSDetachThread(struct OSThread *);
void OSCancelThread(struct OSThread *);

extern OSThread DefaultThread;

#endif
