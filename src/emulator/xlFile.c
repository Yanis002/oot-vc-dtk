#include "emulator/xlFileGCN.h"
#include "dolphin.h"
#include "emulator/xlHeap.h"
#include "emulator/xlObject.h"
#include "revolution/arc/arc.h"
#include "revolution/cnt/cnt.h"

extern ARCHandle lbl_801C9680;
extern u8 lbl_801C96A8[16];

void* fn_800B0DF0(void*, size_t, s32);

_XL_OBJECTTYPE gTypeFile = {
    "FILE",
    sizeof(tXL_FILE),
    NULL,
    (EventFunc)xlFileEvent,
};

static DVDOpenCallback gpfOpen;
static DVDReadCallback gpfRead;

bool xlFileSetOpen(DVDOpenCallback pfOpen) {
    return true;
}

bool xlFileSetRead(DVDReadCallback pfRead) {
    return true;
}

static inline bool xlFileGetFile(tXL_FILE** ppFile, char* szFileName) {
    if (gpfOpen != NULL) {
        return gpfOpen(szFileName, &(*ppFile)->info);
    } else {
        return !ARCGetFile(&lbl_801C9680, szFileName, &(*ppFile)->info);
    }
}

bool xlFileOpen(tXL_FILE** ppFile, XlFileType eType, char* szFileName) {
    if (!xlObjectMake((void**)ppFile, NULL, &gTypeFile)) {
        return false;
    }

    if (xlFileGetFile(ppFile, szFileName)) {
        (*ppFile)->eType = eType;
        (*ppFile)->nSize = contentGetLengthNAND(&(*ppFile)->info);
        (*ppFile)->pData = &(*ppFile)->info;

        return true;
    }

    xlObjectFree((void**)ppFile);
    return false;
}

bool xlFileClose(tXL_FILE** ppFile) {
    if (!xlObjectFree((void**)ppFile)) {
        return false;
    }

    return true;
}

bool xlFileGet(tXL_FILE* pFile, void* pTarget, s32 nSizeBytes) {
    s32 nOffset;
    s32 nOffsetExtra;
    s32 nSize;
    s32 nSizeUsed;

    nOffset = pFile->nOffset;
    nSize = pFile->nSize;
    if (nOffset + nSizeBytes > nSize) {
        nSizeBytes = nSize - nOffset;
    }
    if (nSizeBytes == 0) {
        *(s8*)pTarget = 0xFF;
        return false;
    }

    while (nSizeBytes != 0) {
        if (pFile->unk_24 != -1 && (nOffsetExtra = pFile->nOffset - pFile->unk_24, nOffsetExtra < 0x1000)) {
            nSizeUsed = 0x1000 - nOffsetExtra;
            if (nSizeUsed > nSizeBytes) {
                nSizeUsed = nSizeBytes;
            }
            if (!xlHeapCopy(pTarget, (void*)((u8*)pFile->pBuffer + nOffsetExtra), nSizeUsed)) {
                return false;
            }
            pTarget = (void*)((s32)pTarget + nSizeUsed);
            nSizeBytes -= nSizeUsed;
            pFile->nOffset += nSizeUsed;
        }
        if (nSizeBytes != 0) {
            if (((u32)pFile->pData % 32) == 0 && (nOffset = pFile->nOffset, (nOffset % 4) == 0) && ((u32)nSize % 32) == 0) {
                if (gpfRead != NULL) {
                    gpfRead(pFile->pData, pFile->pBuffer, nSize, nOffset, NULL);
                } else {
                    contentReadNAND(pFile->pData, pFile->pBuffer, nSize, nOffset);
                    // DVDReadPrio(pFile->pData, pFile->pBuffer, nSize, nOffset, 2);
                }
            } else {
                nSize = 0x1000;
                nOffset = pFile->nOffset & ~0x3;
                nOffsetExtra = pFile->nSize - nOffset;
                pFile->unk_24 = nOffset;

                if (nOffsetExtra <= 0x1000) {
                    nSize = (nOffsetExtra + 0x1F) & ~0x1F; // align 32
                }

                if (gpfRead != NULL) {
                    gpfRead(pFile->pData, pFile->pBuffer, nSize, nOffset, NULL);
                } else {
                    contentReadNAND(pFile->pData, pFile->pBuffer, nSize, nOffset);
                    // DVDReadPrio(pFile->pData, pFile->pBuffer, nSize, nOffset, 2);
                }
            }
        }
    }

    return true;
}

bool xlFileSetPosition(tXL_FILE* pFile, s32 nOffset) {
    if ((nOffset >= 0) && (nOffset < pFile->nSize)) {
        pFile->nOffset = nOffset;
        return true;
    }

    return false;
}

static inline bool xlFileEventInline(void) {
    void* buffer;
    void* ret;

    if (!xlHeapTake(&buffer, 0x20000 | 0x70000000)) {
        return false;
    }

    ret = fn_800B0DF0(buffer, 0x20000, 0);
    if (ret == NULL) {
        return false;
    }

    fn_800B165C(&lbl_801C96A8, ret, 4);
    return true;
}

bool xlFileEvent(tXL_FILE* pFile, s32 nEvent, void* pArgument) {
    switch (nEvent) {
        case 0:
            if (!xlFileEventInline()) {
                return false;
            }
            contentInitHandleNAND(5, &lbl_801C9680.hdr, &lbl_801C96A8);
            break;
        case 1:
            contentReleaseHandleNAND(&lbl_801C9680);
            break;
        case 2:
            pFile->nSize = 0;
            pFile->nOffset = 0;
            pFile->pData = NULL;
            pFile->iBuffer = 0;
            pFile->unk_24 = -1;
            if (!xlHeapTake(&pFile->pBuffer, 0x1000 | 0x30000000)) {
                return false;
            }
            break;
        case 3:
            if (pFile->iBuffer != NULL && !xlHeapFree(&pFile->iBuffer)) {
                return false;
            }
            contentCloseNAND(&pFile->info);
            if (!xlHeapFree(&pFile->pBuffer)) {
                return false;
            }
            break;
        case 4:
            break;
        default:
            return false;
    }

    return true;
}
