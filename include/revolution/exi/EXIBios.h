#ifndef RVL_SDK_EXI_BIOS_H
#define RVL_SDK_EXI_BIOS_H

#include "revolution/exi/EXICommon.h"
#include "revolution/types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct EXIItem {
    u32 dev; // at 0x0
    EXICallback callback; // at 0x4
} EXIItem;

typedef struct EXIData {
    EXICallback exiCallback; // at 0x0
    EXICallback tcCallback; // at 0x4
    EXICallback extCallback; // at 0x8
    volatile s32 state; // at 0xC
    s32 bytesRead; // at 0x10
    void* buffer; // at 0x14
    u32 dev; // at 0x18
    u32 id; // at 0x1C
    s32 lastInsert; // at 0x20
    s32 numItems; // at 0x24
    EXIItem items[3]; // at 0x28
} EXIData;

bool EXIImm(EXIChannel chan, void* buf, s32 len, u32 type, EXICallback callback);
bool EXIImmEx(EXIChannel chan, void* buf, s32 len, u32 type);
bool EXIDma(EXIChannel chan, void* buf, s32 len, u32 type, EXICallback callback);
bool EXISync(EXIChannel chan);
void EXIClearInterrupts(EXIChannel chan, bool exi, bool tc, bool ext);
EXICallback EXISetExiCallback(EXIChannel chan, EXICallback callback);
void EXIProbeReset(void);
bool EXIProbe(EXIChannel chan);
bool EXIAttach(EXIChannel chan, EXICallback callback);
bool EXIDetach(EXIChannel chan);
bool EXISelect(EXIChannel chan, u32 dev, u32 freq);
bool EXIDeselect(EXIChannel chan);
void EXIInit(void);
bool EXILock(EXIChannel chan, u32 dev, EXICallback callback);
bool EXIUnlock(EXIChannel chan);
s32 EXIGetID(EXIChannel chan, u32 dev, u32* out);

#ifdef __cplusplus
}
#endif

#endif
