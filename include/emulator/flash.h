#ifndef _FLASH_H
#define _FLASH_H

#include "emulator/xlObject.h"
#include "revolution/types.h"

// __anon_0x7428F
typedef struct Flash {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ u32* flashBuffer;
    /* 0x08 */ s32 flashStatus;
    /* 0x0C */ s32 flashCommand;
    /* 0x10 */ s32 nOffsetRAM;
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ s32 unk_18;
} Flash; // size = 0x1C

bool flashCopyFLASH(Flash* pFLASH, s32 nOffsetRAM, s32 nOffsetFLASH, s32 nSize);
bool flashTransferFLASH(Flash* pFLASH, s32 nOffsetRAM, s32 nOffsetFLASH, s32 nSize);
bool flashEvent(Flash* pFLASH, s32 nEvent, void* pArgument);

extern _XL_OBJECTTYPE gClassFlash;

#endif
