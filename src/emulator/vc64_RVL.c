#include "emulator/simGCN.h"
#include "emulator/THPPlayer.h"
#include "emulator/THPRead.h"
#include "emulator/codeGCN.h"
#include "emulator/frame.h"
#include "emulator/mcardGCN.h"
#include "emulator/movie.h"
#include "emulator/pif.h"
#include "emulator/rom.h"
#include "emulator/soundGCN.h"
#include "emulator/system.h"
#include "emulator/xlCoreGCN.h"
#include "emulator/xlHeap.h"
#include "emulator/xlPostGCN.h"
#include "revolution/gx.h"
#include "macros.h"
#include "string.h"


static char* gaszArgument[12];
System* gpSystem;

static void fn_80007020(void) {

    SYSTEM_FRAME(gpSystem)->aMode[FMT_COMBINE_ALPHA1] = 0;
    SYSTEM_FRAME(gpSystem)->unk_3F230 = -1;
    frameDrawReset(SYSTEM_FRAME(gpSystem), 0x5FFED);

    GXSetZMode(GX_ENABLE, GX_LEQUAL, GX_ENABLE);
    GXSetColorUpdate(GX_ENABLE);
    GXCopyDisp(DemoCurrentBuffer, GX_TRUE);
    GXDrawDone();
    VISetNextFrameBuffer(DemoCurrentBuffer);
    VIFlush();
    VIWaitForRetrace();

    if (DemoCurrentBuffer == DemoFrameBuffer1) {
        DemoCurrentBuffer = DemoFrameBuffer2;
    } else {
        DemoCurrentBuffer = DemoFrameBuffer1;
    }
}

bool simulatorDVDShowError(s32 nStatus, void* anData, s32 nSizeRead, u32 nOffset) {
    return true;
}

bool simulatorDVDOpen(char* szNameFile, DVDFileInfo* pFileInfo) {
    return false;
}

bool simulatorDVDRead(DVDFileInfo* pFileInfo, void* anData, s32 nSizeRead, s32 nOffset, DVDCallback callback) {
    return false;
}

bool simulatorShowLoad(s32 unknown, char* szNameFile, f32 rProgress) { return true; }

static bool simulatorParseArguments(void) {
    char* szText;
    char* szValue;
    s32 iArgument;

    gaszArgument[SAT_NAME] = NULL;
    gaszArgument[SAT_PROGRESSIVE] = NULL;
    gaszArgument[SAT_VIBRATION] = NULL;
    gaszArgument[SAT_CONTROLLER] = NULL;
    gaszArgument[SAT_XTRA] = NULL;
    gaszArgument[SAT_MEMORYCARD] = NULL;
    gaszArgument[SAT_MOVIE] = NULL;
    gaszArgument[SAT_RESET] = NULL;
    gaszArgument[SAT_UNK8] = NULL;
    gaszArgument[SAT_UNK9] = NULL;
    gaszArgument[SAT_UNK10] = NULL;

    iArgument = 0;
    while (iArgument < xlCoreGetArgumentCount()) {
        xlCoreGetArgument(iArgument, &szText);
        iArgument += 1;
        if (szText[0] == '-' || szText[0] == '/' || szText[0] == '\\') {
            if (szText[2] == '\0') {
                xlCoreGetArgument(iArgument, &szValue);
                iArgument += 1;
            } else {
                szValue = &szText[2];
            }

            switch (szText[1]) {
                case 0x4C:
                case 0x6C:
                    gaszArgument[SAT_UNK9] = szValue;
                    break;
                case 0x48:
                case 0x68:
                    gaszArgument[SAT_UNK10] = szValue;
                    break;
                case 0x56:
                case 0x76:
                    gaszArgument[SAT_VIBRATION] = szValue;
                    break;
                case 0x52:
                case 0x72:
                    gaszArgument[SAT_CONTROLLER] = szValue;
                    break;
                case 0x50:
                case 0x70:
                    gaszArgument[SAT_PROGRESSIVE] = szValue;
                    break;
                case 0x47:
                case 0x67:
                    gaszArgument[SAT_XTRA] = szValue;
                    break;
                case 0x43:
                case 0x63:
                    gaszArgument[SAT_MOVIE] = szValue;
                    break;
                case 0x4D:
                case 0x6D:
                    gaszArgument[SAT_RESET] = szValue;
                    break;
                case 0x58:
                case 0x78:
                    gaszArgument[SAT_MEMORYCARD] = szValue;
                    break;
            }
        } else {
            gaszArgument[SAT_NAME] = szText;
        }
    }
    return true;
}

bool simulatorGetArgument(SimulatorArgumentType eType, char** pszArgument) {
    if (eType != SAT_NONE && pszArgument != NULL && gaszArgument[eType] != NULL) {
        *pszArgument = gaszArgument[eType];
        return true;
    }

    return false;
}

static inline bool simulatorRun(SystemMode* peMode) {
    // TODO: fake match?
    int nResult;

    while (systemGetMode(gpSystem, peMode) && *peMode == SM_RUNNING) {
        nResult = systemExecute(gpSystem, 100000);
        if (!nResult) {
            return nResult;
        }
    }

    return true;
}

bool xlMain(void) {
    SystemMode eMode;
    s32 nSize0;
    s32 nSize1;
    GXColor color;
    char acNameROM[32];

    simulatorParseArguments();

    if (!xlHeapGetFree(&nSize0)) {
        return false;
    }

    if (nSize0 > 0x01800000) {
        OSReport("\n\nERROR: This program MUST be run on a system with 24MB (or less) memory!\n");
        OSPanic("vc64_RVL.c", 1352, "       Please reduce memory-size to 24MB (using 'setsmemsize 0x1800000')\n\n");
    }

#ifdef __MWERKS__
    asm {
        li      r3, 0x706
        oris    r3, r3, 0x706
        mtspr   GQR6, r3
        li      r3, 0x507
        oris    r3, r3, 0x507
        mtspr   GQR7, r3
    }
#endif

    VISetBlack(1);
    VIFlush();
    VIWaitForRetrace();

    color.r = color.g = color.b = 0;
    color.a = 255;

    GXSetCopyClear(color, 0xFFFFFF);

    if (!xlHeapGetFree(&nSize0)) {
        return false;
    }

    if (!xlObjectMake((void**)&gpSystem, NULL, &gClassSystem)) {
        return false;
    }

    if (!xlFileSetOpen((CNTOpenCallback)simulatorDVDOpen)) {
        return false;
    }

    if (!xlFileSetRead((CNTReadCallback)simulatorDVDRead)) {
        return false;
    }

    strcpy(acNameROM, "rom");

    if (!romSetImage(SYSTEM_ROM(gpSystem), acNameROM)) {
        return false;
    }

    if (!systemReset(gpSystem)) {
        return false;
    }

    if (!frameShow(SYSTEM_FRAME(gpSystem))) {
        return false;
    }

    if (!xlHeapGetFree(&nSize1)) {
        return false;
    }

    if (!systemSetMode(gpSystem, SM_RUNNING)) {
        return false;
    }

    simulatorRun(&eMode);

    if (!xlObjectFree((void**)&gpSystem)) {
        return false;
    }

    return true;
}
