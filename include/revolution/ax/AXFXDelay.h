#ifndef REVOSDK_AX_FX_DELAY_H
#define REVOSDK_AX_FX_DELAY_H
#include "dolphin/types.h"


struct AXFXDelay
{
	char UNK_0x0[0x40];
};

void AXFXDelayCallback(void **, struct AXFXDelay *);
u32 AXFXDelayGetMemSize(struct AXFXDelay *);

s32 AXFXDelayInit(struct AXFXDelay *);
void AXFXDelayShutdown(struct AXFXDelay *);
s32 AXFXDelaySettings(struct AXFXDelay *);



#endif