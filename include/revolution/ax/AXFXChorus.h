#ifndef REVOSDK_AX_FX_CHORUS
#define REVOSDK_AX_FX_CHORUS
#include "dolphin/types.h"

struct AXFXChorus {
    char UNK_0x0[0xA0];
};

void AXFXChorusCallback(void**, struct AXFXChorus*);
u32 AXFXChorusGetMemSize(struct AXFXChorus*);

s32 AXFXChorusInit(struct AXFXChorus*);
void AXFXChorusShutdown(struct AXFXChorus*);
s32 AXFXChorusSettings(struct AXFXChorus*);

#endif