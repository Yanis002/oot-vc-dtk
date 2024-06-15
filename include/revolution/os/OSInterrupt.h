#ifndef REVOSDK_OS_INTERRUPT
#define REVOSDK_OS_INTERRUPT

#include "dolphin/types.h"

s32 OSDisableInterrupts(void);
void OSRestoreInterrupts(s32);

#endif