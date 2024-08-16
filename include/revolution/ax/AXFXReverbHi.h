#ifndef REVOSDK_AX_FX_REVERB_HI_H
#define REVOSDK_AX_FX_REVERB_HI_H
#include "dolphin/types.h"

struct AXFXReverbHi {
    char UNK_0x0[0x148];
};

void AXFXReverbHiCallback(void**, struct AXFXReverbHi*);
u32 AXFXReverbHiGetMemSize(struct AXFXReverbHi*);

s32 AXFXReverbHiInit(struct AXFXReverbHi*);
void AXFXReverbHiShutdown(struct AXFXReverbHi*);
s32 AXFXReverbHiSettings(struct AXFXReverbHi*);

#endif