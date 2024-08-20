#include "emulator/flash.h"
#include "emulator/ram.h"
#include "emulator/system.h"
#include "emulator/vc64_RVL.h"
#include "emulator/xlHeap.h"
#include "macros.h"

//! TODO: document this
// bool fn_80061B88(void* pBuffer, void* pHeapTarget, s32 arg2, s32 nByteCount);
bool fn_80061BC0(void* pBuffer, void* pHeapTarget, s32 arg2, s32 nByteCount);

static bool flashPut8(Flash* pFLASH, u32 nAddress, s8* pData) { return true; }

static bool flashPut16(Flash* pFLASH, u32 nAddress, s16* pData) { return true; }

static bool flashPut32(Flash* pFLASH, u32 nAddress, s32* pData) {
    void* pRAM;
    char buffer[128];
    u32 nSize;
    s32 i;

    if (*pData == 0) {
        pFLASH->flashStatus = 0;
    }

    switch (*pData & 0xFF000000) {
        case 0xD2000000:
            if (pFLASH->unk_18 == 3) {
                if (!ramGetBuffer(SYSTEM_RAM(gpSystem), &pRAM, pFLASH->nOffsetRAM, &nSize)) {
                    return false;
                }
                if (!fn_80061BC0(pFLASH->flashBuffer, pRAM, pFLASH->unk_14, 0x80)) {
                    return false;
                }
            } else if (pFLASH->unk_18 == 4) {
                for (i = 0; i < ARRAY_COUNT(buffer); i++) {
                    buffer[i] = -1;
                }

                if (pFLASH->unk_14 == 0xFFFFFFFF) {
                    for (i = 0; i < *pFLASH->flashBuffer; i += 0x80) {
                        if (!fn_80061BC0(pFLASH->flashBuffer, buffer, i, 0x80)) {
                            return false;
                        }
                    }
                } else if (!fn_80061BC0(pFLASH->flashBuffer, buffer, pFLASH->unk_14, 0x80)) {
                    return false;
                }
            }

            pFLASH->unk_18 = 0;
            break;
        case 0x0:
            break;
        case 0xE1000000:
            pFLASH->unk_18 = 1;
            pFLASH->flashStatus = 0x11118001;
            break;
        case 0x3C000000:
            pFLASH->unk_14 = -1;
            break;
        case 0x4B000000:
            pFLASH->unk_14 = (*pData << 7) & 0x7FFFFF80;
            break;
        case 0x78000000:
            pFLASH->flashStatus = 0x11118008;
            pFLASH->unk_18 = 4;
            break;
        case 0xB4000000:
            pFLASH->unk_18 = 3;
            break;
        case 0xA5000000:
            pFLASH->unk_18 = 3;
            pFLASH->unk_14 = (*pData << 7) & 0x7FFFFF80;
            pFLASH->flashStatus = 0x11118004;
            break;
        case 0xF0000000:
            pFLASH->flashStatus = 0x11118004;
            pFLASH->unk_18 = 2;
            break;
        default:
            return false;
    }

    pFLASH->flashCommand = *pData;
    return true;
}

static bool flashPut64(Flash* pFLASH, u32 nAddress, s64* pData) { return true; }

static bool flashGet8(Flash* pFLASH, u32 nAddress, s8* pData) { return true; }

static bool flashGet16(Flash* pFLASH, u32 nAddress, s16* pData) { return true; }

static bool flashGet32(Flash* pFLASH, u32 nAddress, s32* pData) {
    if (nAddress == 0x08000000) {
        *pData = pFLASH->flashStatus;
    } else {
        *pData = (nAddress & 0xFFFF) | ((nAddress << 16) & ~0xFFFF);
    }

    return true;
}

static bool flashGet64(Flash* pFLASH, u32 nAddress, s64* pData) { return true; }

static bool flashGetBlock(Flash* pFLASH, CpuBlock* pBlock) {
    void* pRAM;

    if ((pBlock->nAddress0 & 0xFF000000) == 0x08000000) {
        if (!ramGetBuffer(SYSTEM_RAM(gpSystem), &pRAM, pBlock->nAddress1, &pBlock->nSize)) {
            return false;
        }

        if (pFLASH->unk_18 == 1) {
            *((u32*)pRAM + 0) = pFLASH->flashStatus;
            *((u32*)pRAM + 1) = 0xC2001D;
        } else if (pFLASH->unk_18 == 2) {
            fn_80061B88(pFLASH->flashBuffer, pRAM, pBlock->nAddress0 - 0x08000000, pBlock->nSize);
        }
    } else if ((pBlock->nAddress1 & 0xFF000000) == 0x08000000) {
        pFLASH->nOffsetRAM = pBlock->nAddress1;
    }

    if (pBlock->pfUnknown != NULL && !pBlock->pfUnknown(pBlock, 1)) {
        return false;
    }

    return true;
}

bool fn_80045260(Flash* pFLASH, s32 arg1, void* arg2) {
    //! TODO: fake match?
    if (pFLASH == NULL) {
        return false;
    }

    return !!fn_80061B88(pFLASH->flashBuffer, arg2, (arg1 * 8) & 0x7F8, 8);
}

bool fn_800452B0(Flash* pFLASH, s32 arg1, void* arg2) {
    //! TODO: fake match?
    if (pFLASH == NULL) {
        return false;
    }

    return !!fn_80061BC0(pFLASH->flashBuffer, arg2, (arg1 * 8) & 0x7F8, 8);
}

bool fn_80045300(Flash* pFLASH, s32* arg1) {
    *arg1 = pFLASH->unk_00;

    return true;
}

static inline bool flashEvent_UnknownInline(Flash* pFLASH, void* pArgument) {
    if (pFLASH->flashBuffer != NULL && !fn_800618A8(&pFLASH->flashBuffer)) {
        return false;
    }

    if ((s32)pArgument < 0x1000) {
        pArgument = (void*)0x1000;
    } else if ((s32)pArgument < 0x4000) {
        pArgument = (void*)0x4000;
    }

    pFLASH->unk_00 = (u32)pArgument;
    return !!fn_80061770(&pFLASH->flashBuffer, "EEP", gpSystem->eTypeROM, pArgument);
}

bool flashEvent(Flash* pFLASH, s32 nEvent, void* pArgument) {
    switch (nEvent) {
        case 2:
            pFLASH->flashCommand = 0;
            pFLASH->flashBuffer = NULL;

            if (!flashEvent_UnknownInline(pFLASH, pArgument)) {
                return false;
            }

            pFLASH->unk_18 = 0;
            pFLASH->nOffsetRAM = -1;
            break;
        case 3:
            if (pFLASH->flashBuffer != NULL && !fn_800618A8(&pFLASH->flashBuffer)) {
                return false;
            }
            break;
        case 0x1002:
            if (!cpuSetGetBlock(SYSTEM_CPU(gpSystem), pArgument, (GetBlockFunc)flashGetBlock)) {
                return false;
            }
            if (!cpuSetDevicePut(SYSTEM_CPU(gpSystem), pArgument, (Put8Func)flashPut8, (Put16Func)flashPut16,
                                 (Put32Func)flashPut32, (Put64Func)flashPut64)) {
                return false;
            }
            if (!cpuSetDeviceGet(SYSTEM_CPU(gpSystem), pArgument, (Get8Func)flashGet8, (Get16Func)flashGet16,
                                 (Get32Func)flashGet32, (Get64Func)flashGet64)) {
                return false;
            }
        case 0:
        case 1:
            break;
        case 0x1003:
        case 0x1004:
        case 0x1007:
            break;
        default:
            return false;
    }

    return true;
}

_XL_OBJECTTYPE gClassFlash = {
    "FLASH",
    sizeof(Flash),
    NULL,
    (EventFunc)flashEvent,
};
