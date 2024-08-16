#ifndef REVOSDK_AX_AUX_H
#define REVOSDK_AX_AUX_H
#include "AX.h"
#include "dolphin/types.h"

void AXRegisterAuxACallback(AXAuxCallback, s32);
void AXRegisterAuxBCallback(AXAuxCallback, s32);
void AXRegisterAuxCCallback(AXAuxCallback, s32);

void AXGetAuxACallback(AXAuxCallback*, s32*);
void AXGetAuxBCallback(AXAuxCallback*, s32*);
void AXGetAuxCCallback(AXAuxCallback*, s32*);

#endif