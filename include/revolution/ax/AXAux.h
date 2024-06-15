#ifndef REVOSDK_AX_AUX_H
#define REVOSDK_AX_AUX_H
#include "dolphin/types.h"
#include "AX.h"


void AXRegisterAuxACallback(AXAuxCallback, s32);
void AXRegisterAuxBCallback(AXAuxCallback, s32);
void AXRegisterAuxCCallback(AXAuxCallback, s32);

void AXGetAuxACallback(AXAuxCallback *, s32 *);
void AXGetAuxBCallback(AXAuxCallback *, s32 *);
void AXGetAuxCCallback(AXAuxCallback *, s32 *);


#endif