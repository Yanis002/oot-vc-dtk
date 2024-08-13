// #include "emulator/xlFile.h"
#include "dolphin/types.h"
#include "emulator/xlFileGCN.h"
#include "emulator/xlHeap.h"
#include "emulator/xlObject.h"

bool xlFileGetSize(s32* pnSize, char* szFileName) {
    tXL_FILE* pFile;

    if (xlFileOpen(&pFile, XLFT_BINARY, szFileName)) {
        if (pnSize != NULL) {
            *pnSize = pFile->nSize;
        }

        if (!xlFileClose(&pFile)) {
            return false;
        }

        return true;
    }

    return false;
}

u32 fn_800820EC(char* szFileName, void* pTarget) {
    tXL_FILE* pFile;
    s32 pnSize;
    s32 temp_r3;

    if (xlFileGetSize(&pnSize, szFileName)) {
        if (!xlHeapTake(&pTarget, pnSize | 0x30000000)) {
            return 0;
        }

        if (!xlFileOpen(&pFile, XLFT_BINARY, szFileName)) {
            return 0;
        }

        if (!xlFileGet(pFile, pTarget, pnSize)) {
            return 0;
        }

        temp_r3 = xlFileClose(&pFile);
        return (u32) (-temp_r3 | temp_r3) >> 0x1F;
    }

    return 0;
}
