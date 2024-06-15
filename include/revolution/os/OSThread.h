#ifndef REVOSDK_OS_THREAD
#define REVOSDK_OS_THREAD

#include "dolphin/types.h"

struct OSThread
{
    char UNK_0x0[0x304];
    void *mStackTop;
    void *mStackBottom;
    char UNK_0x30C[0xC];
};

struct OSThreadQueue
{
    struct OSThread *thread_0x0;
    struct OSThread *thread_0x4;
};

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

#endif
