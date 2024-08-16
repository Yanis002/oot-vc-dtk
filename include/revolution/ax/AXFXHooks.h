#ifndef REVOSDK_AX_FX_HOOKS_H
#define REVOSDK_AX_FX_HOOKS_H
#include "dolphin/types.h"

typedef void* (*AXFXAllocHook)(u32);
typedef void (*AXFXFreeHook)(void*);

void AXFXGetHooks(AXFXAllocHook*, AXFXFreeHook*);
void AXFXSetHooks(AXFXAllocHook, AXFXFreeHook);

#endif