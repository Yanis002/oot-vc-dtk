#include "emulator/system.h"
#include "emulator/audio.h"
#include "emulator/codeGCN.h"
#include "emulator/cpu.h"
#include "emulator/disk.h"
#include "emulator/flash.h"
#include "emulator/frame.h"
#include "emulator/library.h"
#include "emulator/mcardGCN.h"
#include "emulator/mips.h"
#include "emulator/peripheral.h"
#include "emulator/pif.h"
#include "emulator/ram.h"
#include "emulator/rdb.h"
#include "emulator/rdp.h"
#include "emulator/rom.h"
#include "emulator/rsp.h"
#include "emulator/serial.h"
#include "emulator/simGCN.h"
#include "emulator/soundGCN.h"
#include "emulator/sram.h"
#include "emulator/video.h"
#include "emulator/xlHeap.h"
#include "macros.h"
#include "revolution/vi.h"
#include "stdlib.h"
#include "string.h"

//! TODO: document
extern _XL_OBJECTTYPE gClassHelpMenu;
extern _XL_OBJECTTYPE gClassController;
extern _XL_OBJECTTYPE gClassVI;
extern _XL_OBJECTTYPE gClassAI;
extern _XL_OBJECTTYPE gClassSI;
extern _XL_OBJECTTYPE gClassEEPROM;

// temp
extern void* lbl_8000A0F0;
extern void* lbl_8000A108;
extern void* lbl_8000A120;
extern void* lbl_8000A138;
extern void* lbl_8000A150;
extern void* lbl_8000A168;
extern void* lbl_8000A184;
extern void* lbl_8000A1A4;
extern void* lbl_8000A1C4;
extern void* lbl_8000A1E4;
extern void* lbl_8000A200;
extern void* lbl_8000A220;
extern void* lbl_8000A234;
extern void* lbl_8000A24C;
extern void* lbl_8000A258;
extern void* lbl_8000A264;

#define MCARD_FILE_NAME ""
#define MCARD_COMMENT ""
#define MCARD_FILE_SIZE 0

// clang-format off
static u32 contMap[][GCN_BTN_COUNT] = {
    // Controller Configuration No. 1
    {
        N64_BTN_A,      // GCN_BTN_A
        N64_BTN_B,      // GCN_BTN_B
        N64_BTN_UNSET,  // GCN_BTN_X
        N64_BTN_UNSET,  // GCN_BTN_Y
        N64_BTN_L,      // GCN_BTN_L
        N64_BTN_R,      // GCN_BTN_R
        N64_BTN_Z,      // GCN_BTN_Z
        N64_BTN_START,  // GCN_BTN_START
        0x08000000,     // GCN_BTN_UNK8
        0x04000000,     // GCN_BTN_UNK9
        0x02000000,     // GCN_BTN_UNK10
        0x01000000,     // GCN_BTN_UNK11
        N64_BTN_DUP,    // GCN_BTN_DPAD_UP
        N64_BTN_DDOWN,  // GCN_BTN_DPAD_DOWN
        N64_BTN_DLEFT,  // GCN_BTN_DPAD_LEFT
        N64_BTN_DRIGHT, // GCN_BTN_DPAD_RIGHT
        N64_BTN_CUP,    // GCN_BTN_CSTICK_UP
        N64_BTN_CDOWN,  // GCN_BTN_CSTICK_DOWN
        N64_BTN_CLEFT,  // GCN_BTN_CSTICK_LEFT
        N64_BTN_CRIGHT, // GCN_BTN_CSTICK_RIGHT
    },
    // Controller Configuration No. 2
    {
        N64_BTN_A,      // GCN_BTN_A
        N64_BTN_B,      // GCN_BTN_B
        N64_BTN_UNSET,  // GCN_BTN_X
        N64_BTN_UNSET,  // GCN_BTN_Y
        N64_BTN_Z,      // GCN_BTN_L
        N64_BTN_R,      // GCN_BTN_R
        N64_BTN_Z,      // GCN_BTN_Z
        N64_BTN_START,  // GCN_BTN_START
        0x08000000,     // GCN_BTN_UNK8
        0x04000000,     // GCN_BTN_UNK9
        0x02000000,     // GCN_BTN_UNK10
        0x01000000,     // GCN_BTN_UNK11
        N64_BTN_DUP,    // GCN_BTN_DPAD_UP
        N64_BTN_DDOWN,  // GCN_BTN_DPAD_DOWN
        N64_BTN_DLEFT,  // GCN_BTN_DPAD_LEFT
        N64_BTN_DRIGHT, // GCN_BTN_DPAD_RIGHT
        N64_BTN_CUP,    // GCN_BTN_CSTICK_UP
        N64_BTN_CDOWN,  // GCN_BTN_CSTICK_DOWN
        N64_BTN_CLEFT,  // GCN_BTN_CSTICK_LEFT
        N64_BTN_CRIGHT, // GCN_BTN_CSTICK_RIGHT
    },
    // Controller Configuration No. 3
    {
        N64_BTN_A,      // GCN_BTN_A
        N64_BTN_B,      // GCN_BTN_B
        N64_BTN_CRIGHT, // GCN_BTN_X
        N64_BTN_CLEFT,  // GCN_BTN_Y
        N64_BTN_Z,      // GCN_BTN_L
        N64_BTN_R,      // GCN_BTN_R
        N64_BTN_CDOWN,  // GCN_BTN_Z
        N64_BTN_START,  // GCN_BTN_START
        0x08000000,     // GCN_BTN_UNK8
        0x04000000,     // GCN_BTN_UNK9
        0x02000000,     // GCN_BTN_UNK10
        0x01000000,     // GCN_BTN_UNK11
        N64_BTN_L,      // GCN_BTN_DPAD_UP
        N64_BTN_L,      // GCN_BTN_DPAD_DOWN
        N64_BTN_L,      // GCN_BTN_DPAD_LEFT
        N64_BTN_L,      // GCN_BTN_DPAD_RIGHT
        N64_BTN_CUP,    // GCN_BTN_CSTICK_UP
        N64_BTN_CDOWN,  // GCN_BTN_CSTICK_DOWN
        N64_BTN_CLEFT,  // GCN_BTN_CSTICK_LEFT
        N64_BTN_CRIGHT, // GCN_BTN_CSTICK_RIGHT
    },
    // Controller Configuration No. 4
    {
        N64_BTN_A,      // GCN_BTN_A
        N64_BTN_B,      // GCN_BTN_B
        N64_BTN_Z,      // GCN_BTN_X
        N64_BTN_Z,      // GCN_BTN_Y
        N64_BTN_Z,      // GCN_BTN_L
        N64_BTN_R,      // GCN_BTN_R
        N64_BTN_L,      // GCN_BTN_Z
        N64_BTN_START,  // GCN_BTN_START
        0x08000000,     // GCN_BTN_UNK8
        0x04000000,     // GCN_BTN_UNK9
        0x02000000,     // GCN_BTN_UNK10
        0x01000000,     // GCN_BTN_UNK11
        N64_BTN_DUP,    // GCN_BTN_DPAD_UP
        N64_BTN_DDOWN,  // GCN_BTN_DPAD_DOWN
        N64_BTN_DLEFT,  // GCN_BTN_DPAD_LEFT
        N64_BTN_DRIGHT, // GCN_BTN_DPAD_RIGHT
        N64_BTN_CUP,    // GCN_BTN_CSTICK_UP
        N64_BTN_CDOWN,  // GCN_BTN_CSTICK_DOWN
        N64_BTN_CLEFT,  // GCN_BTN_CSTICK_LEFT
        N64_BTN_CRIGHT, // GCN_BTN_CSTICK_RIGHT
    }, 
    // Controller Configuration No. 5
    {
        N64_BTN_A,      // GCN_BTN_A
        N64_BTN_B,      // GCN_BTN_B
        N64_BTN_CDOWN,  // GCN_BTN_X
        N64_BTN_CLEFT,  // GCN_BTN_Y
        N64_BTN_Z,      // GCN_BTN_L
        N64_BTN_R,      // GCN_BTN_R
        N64_BTN_CRIGHT, // GCN_BTN_Z
        N64_BTN_START,  // GCN_BTN_START
        0x08000000,     // GCN_BTN_UNK8
        0x04000000,     // GCN_BTN_UNK9
        0x02000000,     // GCN_BTN_UNK10
        0x01000000,     // GCN_BTN_UNK11
        N64_BTN_CUP,    // GCN_BTN_DPAD_UP
        N64_BTN_CUP,    // GCN_BTN_DPAD_DOWN
        N64_BTN_CUP,    // GCN_BTN_DPAD_LEFT
        N64_BTN_CUP,    // GCN_BTN_DPAD_RIGHT
        N64_BTN_CUP,    // GCN_BTN_CSTICK_UP
        N64_BTN_CDOWN,  // GCN_BTN_CSTICK_DOWN
        N64_BTN_CLEFT,  // GCN_BTN_CSTICK_LEFT
        N64_BTN_CRIGHT, // GCN_BTN_CSTICK_RIGHT
    },
};
// clang-format on

SystemDevice gaSystemDevice[] = {
    {
        SOT_HELP,
        &gClassHelpMenu,
        0,
        {{0, 0x00000000, 0x00000000}, {0, 0x00000000, 0x00000000}, {0, 0x00000000, 0x00000000}},
    },
    {
        SOT_FRAME,
        &gClassFrame,
        0,
        {{0, 0x00000000, 0x00000000}, {0, 0x00000000, 0x00000000}, {0, 0x00000000, 0x00000000}},
    },
    {
        SOT_LIBRARY,
        &gClassLibrary,
        0,
        {{0, 0x00000000, 0x00000000}, {0, 0x00000000, 0x00000000}, {0, 0x00000000, 0x00000000}},
    },
    {
        SOT_CODE,
        &gClassCode,
        0,
        {{0, 0x00000000, 0x00000000}, {0, 0x00000000, 0x00000000}, {0, 0x00000000, 0x00000000}},
    },
    {
        SOT_AUDIO,
        &gClassAudio,
        0,
        {{0, 0x00000000, 0x00000000}, {0, 0x00000000, 0x00000000}, {0, 0x00000000, 0x00000000}},
    },
    {
        SOT_VIDEO,
        &gClassVideo,
        0,
        {{0, 0x00000000, 0x00000000}, {0, 0x00000000, 0x00000000}, {0, 0x00000000, 0x00000000}},
    },
    {
        SOT_CONTROLLER,
        &gClassController,
        0,
        {{0, 0x00000000, 0x00000000}, {0, 0x00000000, 0x00000000}, {0, 0x00000000, 0x00000000}},
    },
    {
        SOT_CPU,
        &gClassCPU,
        1,
        {{0, 0x00000000, 0xFFFFFFFF}, {0, 0x00000000, 0x00000000}, {0, 0x00000000, 0x00000000}},
    },
    {
        SOT_RAM,
        &gClassRAM,
        3,
        {{256, 0x00000000, 0x03EFFFFF}, {2, 0x03F00000, 0x03FFFFFF}, {1, 0x04700000, 0x047FFFFF}},
    },
    {
        SOT_RDP,
        &gClassRDP,
        2,
        {{0, 0x04100000, 0x041FFFFF}, {1, 0x04200000, 0x042FFFFF}, {0, 0x00000000, 0x00000000}},
    },
    {
        SOT_RSP,
        &gClassRSP,
        1,
        {{0, 0x04000000, 0x040FFFFF}, {0, 0x00000000, 0x00000000}, {0, 0x00000000, 0x00000000}},
    },
    {
        SOT_MIPS,
        &gClassMips,
        1,
        {{0, 0x04300000, 0x043FFFFF}, {0, 0x00000000, 0x00000000}, {0, 0x00000000, 0x00000000}},
    },
    {
        SOT_VI,
        &gClassVI,
        1,
        {{0, 0x04400000, 0x044FFFFF}, {0, 0x00000000, 0x00000000}, {0, 0x00000000, 0x00000000}},
    },
    {
        SOT_AI,
        &gClassAI,
        1,
        {{0, 0x04500000, 0x045FFFFF}, {0, 0x00000000, 0x00000000}, {0, 0x00000000, 0x00000000}},
    },
    {
        SOT_PERIPHERAL,
        &gClassPeripheral,
        1,
        {{0, 0x04600000, 0x046FFFFF}, {0, 0x00000000, 0x00000000}, {0, 0x00000000, 0x00000000}},
    },
    {
        SOT_SI,
        &gClassSI,
        1,
        {{0, 0x04800000, 0x048FFFFF}, {0, 0x00000000, 0x00000000}, {0, 0x00000000, 0x00000000}},
    },
    {
        SOT_RDB,
        &gClassRdb,
        1,
        {{0, 0x04900000, 0x0490FFFF}, {0, 0x00000000, 0x00000000}, {0, 0x00000000, 0x00000000}},
    },
    {
        SOT_DISK,
        &gClassDisk,
        2,
        {{0, 0x05000000, 0x05FFFFFF}, {1, 0x06000000, 0x06FFFFFF}, {0, 0x00000000, 0x00000000}},
    },
    {
        SOT_ROM,
        &gClassROM,
        2,
        {{0, 0x10000000, 0x1FBFFFFF}, {1, 0x1FF00000, 0x1FF0FFFF}, {0, 0x00000000, 0x00000000}},
    },
    {
        SOT_PIF,
        &gClassPIF,
        1,
        {{0, 0x1FC00000, 0x1FC007FF}, {0, 0x00000000, 0x00000000}, {0, 0x00000000, 0x00000000}},
    },
    {
        SOT_NONE,
        NULL,
        0,
        {{0, 0x00000000, 0x00000000}, {0, 0x00000000, 0x00000000}, {0, 0x00000000, 0x00000000}},
    },
};

s32 lbl_8016FEA0[] = {
    0x3C1A8007, 0x275ACEC0, 0x03400008, 0x00000000, 0x3C010010, 0x012A4824, 0x01214823, 0x3C01A460, 0xAC290000,
    0x3C08A460, 0x8D080010, 0x31080002, 0x5500FFFD, 0x3C08A460, 0x24081000, 0x010B4020, 0x010A4024, 0x3C01A460,
    0xAC280004, 0x3C0A0010, 0x254A0003, 0x3C01A460, 0xAC2A000C, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x3C1A8007, 0x275ACEC0, 0x03400008, 0x00000000,
    0x8D6B0010, 0x316B0001, 0x1560FFF0, 0x00000000, 0x3C0BB000, 0x8D640008, 0x3C010010, 0x02C02825, 0x00812023,
    0x3C016C07, 0x34218965, 0x00A10019, 0x27BDFFE0, 0xAFBF001C, 0xAFB00014, 0x3C1F0010, 0x00001825, 0x00004025,
    0x00804825, 0x240D0020, 0x00001012, 0x24420001, 0x00403825, 0x00405025, 0x00405825, 0x00408025, 0x00403025,
    0x00406025, 0x3C1A8007, 0x275ACEC0, 0x03400008, 0x00000000, 0x00602825, 0x254A0001, 0x3043001F, 0x01A37823,
    0x01E2C006, 0x00627004, 0x01D82025, 0x00C2082B, 0x00A03825, 0x01625826, 0x10200004, 0x02048021, 0x00E2C826,
    0x10000002, 0x03263026, 0x00C43026, 0x25080004, 0x00507826, 0x25290004, 0x151FFFE8, 0x01EC6021, 0x00EA7026,
    0x01CB3821, 0x0206C026, 0x030C8021, 0x3C0BB000, 0x8D680010, 0x14E80006, 0x08018B0A, 0x00000000, 0xAFBAFFF0,
    0x3C1A8006, 0x04110003, 0x00000000, 0x0411FFFF, 0x00000000, 0x3C09A408, 0x8D290000, 0x8FB00014, 0x8FBF001C,
    0x11200006, 0x27BD0020, 0x240A0041, 0x3C01A404, 0xAC2A0010, 0x3C01A408, 0xAC200000, 0x3C0B00AA, 0x356BAAAE,
    0x3C01A404, 0xAC2B0010, 0x3C01A430, 0x24080555, 0xAC28000C, 0x3C01A480, 0xAC200018, 0x3C01A450, 0xAC20000C,
    0x3C01A430, 0x24090800, 0xAC290000, 0x24090002, 0x3C01A460, 0xAC290010, 0x3C08A000, 0x35080300, 0x240917D7,
    0xAD090010, 0xAD140000, 0xAD130004, 0xAD15000C, 0x12600004, 0xAD170014, 0x3C09A600, 0x10000003, 0x25290000,
    0x3C09B000, 0x25290000, 0xAD090008, 0x3C08A400, 0x25080000, 0x21091000, 0x240AFFFF, 0x25080004, 0x1509FFFE,
    0xAD0AFFFC, 0x3C08A400, 0x25081000, 0x21091000, 0x25080004, 0x1509FFFE, 0xAD0AFFFC, 0x3C0AA400, 0x240B17D7,
    0xAD4B1000, 0x3C0BB000, 0x254A1000, 0x8D690008, 0x3C010010, 0x01214823, 0x01200008, 0x00000000, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
};

char lbl_8016FEA0_2[] = "BREAK (CPU)";
char lbl_8016FEA0_3[] = "BREAK (SP)";
char lbl_8016FEA0_4[] = "THREADSTATUS";

void* jumptable_801701C8[] = {
    &lbl_8000A0F0, &lbl_8000A108, &lbl_8000A120, &lbl_8000A138, &lbl_8000A150, &lbl_8000A168,
    &lbl_8000A184, &lbl_8000A1A4, &lbl_8000A1C4, &lbl_8000A1E4, &lbl_8000A200, &lbl_8000A220,
    &lbl_8000A234, &lbl_8000A24C, &lbl_8000A258, &lbl_8000A264,
};

_XL_OBJECTTYPE gClassSystem = {
    "SYSTEM",
    sizeof(System),
    NULL,
    (EventFunc)systemEvent,
}; // size = 0x10

SystemRomConfig gSystemRomConfigurationList[1];
u32 nTickMultiplier = 2;
f32 fTickScale = 1.0;
u32 gnFlagZelda;

#define Z_ICON_PATH "TPL/z_icon.tpl"
#define Z_BNR_PATH "TPL/z_bnr.tpl"

bool systemSetStorageDevice(System* pSystem, SystemObjectType eStorageDevice) {
    // pSystem->storageDevice = eStorageDevice;

    switch (eStorageDevice) {
        case SOT_EEPROM:
            if (!xlObjectMake(&pSystem->apObject[SOT_EEPROM], pSystem, &gClassEEPROM)) {
                return false;
            }

            if (!cpuMapObject(SYSTEM_CPU(gpSystem), pSystem->apObject[SOT_EEPROM], 0x08000000, 0x0801FFFF, 0)) {
                return false;
            }
            break;
        case SOT_SRAM:
            if (!xlObjectMake(&pSystem->apObject[SOT_SRAM], pSystem, &gClassSram)) {
                return false;
            }

            if (!cpuMapObject(SYSTEM_CPU(gpSystem), pSystem->apObject[SOT_SRAM], 0x08000000, 0x08007FFF, 0)) {
                return false;
            }
            break;
        case SOT_FLASH:
            if (!xlObjectMake(&pSystem->apObject[SOT_FLASH], pSystem, &gClassFlash)) {
                return false;
            }

            if (!cpuMapObject(SYSTEM_CPU(gpSystem), pSystem->apObject[SOT_FLASH], 0x08000000, 0x0801FFFF, 0)) {
                return false;
            }
            break;
        default:
            return false;
    }

    NO_INLINE();
    return true;
}

bool systemMakeDevices(System* pSystem, void* pArgument) {
    SystemDevice* pDevice;
    SystemDeviceInfo* pInfo;
    s32 i;
    s32 nSlotUsed;
    SystemObjectType storageDevice;
    void** ppObject;
    s32 iDevice;

    pSystem->apObject[SOT_CPU] = NULL;
    pSystem->apObject[SOT_PIF] = NULL;
    pSystem->apObject[SOT_RAM] = NULL;
    pSystem->apObject[SOT_ROM] = NULL;
    pSystem->apObject[SOT_RSP] = NULL;
    pSystem->apObject[SOT_RDP] = NULL;
    pSystem->apObject[SOT_MIPS] = NULL;
    pSystem->apObject[SOT_DISK] = NULL;
    pSystem->apObject[SOT_AI] = NULL;
    pSystem->apObject[SOT_VI] = NULL;
    pSystem->apObject[SOT_SI] = NULL;
    pSystem->apObject[SOT_PERIPHERAL] = NULL;
    pSystem->apObject[SOT_RDB] = NULL;
    pSystem->apObject[SOT_EEPROM] = NULL;
    pSystem->apObject[SOT_SRAM] = NULL;
    pSystem->apObject[SOT_FLASH] = NULL;
    pSystem->apObject[SOT_CODE] = NULL;
    pSystem->apObject[SOT_HELP] = NULL;
    pSystem->apObject[SOT_LIBRARY] = NULL;
    pSystem->apObject[SOT_FRAME] = NULL;
    pSystem->apObject[SOT_AUDIO] = NULL;
    pSystem->apObject[SOT_VIDEO] = NULL;
    pSystem->apObject[SOT_CONTROLLER] = NULL;

    iDevice = 0;

    while ((pDevice = &gaSystemDevice[iDevice], storageDevice = pDevice->storageDevice, storageDevice) != SOT_NONE) {
        ppObject = &pSystem->apObject[storageDevice];

        if (pSystem->apObject[storageDevice] == NULL) {
            if (!xlObjectMake(ppObject, pArgument, pDevice->pClass)) {
                return false;
            }
        } else {
            return false;
        }

        nSlotUsed = pDevice->nSlotUsed;

        if (nSlotUsed > 0) {
            for (i = 0; i < nSlotUsed; i++) {
                pInfo = &pDevice->aDeviceSlot[i];

                if (storageDevice == SOT_CPU) {
                    if (!cpuMapObject(SYSTEM_CPU(pSystem), pSystem, pInfo->nAddress0, pInfo->nAddress1, pInfo->nType)) {
                        return false;
                    }
                } else {
                    if (!cpuMapObject(SYSTEM_CPU(pSystem), *ppObject, pInfo->nAddress0, pInfo->nAddress1, pInfo->nType)) {
                        return false;
                    }
                }
            }
        }

        iDevice++;
    }

    return true;
}


static bool systemSetupGameRAM(System* pSystem) {
    char* szExtra;
    bool bExpansion;
    s32 nSizeRAM;
    s32 nSizeCacheROM;
    s32 nSizeExtra;
    Rom* pROM;
    u32 nCode;
    u32 iCode;
    u32 anCode[0x100]; // size = 0x400

    bExpansion = false;
    pROM = SYSTEM_ROM(pSystem);

    if (!romCopy(SYSTEM_ROM(gpSystem), anCode, 0x1000, sizeof(anCode), NULL)) {
        return false;
    }

    nCode = 0;
    for (iCode = 0; iCode < ARRAY_COUNT(anCode); iCode++) {
        nCode += anCode[iCode];
    }

    // Ocarina of Time or Majora's Mask
    if (pSystem->eTypeROM == 'NZSJ' || pSystem->eTypeROM == 'NZSE' || pSystem->eTypeROM == 'NZSP') {
        bExpansion = true;
    } else if (nCode == 0x184CED80 || nCode == 0x184CED18 || nCode == 0x7E8BEE60) {
        bExpansion = true;
    }

    // Conker's Bad Fur Day
    if (pSystem->eTypeROM == 'NFUJ' || pSystem->eTypeROM == 'NFUE' || pSystem->eTypeROM == 'NFUP') {
        bExpansion = true;
    }

    if (bExpansion) {
        nSizeRAM = 0x800000;
        nSizeCacheROM = 0x400000;
    } else {
        nSizeRAM = 0x400000;
        nSizeCacheROM = 0x800000;
    }

    if (!ramSetSize(SYSTEM_RAM(gpSystem), nSizeRAM)) {
        return false;
    }

    return !!ramWipe(SYSTEM_RAM(gpSystem));
}

static inline void systemSetControllerConfiguration(SystemRomConfig* pRomConfig, s32 controllerConfig,
                                                    bool bSetControllerConfig) {
    s32 iConfigList;

    pRomConfig->rumbleConfiguration = 0;

    for (iConfigList = 0; iConfigList < 4; iConfigList++) {
        simulatorCopyControllerMap((u32*)pRomConfig->controllerConfiguration[iConfigList],
                                   contMap[((controllerConfig >> (iConfigList * 8)) & 0x7F)]);
        pRomConfig->rumbleConfiguration |= (1 << (iConfigList * 8)) & (controllerConfig >> 7);
    }

    if (bSetControllerConfig) {
        pRomConfig->normalControllerConfig = controllerConfig;
        pRomConfig->currentControllerConfig = controllerConfig;
    }
}

bool systemGetInitialConfiguration(System* pSystem, Rom* pROM, s32 index) {
    char* szText;

    // fake match
    (void)contMap;
    (void)contMap;

    if (!romGetCode(pROM, (s32*)gSystemRomConfigurationList[index].rom)) {
        return false;
    }

    systemSetControllerConfiguration(&gSystemRomConfigurationList[index], 0, false);
    gSystemRomConfigurationList[index].storageDevice = SOT_CPU;

#if VERSION == MQ_J
    if (!simulatorGetArgument(SAT_VIBRATION, &szText) || (*szText == '1')) {
        if (!simulatorGetArgument(SAT_CONTROLLER, &szText) || (*szText == '0')) {
            systemSetControllerConfiguration(&gSystemRomConfigurationList[index], 0x82828282, true);
        } else {
            systemSetControllerConfiguration(&gSystemRomConfigurationList[index], 0x80808080, true);
        }
    } else {
        if (!simulatorGetArgument(SAT_CONTROLLER, &szText) || (*szText == '0')) {
            systemSetControllerConfiguration(&gSystemRomConfigurationList[index], 0x02020202, true);
        } else {
            systemSetControllerConfiguration(&gSystemRomConfigurationList[index], 0, true);
        }
    }
#endif

    if (romTestCode(pROM, "NSME") || romTestCode(pROM, "NSMJ")) {
        // Super Mario 64
        systemSetControllerConfiguration(&gSystemRomConfigurationList[index], 0x01010101, false);

#if VERSION != MQ_J
        gSystemRomConfigurationList[index].storageDevice = SOT_RSP;
#endif

    } else if (romTestCode(pROM, "CZLE") || romTestCode(pROM, "CZLJ")) {
        // Ocarina of Time
        gSystemRomConfigurationList[index].storageDevice = SOT_PIF;

#if VERSION != MQ_J
        if (!simulatorGetArgument(SAT_VIBRATION, &szText) || (*szText == '1')) {
            if (!simulatorGetArgument(SAT_CONTROLLER, &szText) || (*szText == '0')) {
                systemSetControllerConfiguration(&gSystemRomConfigurationList[index], 0x82828282, true);
            } else {
                systemSetControllerConfiguration(&gSystemRomConfigurationList[index], 0x80808080, true);
            }
        } else {
            if (!simulatorGetArgument(SAT_CONTROLLER, &szText) || (*szText == '0')) {
                systemSetControllerConfiguration(&gSystemRomConfigurationList[index], 0x02020202, true);
            } else {
                systemSetControllerConfiguration(&gSystemRomConfigurationList[index], 0, true);
            }
        }
#endif

    } else if (romTestCode(pROM, "NZSJ") || romTestCode(pROM, "NZSE")) {
        // Majora's Mask
        gSystemRomConfigurationList[index].storageDevice = SOT_RAM;

#if VERSION != MQ_J
        if (!simulatorGetArgument(SAT_VIBRATION, &szText) || (*szText == '1')) {
            if (!simulatorGetArgument(SAT_CONTROLLER, &szText) || (*szText == '0')) {
                systemSetControllerConfiguration(&gSystemRomConfigurationList[index], 0x82828282, true);
            } else {
                systemSetControllerConfiguration(&gSystemRomConfigurationList[index], 0x80808080, true);
            }
        } else {
            if (!simulatorGetArgument(SAT_CONTROLLER, &szText) || (*szText == '0')) {
                systemSetControllerConfiguration(&gSystemRomConfigurationList[index], 0x02020202, true);
            } else {
                systemSetControllerConfiguration(&gSystemRomConfigurationList[index], 0, true);
            }
        }
#endif

    } else if (romTestCode(pROM, "NPWE")) {
        // Pilotwings 64
        gSystemRomConfigurationList[index].storageDevice = SOT_RSP;

    } else if (romTestCode(pROM, "NAFJ")) {
        // Animal Forest
        gSystemRomConfigurationList[index].storageDevice = SOT_RAM;

    } else if (romTestCode(pROM, "NCUE")) {
        // Cruis'n
        gSystemRomConfigurationList[index].storageDevice = 20; // bug?

    } else if (romTestCode(pROM, "NKTE")) {
        // Mario Kart 64
        gSystemRomConfigurationList[index].storageDevice = SOT_RSP;

    } else if (romTestCode(pROM, "NFXE") || romTestCode(pROM, "NFXJ")) {
        // Star Fox 64
        gSystemRomConfigurationList[index].storageDevice = SOT_RSP;

    } else if (romTestCode(pROM, "NMVE")) {
        // Mario Party 3
        gSystemRomConfigurationList[index].storageDevice = SOT_FLASH;

    } else if (romTestCode(pROM, "NYSE")) {
        // Yoshi's Story
        gSystemRomConfigurationList[index].storageDevice = SOT_FLASH;

    } else if (romTestCode(pROM, "NTEA")) {
        // 1080° Snowboarding
        gSystemRomConfigurationList[index].storageDevice = SOT_PIF;

    } else if (romTestCode(pROM, "NMQE")) {
        // Paper Mario
        gSystemRomConfigurationList[index].storageDevice = SOT_RAM;

    } else if (romTestCode(pROM, "NPOE")) {
        // Pokémon Stadium
        gSystemRomConfigurationList[index].storageDevice = SOT_RAM;

    } else if (romTestCode(pROM, "NYLJ")) {
        // Panel de Pon 64
        gSystemRomConfigurationList[index].storageDevice = SOT_PIF;

    } else if (romTestCode(pROM, "NN6E") || romTestCode(pROM, "NN6J")) {
        // Dr. Mario 64
        gSystemRomConfigurationList[index].storageDevice = SOT_RSP;

    } else if (romTestCode(pROM, "NBNJ")) {
        // Bakuretsu Muteki Bangaioh
        gSystemRomConfigurationList[index].storageDevice = SOT_FLASH;
    }

    return true;
}

static bool systemSetupGameALL(System* pSystem) {
    s32 nSizeSound;
    s32 iController;
    s32 nSize;
    u32* anMode;
    s32 i;
    u64 nTimeRetrace;
    char acCode[5];
    DVDFileInfo fileInfo;
    Cpu* pCPU;
    Rom* pROM;
    Pif* pPIF;
    s32 defaultConfiguration;

    pCPU = SYSTEM_CPU(gpSystem);
    pROM = SYSTEM_ROM(pSystem);
    pPIF = SYSTEM_PIF(pSystem);

    if (!xlHeapTake((void**)&mCard.saveIcon, gz_iconSize | 0x30000000)) {
        return false;
    }

    if (!xlHeapTake((void**)&mCard.saveBanner, gz_bnrSize | 0x30000000)) {
        return false;
    }

    nSizeSound = 0x2000;
    memset(&defaultConfiguration, 0, 4);
    pSystem->eTypeROM = SRT_UNKNOWN;

    if (!ramGetBuffer(SYSTEM_RAM(pSystem), (void**)&anMode, 0x300, NULL)) {
        return false;
    }

    anMode[0] = 1;
    anMode[1] = 0;
    anMode[2] = 0xB0000000;
    anMode[3] = 0;
    anMode[4] = 0x17D7;
    anMode[5] = 1;

    nTimeRetrace = OSSecondsToTicks(1.0f / 60.0f);

    if (!ramGetSize(SYSTEM_RAM(pSystem), &nSize)) {
        return false;
    }

    anMode[6] = nSize;
    systemGetInitialConfiguration(pSystem, pROM, 0);

    i = 0;
    if (gSystemRomConfigurationList[i].storageDevice & 1) {
        if (!systemSetStorageDevice(pSystem, SOT_SRAM)) {
            return false;
        }
    } else {
        if (gSystemRomConfigurationList[i].storageDevice & 2) {
            if (!systemSetStorageDevice(pSystem, SOT_FLASH)) {
                return false;
            }
        } else {
            if (!systemSetStorageDevice(pSystem, SOT_NONE)) {
                return false;
            }
        }
    }

    if (gSystemRomConfigurationList[i].storageDevice & 4) {
        if (!pifSetEEPROMType(pPIF, CT_4K)) {
            return false;
        }
    } else {
        if (gSystemRomConfigurationList[i].storageDevice & 8) {
            if (!pifSetEEPROMType(pPIF, CT_16K)) {
                return false;
            }
        } else {
            if (!pifSetEEPROMType(pPIF, CT_NONE)) {
                return false;
            }
        }
    }

    if (romTestCode(pROM, "NSME") || romTestCode(pROM, "NSMJ")) {
        // Super Mario 64
        pSystem->eTypeROM = SRT_MARIO;
        nSizeSound = 0x1000;

        if (DVDOpen(Z_ICON_PATH, &fileInfo) == 1 &&
            !simulatorDVDRead(&fileInfo, mCard.saveIcon, (gz_iconSize + 0x1F) & 0xFFFFFFE0, 0, NULL)) {
            return false;
        }

        DVDClose(&fileInfo);
        simulatorUnpackTexPalette((TEXPalette*)mCard.saveIcon);

        if (DVDOpen(Z_BNR_PATH, &fileInfo) == 1 &&
            !simulatorDVDRead(&fileInfo, mCard.saveBanner, (gz_bnrSize + 0x1F) & 0xFFFFFFE0, 0, NULL)) {
            return false;
        }

        DVDClose(&fileInfo);
        simulatorUnpackTexPalette((TEXPalette*)mCard.saveBanner);
        mcardOpen(&mCard, "MARIO", "Mario 64", mCard.saveIcon, mCard.saveBanner, "MARIO",
                  &gSystemRomConfigurationList[i].currentControllerConfig, 0x4000, 0x200);

        if (!cpuSetCodeHack(pCPU, 0x80317938, 0x5420FFFE, 0)) {
            return false;
        }
    } else if (romTestCode(pROM, "CZLE") || romTestCode(pROM, "CZLJ")) {
        // Ocarina of Time
        pSystem->eTypeROM = SRT_ZELDA1;
        nSizeSound = 0x1000;

#if VERSION == MQ_J
        if ((gnFlagZelda & 1)) {
            if (!cpuSetCodeHack(pCPU, 0x80062D64, 0x94639680, -1)) {
                return false;
            }

            if (!cpuSetCodeHack(pCPU, 0x8006E468, 0x97040000, -1)) {
                return false;
            }
        } else {
            if (!cpuSetCodeHack(pCPU, 0x8005BB34, 0x9463D040, -1)) {
                return false;
            }

            if (!cpuSetCodeHack(pCPU, 0x80066658, 0x97040000, -1)) {
                return false;
            }
        }
#else
        if (gnFlagZelda & 1) {
            if (romTestCode(pROM, "CZLE")) {
                if (!cpuSetCodeHack(pCPU, 0x80062D64, 0x94639680, -1)) {
                    return false;
                }

                if (!cpuSetCodeHack(pCPU, 0x8006E468, 0x97040000, -1)) {
                    return false;
                }
            } else {
                if (!cpuSetCodeHack(pCPU, 0x80062D64, 0x94639680, -1)) {
                    return false;
                }

                if (!cpuSetCodeHack(pCPU, 0x8006E468, 0x97040000, -1)) {
                    return false;
                }
            }
        } else {
            if (romTestCode(pROM, "CZLE")) {
                if (!cpuSetCodeHack(pCPU, 0x8005BB14, ((gnFlagZelda & 2) ? 0x9463D040 : 0x9463D000), -1)) {
                    return false;
                }

                if (!cpuSetCodeHack(pCPU, 0x80066638, 0x97040000, -1)) {
                    return false;
                }
            } else {
                if (!cpuSetCodeHack(pCPU, 0x8005BB34, 0x9463D040, -1)) {
                    return false;
                }

                if (!cpuSetCodeHack(pCPU, 0x80066658, 0x97040000, -1)) {
                    return false;
                }
            }
        }
#endif

        if (!(gnFlagZelda & 1)) {
            if (!(gnFlagZelda & 2)) {
                // CE-J/MQ-J?
                if (DVDOpen(Z_ICON_PATH, &fileInfo) == 1 &&
                    !simulatorDVDRead(&fileInfo, mCard.saveIcon, (gz_iconSize + 0x1F) & 0xFFFFFFE0, 0, NULL)) {
                    return false;
                }

                DVDClose(&fileInfo);
                simulatorUnpackTexPalette((TEXPalette*)mCard.saveIcon);

                if (DVDOpen(Z_BNR_PATH, &fileInfo) == 1 &&
                    !simulatorDVDRead(&fileInfo, mCard.saveBanner, (gz_bnrSize + 0x1F) & 0xFFFFFFE0, 0, NULL)) {
                    return false;
                }

                DVDClose(&fileInfo);
                simulatorUnpackTexPalette((TEXPalette*)mCard.saveBanner);
                mcardOpen(&mCard, MCARD_FILE_NAME, MCARD_COMMENT, mCard.saveIcon, mCard.saveBanner, "ZELDAX",
                          &gSystemRomConfigurationList[i].currentControllerConfig, MCARD_FILE_SIZE, 0x8000);
            } else {
                if (DVDOpen(Z_ICON_PATH, &fileInfo) == 1 &&
                    !simulatorDVDRead(&fileInfo, mCard.saveIcon, (gz_iconSize + 0x1F) & 0xFFFFFFE0, 0, NULL)) {
                    return false;
                }

                DVDClose(&fileInfo);
                simulatorUnpackTexPalette((TEXPalette*)mCard.saveIcon);

                if (DVDOpen(Z_BNR_PATH, &fileInfo) == 1 &&
                    !simulatorDVDRead(&fileInfo, mCard.saveBanner, (gz_bnrSize + 0x1F) & 0xFFFFFFE0, 0, NULL)) {
                    return false;
                }

                DVDClose(&fileInfo);
                simulatorUnpackTexPalette((TEXPalette*)mCard.saveBanner);
                mcardOpen(&mCard, MCARD_FILE_NAME, MCARD_COMMENT, mCard.saveIcon, mCard.saveBanner, "ZELDA",
                          &gSystemRomConfigurationList[i].currentControllerConfig, MCARD_FILE_SIZE, 0x8000);
            }
        } else {
            // debug rom?
            if (DVDOpen(Z_ICON_PATH, &fileInfo) == 1 &&
                !simulatorDVDRead(&fileInfo, mCard.saveIcon, (gz_iconSize + 0x1F) & 0xFFFFFFE0, 0, NULL)) {
                return false;
            }

            DVDClose(&fileInfo);
            simulatorUnpackTexPalette((TEXPalette*)mCard.saveIcon);

            if (DVDOpen(Z_BNR_PATH, &fileInfo) == 1 &&
                !simulatorDVDRead(&fileInfo, mCard.saveBanner, (gz_bnrSize + 0x1F) & 0xFFFFFFE0, 0, NULL)) {
                return false;
            }

            DVDClose(&fileInfo);
            simulatorUnpackTexPalette((TEXPalette*)mCard.saveBanner);
            mcardOpen(&mCard, "ZELDAD", "The Legend of Zelda Debug", mCard.saveIcon, mCard.saveBanner, "ZELDAD",
                      &gSystemRomConfigurationList[i].currentControllerConfig, 0xC000, 0x8000);
        }

        pCPU->nCompileFlag |= 0x110;
    } else if (romTestCode(pROM, "NZSJ") || romTestCode(pROM, "NZSE")) {
        // Majora's Mask
        pSystem->eTypeROM = SRT_ZELDA2;
        nSizeSound = 0x1000;

        if (romTestCode(pROM, "NZSJ")) {
            pSystem->bJapaneseVersion = true;
        } else {
            pSystem->bJapaneseVersion = false;
        }

        nTickMultiplier = 2;
        fTickScale = 1.1f;

        if (!ramGetBuffer(SYSTEM_RAM(pSystem), (void**)&anMode, 0x300U, NULL)) {
            return false;
        }

        anMode[4] = 0x17D9;

        if (DVDOpen(Z_ICON_PATH, &fileInfo) == 1 &&
            !simulatorDVDRead(&fileInfo, mCard.saveIcon, (gz_iconSize + 0x1F) & 0xFFFFFFE0, 0, NULL)) {
            return false;
        }

        DVDClose(&fileInfo);
        simulatorUnpackTexPalette((TEXPalette*)mCard.saveIcon);

        if (DVDOpen(Z_BNR_PATH, &fileInfo) == 1 &&
            !simulatorDVDRead(&fileInfo, mCard.saveBanner, (gz_bnrSize + 0x1F) & 0xFFFFFFE0, 0, NULL)) {
            return false;
        }

        DVDClose(&fileInfo);
        simulatorUnpackTexPalette((TEXPalette*)mCard.saveBanner);
        mcardOpen(&mCard, "ZELDA3", "Legend of Zelda", mCard.saveIcon, mCard.saveBanner, "ZELDA3",
                  &gSystemRomConfigurationList[i].currentControllerConfig, 0x24000, 0x20000);

        if (gnFlagZelda & 1) {
            if (cpuSetCodeHack(pCPU, 0x801C6FC0, 0x95630000, -1) == 0) {
                return false;
            }
        } else if (romTestCode(pROM, "NZSJ")) {
            if (!cpuSetCodeHack(pCPU, 0x80179994, 0x95630000, -1)) {
                return false;
            }

            if (!cpuSetCodeHack(pCPU, 0x800BDA84, 0x860C0000, 0x6025)) {
                return false;
            }

            if (!cpuSetCodeHack(pCPU, 0x800BDA88, 0x860D0004, 0x6825)) {
                return false;
            }

            if (!cpuSetCodeHack(pCPU, 0x800BDB0C, 0x86180000, 0xC025)) {
                return false;
            }

            if (!cpuSetCodeHack(pCPU, 0x800BDB20, 0x86190004, 0xC825)) {
                return false;
            }

            if (!cpuSetCodeHack(pCPU, 0x800BDB34, 0x86080002, 0x4025)) {
                return false;
            }

            if (!cpuSetCodeHack(pCPU, 0x800BDB4C, 0x8609FFFA, 0x4825)) {
                return false;
            }

            if (!cpuSetCodeHack(pCPU, 0x800BDB60, 0x860AFFFE, 0x5025)) {
                return false;
            }

            if (!cpuSetCodeHack(pCPU, 0x800BDB94, 0x844EFFFA, 0x7025)) {
                return false;
            }

            if (!cpuSetCodeHack(pCPU, 0x800BDBA8, 0x844FFFFE, 0x7825)) {
                return false;
            }

            if (!cpuSetCodeHack(pCPU, 0x800BDC20, 0x860A0006, 0x5025)) {
                return false;
            }

            if (!cpuSetCodeHack(pCPU, 0x800BDC34, 0x860B000A, 0x5825)) {
                return false;
            }
        } else {
            if (!cpuSetCodeHack(pCPU, 0x80178A80, 0x95630000, -1)) {
                return false;
            }
        }

        pCPU->nCompileFlag |= 0x1010;
    } else if (romTestCode(pROM, "NPWE")) {
        // Pilotwings 64
        if (DVDOpen(Z_ICON_PATH, &fileInfo) == 1 &&
            !simulatorDVDRead(&fileInfo, mCard.saveIcon, (gz_iconSize + 0x1F) & 0xFFFFFFE0, 0, NULL)) {
            return false;
        }

        DVDClose(&fileInfo);
        simulatorUnpackTexPalette((TEXPalette*)mCard.saveIcon);

        if (DVDOpen(Z_BNR_PATH, &fileInfo) == 1 &&
            !simulatorDVDRead(&fileInfo, mCard.saveBanner, (gz_bnrSize + 0x1F) & 0xFFFFFFE0, 0, NULL)) {
            return false;
        }

        DVDClose(&fileInfo);
        simulatorUnpackTexPalette((TEXPalette*)mCard.saveBanner);
        mcardOpen(&mCard, "PILOT", "Pilotwings", mCard.saveIcon, mCard.saveBanner, "PILOT",
                  &gSystemRomConfigurationList[i].currentControllerConfig, 0x4000, 0x200);
    } else if (romTestCode(pROM, "NAFJ")) {
        // Animal Forest
        if (DVDOpen(Z_ICON_PATH, &fileInfo) == 1 &&
            !simulatorDVDRead(&fileInfo, mCard.saveIcon, (gz_iconSize + 0x1F) & 0xFFFFFFE0, 0, NULL)) {
            return false;
        }

        DVDClose(&fileInfo);
        simulatorUnpackTexPalette((TEXPalette*)mCard.saveIcon);

        if (DVDOpen(Z_ICON_PATH, &fileInfo) == 1 &&
            !simulatorDVDRead(&fileInfo, mCard.saveIcon, (gz_iconSize + 0x1F) & 0xFFFFFFE0, 0, NULL)) {
            return false;
        }

        DVDClose(&fileInfo);
        simulatorUnpackTexPalette((TEXPalette*)mCard.saveIcon);
        mcardOpen(&mCard, "AF", "Animal Forest", mCard.saveIcon, mCard.saveBanner, "AF",
                  &gSystemRomConfigurationList[i].currentControllerConfig, 0x24000, 0x20000);
    } else if (romTestCode(pROM, "NBCE")) {
        // Blast Corps
        if (!cpuSetCodeHack(pCPU, 0x80244CFC, 0x1420FFFA, 0)) {
            return false;
        }
    } else if (!romTestCode(pROM, "NBKE") && !romTestCode(pROM, "NGEE") && !romTestCode(pROM, "NFUE")) {
        if (romTestCode(pROM, "NBYE")) {
            // A Bug's Life
            if (!cpuSetCodeHack(pCPU, 0x8007ADD0, 0x1440FFF9, 0)) {
                return false;
            }
        } else if (romTestCode(pROM, "NCUE")) {
            // Cruis'n USA
            if (DVDOpen(Z_ICON_PATH, &fileInfo) == 1 &&
                !simulatorDVDRead(&fileInfo, mCard.saveIcon, (gz_iconSize + 0x1F) & 0xFFFFFFE0, 0, NULL)) {
                return false;
            }

            DVDClose(&fileInfo);
            simulatorUnpackTexPalette((TEXPalette*)mCard.saveIcon);

            if (DVDOpen(Z_ICON_PATH, &fileInfo) == 1 &&
                !simulatorDVDRead(&fileInfo, mCard.saveIcon, (gz_iconSize + 0x1F) & 0xFFFFFFE0, 0, NULL)) {
                return false;
            }

            DVDClose(&fileInfo);
            simulatorUnpackTexPalette((TEXPalette*)mCard.saveIcon);
            mcardOpen(&mCard, "CRUISE", "Cruise 'n USA", mCard.saveIcon, mCard.saveBanner, "CRUISE",
                      &gSystemRomConfigurationList[i].currentControllerConfig, 0x4000, 0x200);

            if (!cpuSetCodeHack(pCPU, 0x80103E0C, 0x1616FFF2, 0)) {
                return false;
            }
        } else if (!romTestCode(pROM, "NDNE")) {
            if (romTestCode(pROM, "NDOE")) {
                // Donkey Kong 64
                if (!cpuSetCodeHack(pCPU, 0x80000A04, 0x1462FFFF, 0)) {
                    return false;
                }
            } else if ((romTestCode(pROM, "NN6E")) || (romTestCode(pROM, "NN6J"))) {
                // Dr. Mario 64
                nTickMultiplier = 3;

                if (!cpuSetCodeHack(pCPU, 0x800005EC, 0x3C028001, -1)) {
                    return false;
                }

                if (romTestCode(pROM, "NN6J")) {
                    if (!cpuSetCodeHack(pCPU, 0x8006D458, 0x0C0189E9, 0x0C0189A3)) {
                        return false;
                    }
                    if (!cpuSetCodeHack(pCPU, 0x8006D664, 0x0C0189E9, 0x0C0189A3)) {
                        return false;
                    }
                    if (!cpuSetCodeHack(pCPU, 0x8006D6D0, 0x0C0189E9, 0x0C0189A3)) {
                        return false;
                    }
                } else {
                    if (!cpuSetCodeHack(pCPU, 0x8006D338, 0x0C0189A9, 0x0C018963)) {
                        return false;
                    }
                    if (!cpuSetCodeHack(pCPU, 0x8006D544, 0x0C0189A9, 0x0C018963)) {
                        return false;
                    }
                    if (!cpuSetCodeHack(pCPU, 0x8006D5B0, 0x0C0189A9, 0x0C018963)) {
                        return false;
                    }
                }

                if (DVDOpen(Z_ICON_PATH, &fileInfo) == 1 &&
                    !simulatorDVDRead(&fileInfo, mCard.saveIcon, (gz_iconSize + 0x1F) & 0xFFFFFFE0, 0, NULL)) {
                    return false;
                }

                DVDClose(&fileInfo);
                simulatorUnpackTexPalette((TEXPalette*)mCard.saveIcon);

                if (DVDOpen(Z_BNR_PATH, &fileInfo) == 1 &&
                    !simulatorDVDRead(&fileInfo, mCard.saveBanner, (gz_bnrSize + 0x1F) & 0xFFFFFFE0, 0, NULL)) {
                    return false;
                }

                DVDClose(&fileInfo);
                simulatorUnpackTexPalette((TEXPalette*)mCard.saveBanner);
                mcardOpen(&mCard, "DRMARIO", "Dr. Mario", mCard.saveIcon, mCard.saveBanner, "DRMARIO",
                          &gSystemRomConfigurationList[i].currentControllerConfig, 0x4000, 0x200);
                gpSystem->eTypeROM = SRT_DRMARIO;
                pCPU->nCompileFlag |= 0x10;
            } else if (romTestCode(pROM, "CFZE")) {
                // F-Zero X
                if (!cpuSetCodeHack(pCPU, 0x800673A8, 0x144DFFFC, 0)) {
                    return false;
                }
                if (!cpuSetCodeHack(pCPU, 0x80067564, 0x144FFFFC, 0)) {
                    return false;
                }
                if (!cpuSetCodeHack(pCPU, 0x8006759C, 0x1459FFFC, 0)) {
                    return false;
                }
            } else if (romTestCode(pROM, "NLRE")) {
                // Lode Runner 3D
                if (!cpuSetCodeHack(pCPU, 0x80097B6C, 0x1443FFF9, 0)) {
                    return false;
                }
                if (!cpuSetCodeHack(pCPU, 0x80097BF4, 0x1443FFF9, 0)) {
                    return false;
                }
                if (!cpuSetCodeHack(pCPU, 0x80096D08, 0x08025B40, 0x1000FFFF)) {
                    return false;
                }
            } else if (romTestCode(pROM, "NMFE")) {
                // Mario Golf
                if (!cpuSetCodeHack(pCPU, 0x800B2DCC, 0x8C430004, -1)) {
                    return false;
                }
                if (!cpuSetCodeHack(pCPU, 0x800B2E70, 0x8C430004, -1)) {
                    return false;
                }
                if (!cpuSetCodeHack(pCPU, 0x80029EB8, 0x8C4252CC, -1)) {
                    return false;
                }
            } else if (romTestCode(pROM, "NKTE")) {
                // Mario Kart 64
                pSystem->eTypeROM = SRT_MARIOKART;

                if (DVDOpen(Z_ICON_PATH, &fileInfo) == 1 &&
                    !simulatorDVDRead(&fileInfo, mCard.saveIcon, (gz_iconSize + 0x1F) & 0xFFFFFFE0, 0, NULL)) {
                    return false;
                }

                DVDClose(&fileInfo);
                simulatorUnpackTexPalette((TEXPalette*)mCard.saveIcon);

                if (DVDOpen(Z_BNR_PATH, &fileInfo) == 1 &&
                    !simulatorDVDRead(&fileInfo, mCard.saveBanner, (gz_bnrSize + 0x1F) & 0xFFFFFFE0, 0, NULL)) {
                    return false;
                }

                DVDClose(&fileInfo);
                simulatorUnpackTexPalette((TEXPalette*)mCard.saveBanner);
                mcardOpen(&mCard, "KART", "Mario Kart", mCard.saveIcon, mCard.saveBanner, "KART",
                          &gSystemRomConfigurationList[i].currentControllerConfig, 0x4000, 0x200);
                pCPU->nCompileFlag |= 0x10;
            } else if (romTestCode(pROM, "NK4E")) {
                // Kirby 64
#if VERSION != MQ_J
                if (!audioEnable(SYSTEM_AUDIO(pSystem), false)) {
                    return false;
                }
#endif

                if (!cpuSetCodeHack(pCPU, 0x80020BCC, 0x8DF80034, -1)) {
                    return false;
                }
                if (!cpuSetCodeHack(pCPU, 0x80020EBC, 0x8DEFF330, -1)) {
                    return false;
                }
            } else if (romTestCode(pROM, "CLBE")) {
                // Mario Party 1
                pSystem->eTypeROM = SRT_MARIOPARTY1;

                if (DVDOpen(Z_ICON_PATH, &fileInfo) == 1 &&
                    !simulatorDVDRead(&fileInfo, mCard.saveIcon, (gz_iconSize + 0x1F) & 0xFFFFFFE0, 0, NULL)) {
                    return false;
                }

                DVDClose(&fileInfo);
                simulatorUnpackTexPalette((TEXPalette*)mCard.saveIcon);

                if (DVDOpen(Z_BNR_PATH, &fileInfo) == 1 &&
                    !simulatorDVDRead(&fileInfo, mCard.saveBanner, (gz_bnrSize + 0x1F) & 0xFFFFFFE0, 0, NULL)) {
                    return false;
                }

                DVDClose(&fileInfo);
                simulatorUnpackTexPalette((TEXPalette*)mCard.saveBanner);
                mcardOpen(&mCard, "MP1", "Mario Party 1", mCard.saveIcon, mCard.saveBanner, "MP1",
                          &gSystemRomConfigurationList[i].currentControllerConfig, 0x4000, 0x200);
            } else if (romTestCode(pROM, "NMWE")) {
                // Mario Party 2
                pSystem->eTypeROM = SRT_MARIOPARTY2;

                if (DVDOpen(Z_ICON_PATH, &fileInfo) == 1 &&
                    !simulatorDVDRead(&fileInfo, mCard.saveIcon, (gz_iconSize + 0x1F) & 0xFFFFFFE0, 0, NULL)) {
                    return false;
                }

                DVDClose(&fileInfo);
                simulatorUnpackTexPalette((TEXPalette*)mCard.saveIcon);

                if (DVDOpen(Z_BNR_PATH, &fileInfo) == 1 &&
                    !simulatorDVDRead(&fileInfo, mCard.saveBanner, (gz_bnrSize + 0x1F) & 0xFFFFFFE0, 0, NULL)) {
                    return false;
                }

                DVDClose(&fileInfo);
                simulatorUnpackTexPalette((TEXPalette*)mCard.saveBanner);
                mcardOpen(&mCard, "MP2", "Mario Party 2", mCard.saveIcon, mCard.saveBanner, "MP2",
                          &gSystemRomConfigurationList[i].currentControllerConfig, 0x4000, 0x200);
            } else if (romTestCode(pROM, "NMVE")) {
                // Mario Party 3
                pSystem->eTypeROM = SRT_MARIOPARTY3;

                if (DVDOpen(Z_ICON_PATH, &fileInfo) == 1 &&
                    !simulatorDVDRead(&fileInfo, mCard.saveIcon, (gz_iconSize + 0x1F) & 0xFFFFFFE0, 0, NULL)) {
                    return false;
                }

                DVDClose(&fileInfo);
                simulatorUnpackTexPalette((TEXPalette*)mCard.saveIcon);

                if (DVDOpen(Z_BNR_PATH, &fileInfo) == 1 &&
                    !simulatorDVDRead(&fileInfo, mCard.saveBanner, (gz_bnrSize + 0x1F) & 0xFFFFFFE0, 0, NULL)) {
                    return false;
                }

                DVDClose(&fileInfo);
                simulatorUnpackTexPalette((TEXPalette*)mCard.saveBanner);
                mcardOpen(&mCard, "MP3", "Mario Party 3", mCard.saveIcon, mCard.saveBanner, "MP3",
                          &gSystemRomConfigurationList[i].currentControllerConfig, 0x4000, 0x800);
            } else if (!romTestCode(pROM, "NM3E") && !romTestCode(pROM, "NRIE")) {
                if (romTestCode(pROM, "NMQE")) {
                    // Paper Mario
                    if (DVDOpen(Z_ICON_PATH, &fileInfo) == 1 &&
                        !simulatorDVDRead(&fileInfo, mCard.saveIcon, (gz_iconSize + 0x1F) & 0xFFFFFFE0, 0, NULL)) {
                        return false;
                    }

                    DVDClose(&fileInfo);
                    simulatorUnpackTexPalette((TEXPalette*)mCard.saveIcon);

                    if (DVDOpen(Z_BNR_PATH, &fileInfo) == 1 &&
                        !simulatorDVDRead(&fileInfo, mCard.saveBanner, (gz_bnrSize + 0x1F) & 0xFFFFFFE0, 0, NULL)) {
                        return false;
                    }

                    DVDClose(&fileInfo);
                    simulatorUnpackTexPalette((TEXPalette*)mCard.saveBanner);

                    mcardOpen(&mCard, "PaperMario", "Paper Mario", mCard.saveIcon, mCard.saveBanner, "PAPERMARIO",
                              &gSystemRomConfigurationList[i].currentControllerConfig, 0x24000, 0x20000);
                } else if (romTestCode(pROM, "NPOE")) {
                    if (DVDOpen(Z_ICON_PATH, &fileInfo) == 1 &&
                        !simulatorDVDRead(&fileInfo, mCard.saveIcon, (gz_iconSize + 0x1F) & 0xFFFFFFE0, 0, NULL)) {
                        return false;
                    }

                    DVDClose(&fileInfo);
                    simulatorUnpackTexPalette((TEXPalette*)mCard.saveIcon);

                    if (DVDOpen(Z_ICON_PATH, &fileInfo) == 1 &&
                        !simulatorDVDRead(&fileInfo, mCard.saveIcon, (gz_iconSize + 0x1F) & 0xFFFFFFE0, 0, NULL)) {
                        return false;
                    }

                    DVDClose(&fileInfo);
                    simulatorUnpackTexPalette((TEXPalette*)mCard.saveIcon);

                    mcardOpen(&mCard, "PokemonStadium", "Pokemon Stadium", mCard.saveIcon, mCard.saveBanner,
                              "POKEMONSTADIUM", &gSystemRomConfigurationList[i].currentControllerConfig, 0x24000,
                              0x20000);
                } else if (romTestCode(pROM, "NQKE")) {
                    if (!cpuSetCodeHack(pCPU, 0x8004989C, 0x1459FFFB, 0)) {
                        return false;
                    }
                    if (!cpuSetCodeHack(pCPU, 0x80049FF0, 0x1608FFFB, 0)) {
                        return false;
                    }
                    if (!cpuSetCodeHack(pCPU, 0x8004A384, 0x15E0FFFB, 0)) {
                        return false;
                    }
                    if (!cpuSetCodeHack(pCPU, 0x8004A97C, 0x15E0FFFB, 0)) {
                        return false;
                    }
                    if (!cpuSetCodeHack(pCPU, 0x80048FF8, 0x1000FFFD, 0x1000FFFF)) {
                        return false;
                    }
                } else if (!romTestCode(pROM, "NRSE") && !romTestCode(pROM, "NRZE")) {
                    if (romTestCode(pROM, "NFXE") || romTestCode(pROM, "NFXJ")) {
                        pSystem->eTypeROM = SRT_STARFOX;

                        if (DVDOpen(Z_ICON_PATH, &fileInfo) == 1 &&
                            !simulatorDVDRead(&fileInfo, mCard.saveIcon, (gz_iconSize + 0x1F) & 0xFFFFFFE0, 0, NULL)) {
                            return false;
                        }

                        DVDClose(&fileInfo);
                        simulatorUnpackTexPalette((TEXPalette*)mCard.saveIcon);

                        if (DVDOpen(Z_BNR_PATH, &fileInfo) == 1 &&
                            !simulatorDVDRead(&fileInfo, mCard.saveBanner, (gz_bnrSize + 0x1F) & 0xFFFFFFE0, 0, NULL)) {
                            return false;
                        }

                        DVDClose(&fileInfo);
                        simulatorUnpackTexPalette((TEXPalette*)mCard.saveBanner);
                        mcardOpen(&mCard, "STARFOX", "Starfox", mCard.saveIcon, mCard.saveBanner, "STARFOX",
                                  &gSystemRomConfigurationList[i].currentControllerConfig, 0x4000, 0x200);
                        pCPU->nCompileFlag |= 0x110;
                    } else if (romTestCode(pROM, "NGUJ")) {
                        if (!cpuSetCodeHack(pCPU, 0x80025D30, 0x3C018006, -1)) {
                            return false;
                        }
                    } else if (romTestCode(pROM, "NALE")) {
                        if (!cpuSetCodeHack(pCPU, 0x8000092C, 0x3C028004, -1)) {
                            return false;
                        }
                        if (!cpuSetCodeHack(pCPU, 0x8002103C, 0x3C028004, -1)) {
                            return false;
                        }
                        if (!cpuSetCodeHack(pCPU, 0x80021048, 0x3C028004, -1)) {
                            return false;
                        }
                        if (!cpuSetCodeHack(pCPU, 0x800A1BB8, 0x1440FFFD, 0)) {
                            return false;
                        }
                        if (!cpuSetCodeHack(pCPU, 0x800A1BE0, 0x1440FFFD, 0)) {
                            return false;
                        }
                        pCPU->nCompileFlag |= 0x110;
                    } else if (romTestCode(pROM, "NTEA")) {
                        pSystem->eTypeROM = SRT_1080;

                        if (!ramGetBuffer(SYSTEM_RAM(pSystem), (void**)&anMode, 0x300U, NULL)) {
                            return false;
                        }

                        anMode[4] = 0x17D7;
                        if (!ramGetBuffer(SYSTEM_RAM(pSystem), (void**)&anMode, 0x200U, NULL)) {
                            return false;
                        }

                        anMode[0] = 0xAC290000;
                        if (!ramGetBuffer(SYSTEM_RAM(pSystem), (void**)&anMode, 0x284U, NULL)) {
                            return false;
                        }

                        anMode[0] = 0x240B17D7;
                        if (!cpuSetCodeHack(pCPU, 0x800F04E8, 0x1218FFFB, 0)) {
                            return false;
                        }

                        if (DVDOpen(Z_ICON_PATH, &fileInfo) == 1 &&
                            !simulatorDVDRead(&fileInfo, mCard.saveIcon, (gz_iconSize + 0x1F) & 0xFFFFFFE0, 0, NULL)) {
                            return false;
                        }

                        DVDClose(&fileInfo);
                        simulatorUnpackTexPalette((TEXPalette*)mCard.saveIcon);

                        if (DVDOpen(Z_BNR_PATH, &fileInfo) == 1 &&
                            !simulatorDVDRead(&fileInfo, mCard.saveBanner, (gz_bnrSize + 0x1F) & 0xFFFFFFE0, 0, NULL)) {
                            return false;
                        }

                        DVDClose(&fileInfo);
                        simulatorUnpackTexPalette((TEXPalette*)mCard.saveBanner);
                        mcardOpen(&mCard, "1080", "1080", mCard.saveIcon, mCard.saveBanner, "1080",
                                  &gSystemRomConfigurationList[i].currentControllerConfig, 0xC000, 0x8000);
                    } else if (!romTestCode(pROM, "NTPE") && !romTestCode(pROM, "NEPE")) {
                        if (romTestCode(pROM, "NM8E")) {
                            if (!cpuSetCodeHack(pCPU, 0x80031D94, 0x8C421F6C, -1)) {
                                return false;
                            }
                        } else if (romTestCode(pROM, "NYLJ")) {
                            pSystem->eTypeROM = SRT_PANEL;
                            if (!cpuSetCodeHack(pCPU, 0x800A58F8, 0x8C62FF8C, -1)) {
                                return false;
                            }

                            if (DVDOpen(Z_ICON_PATH, &fileInfo) == 1 &&
                                !simulatorDVDRead(&fileInfo, mCard.saveIcon, (gz_iconSize + 0x1F) & 0xFFFFFFE0, 0,
                                                  NULL)) {
                                return false;
                            }

                            DVDClose(&fileInfo);
                            simulatorUnpackTexPalette((TEXPalette*)mCard.saveIcon);

                            if (DVDOpen(Z_BNR_PATH, &fileInfo) == 1 &&
                                !simulatorDVDRead(&fileInfo, mCard.saveBanner, (gz_bnrSize + 0x1F) & 0xFFFFFFE0, 0,
                                                  NULL)) {
                                return false;
                            }

                            DVDClose(&fileInfo);
                            simulatorUnpackTexPalette((TEXPalette*)mCard.saveBanner);

                            mcardOpen(&mCard, "PANEL", "Panel de Pon", mCard.saveIcon, mCard.saveBanner, "PANEL",
                                      &gSystemRomConfigurationList[i].currentControllerConfig, 0xC000, 0x8000);
                            pCPU->nCompileFlag |= 0x10;
                        } else if (romTestCode(pROM, "NTUE")) {
                            if (!cpuSetCodeHack(pCPU, 0x8002BDD0, 0xA0000000, 0)) {
                                return false;
                            }
                        } else if (romTestCode(pROM, "NWRE")) {
                            pSystem->eTypeROM = SRT_WAVERACE;

                            if (!cpuSetCodeHack(pCPU, 0x8004795C, 0x1448FFFC, 0)) {
                                return false;
                            }

                            if (!cpuSetCodeHack(pCPU, 0x80047994, 0x144AFFFC, 0)) {
                                return false;
                            }

                            pCPU->nCompileFlag |= 0x10;
                        } else if (romTestCode(pROM, "NYSE")) {
                            if (DVDOpen(Z_ICON_PATH, &fileInfo) == 1 &&
                                !simulatorDVDRead(&fileInfo, mCard.saveIcon, (gz_iconSize + 0x1F) & 0xFFFFFFE0, 0,
                                                  NULL)) {
                                return false;
                            }

                            DVDClose(&fileInfo);
                            simulatorUnpackTexPalette((TEXPalette*)mCard.saveIcon);

                            if (DVDOpen(Z_BNR_PATH, &fileInfo) == 1 &&
                                !simulatorDVDRead(&fileInfo, mCard.saveBanner, (gz_bnrSize + 0x1F) & 0xFFFFFFE0, 0,
                                                  NULL)) {
                                return false;
                            }

                            DVDClose(&fileInfo);
                            simulatorUnpackTexPalette((TEXPalette*)mCard.saveBanner);
                            mcardOpen(&mCard, "YoshiStory", "YoshiStory", mCard.saveIcon, mCard.saveBanner,
                                      "YoshiStory", &gSystemRomConfigurationList[i].currentControllerConfig, 0x4000,
                                      0x800);
                        } else if (romTestCode(pROM, "NBNJ")) {
                            mcardOpen(&mCard, "XXX", "XXX", mCard.saveIcon, mCard.saveBanner, "XXX",
                                      &gSystemRomConfigurationList[i].currentControllerConfig, 0x4000, 0x800);

                            if (!cpuSetCodeHack(pCPU, 0x80000548, 0x08000156, 0x1000FFFF)) {
                                return false;
                            }

                            if (!cpuSetCodeHack(pCPU, 0x80000730, 0x3C02800C, -1)) {
                                return false;
                            }
                        } else if (!romGetCode(pROM, (s32*)acCode)) {
                            return false;
                        }
                    }
                }
            }
        }
    }

    if (!soundSetBufferSize(SYSTEM_SOUND(gpSystem), nSizeSound)) {
        return false;
    }

    pCPU->nTimeRetrace = nTimeRetrace;
    systemSetControllerConfiguration(&gSystemRomConfigurationList[0],
                                     gSystemRomConfigurationList[i].currentControllerConfig, false);

    for (iController = 0; iController < 4; iController++) {
        simulatorSetControllerMap((u32*)gSystemRomConfigurationList[i].controllerConfiguration[iController],
                                  iController);

        if (gSystemRomConfigurationList[i].storageDevice & 0x10) {
            if (!pifSetControllerType(pPIF, iController, 2)) {
                return false;
            }
        } else if ((1 << (iController << 3)) & gSystemRomConfigurationList[0].rumbleConfiguration) {
            if (!pifSetControllerType(pPIF, iController, 3)) {
                return false;
            }
        } else if (!pifSetControllerType(pPIF, iController, 1)) {
            return false;
        }
    }

    if (mCard.saveToggle == 1) {
        systemSetControllerConfiguration(&gSystemRomConfigurationList[0], mCard.file.game.configuration, false);
    }

    return true;
}

static bool systemGetException(System* pSystem, SystemInterruptType eType, SystemException* pException) {
    pException->nMask = 0;
    pException->szType = "";
    pException->eType = eType;
    pException->eCode = CEC_NONE;
    pException->eTypeMips = MIT_NONE;

    switch (eType) {
        case SIT_SW0:
            pException->nMask = 5;
            pException->szType = "SW0";
            pException->eCode = CEC_INTERRUPT;
            break;
        case SIT_SW1:
            pException->nMask = 6;
            pException->szType = "SW1";
            pException->eCode = CEC_INTERRUPT;
            break;
        case SIT_CART:
            pException->nMask = 0xC;
            pException->szType = "CART";
            pException->eCode = CEC_INTERRUPT;
            break;
        case SIT_COUNTER:
            pException->nMask = 0x84;
            pException->szType = "COUNTER";
            pException->eCode = CEC_INTERRUPT;
            break;
        case SIT_RDB:
            pException->nMask = 0x24;
            pException->szType = "RDB";
            pException->eCode = CEC_INTERRUPT;
            break;
        case SIT_SP:
            pException->nMask = 4;
            pException->szType = "SP";
            pException->eTypeMips = MIT_SP;
            pException->eCode = CEC_INTERRUPT;
            break;
        case SIT_SI:
            pException->nMask = 4;
            pException->szType = "SI";
            pException->eTypeMips = MIT_SI;
            pException->eCode = CEC_INTERRUPT;
            break;
        case SIT_AI:
            pException->nMask = 4;
            pException->szType = "AI";
            pException->eTypeMips = MIT_AI;
            pException->eCode = CEC_INTERRUPT;
            break;
        case SIT_VI:
            pException->nMask = 4;
            pException->szType = "VI";
            pException->eTypeMips = MIT_VI;
            pException->eCode = CEC_INTERRUPT;
            break;
        case SIT_PI:
            pException->nMask = 4;
            pException->szType = "PI";
            pException->eTypeMips = MIT_PI;
            pException->eCode = CEC_INTERRUPT;
            break;
        case SIT_DP:
            pException->nMask = 4;
            pException->szType = "DP";
            pException->eTypeMips = MIT_DP;
            pException->eCode = CEC_INTERRUPT;
            break;
        case SIT_CPU_BREAK:
            pException->szType = "BREAK (CPU)";
            pException->eCode = CEC_BREAK;
            break;
        case SIT_SP_BREAK:
            pException->nMask = 4;
            pException->szType = "BREAK (SP)";
            pException->eCode = CEC_INTERRUPT;
            break;
        case SIT_FAULT:
            pException->szType = "FAULT";
            break;
        case SIT_THREADSTATUS:
            pException->szType = "THREADSTATUS";
            break;
        case SIT_PRENMI:
            pException->szType = "PRENMI";
            pException->eCode = CEC_INTERRUPT;
            break;
        default:
            return false;
    }

    return true;
}

static bool systemGet8(System* pSystem, u32 nAddress, s8* pData) {
    s64 pnPC;
    *pData = 0;
    cpuGetXPC(SYSTEM_CPU(gpSystem), &pnPC, NULL, NULL);
    return false;
}

static bool systemGet16(System* pSystem, u32 nAddress, s16* pData) {
    s64 pnPC;
    *pData = 0;
    cpuGetXPC(SYSTEM_CPU(gpSystem), &pnPC, NULL, NULL);
    return false;
}

static bool systemGet32(System* pSystem, u32 nAddress, s32* pData) {
    s64 pnPC;
    *pData = 0;
    cpuGetXPC(SYSTEM_CPU(gpSystem), &pnPC, NULL, NULL);
    return false;
}

static bool systemGet64(System* pSystem, u32 nAddress, s64* pData) {
    s64 pnPC;
    *pData = 0;
    cpuGetXPC(SYSTEM_CPU(gpSystem), &pnPC, NULL, NULL);
    return false;
}

static bool systemPut8(System* pSystem, u32 nAddress, s8* pData) {
    s64 pnPC;
    cpuGetXPC(SYSTEM_CPU(gpSystem), &pnPC, NULL, NULL);
    return false;
}

static bool systemPut16(System* pSystem, u32 nAddress, s16* pData) {
    s64 pnPC;
    cpuGetXPC(SYSTEM_CPU(gpSystem), &pnPC, NULL, NULL);
    return false;
}

static bool systemPut32(System* pSystem, u32 nAddress, s32* pData) {
    s64 pnPC;
    cpuGetXPC(SYSTEM_CPU(gpSystem), &pnPC, NULL, NULL);
    return false;
}

static bool systemPut64(System* pSystem, u32 nAddress, s64* pData) {
    s64 pnPC;
    cpuGetXPC(SYSTEM_CPU(gpSystem), &pnPC, NULL, NULL);
    return false;
}

static bool systemGetBlock(System* pSystem, CpuBlock* pBlock) {
    void* pBuffer;

    if (pBlock->nAddress1 < 0x04000000) {
        if (!ramGetBuffer(SYSTEM_RAM(gpSystem), &pBuffer, pBlock->nAddress1, &pBlock->nSize)) {
            return false;
        }

        xlHeapFill8(pBuffer, pBlock->nSize, 0xFF);
    }

    if (pBlock->pfUnknown != NULL && !pBlock->pfUnknown(pBlock, 1)) {
        return false;
    }

    return true;
}

static inline bool fn_8000A504_UnknownInline(System* pSystem, CpuBlock** pBlock) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (*pBlock == (CpuBlock*)(pSystem->UNKNOWN_78 + (i * 5))) {
            pSystem->storageDevice &= ~(1 << i);
            return true;
        }
    }

    return false;
}

bool fn_8000A504(CpuBlock* pBlock, bool bUnknown) {
    u32 nAddressOffset[32];
    u32 nAddress;
    u32* pnAddress;
    u32 nAddressEnd;
    s32 nCount;
    s32 i;

    if (bUnknown == true) {
        nAddress = pBlock->nAddress1;

        if (nAddress < 0x04000000) {
            nAddressEnd = (nAddress + pBlock->nSize) - 1;

            if (!rspInvalidateCache(SYSTEM_RSP(gpSystem), nAddress, nAddressEnd)) {
                return false;
            }

            if (!frameInvalidateCache(SYSTEM_FRAME(gpSystem), nAddress, nAddressEnd)) {
                return false;
            }

            if (!cpuGetOffsetAddress(SYSTEM_CPU(gpSystem), nAddressOffset, &nCount, pBlock->nAddress1,
                                     pBlock->nSize)) {
                return false;
            }

            for (i = 0, pnAddress = nAddressOffset; i < nCount; pnAddress++, i++) {
                if (!cpuInvalidateCache(SYSTEM_CPU(gpSystem), *pnAddress, (*pnAddress + pBlock->nSize) - 1)) {
                    return false;
                }
            }
        }

        if (pBlock->pNext->pfUnknown != NULL) {
            pBlock->pNext->pfUnknown(pBlock->pNext, bUnknown);
        }

        if (!fn_8000A504_UnknownInline(gpSystem, &pBlock)) {
            return false;
        }
    }

    return true;
}

static inline bool systemGetNewBlock(System* pSystem, CpuBlock** ppBlock) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (!(pSystem->storageDevice & (1 << i))) {
            pSystem->storageDevice |= (1 << i);
            *ppBlock = (CpuBlock*)(pSystem->UNKNOWN_78 + (i * 5));
            return true;
        }
    }

    *ppBlock = NULL;
    return false;
}

bool fn_8000A6A4(System* pSystem, CpuBlock* pBlock) {
    CpuBlock* pNewBlock;

    if (!systemGetNewBlock(pSystem, &pNewBlock)) {
        return false;
    }

    pNewBlock->pNext = pBlock;
    pNewBlock->nSize = pBlock->nSize;
    pNewBlock->pfUnknown = fn_8000A504;
    pNewBlock->nAddress0 = pBlock->nAddress0;
    pNewBlock->nAddress1 = pBlock->nAddress1;

    return !!cpuGetBlock(SYSTEM_CPU(gpSystem), pNewBlock);
}

bool systemSetMode(System* pSystem, SystemMode eMode) {
    if (xlObjectTest(pSystem, &gClassSystem)) {
        pSystem->eMode = eMode;

        if (eMode == SM_STOPPED) {
            pSystem->nAddressBreak = -1;
        }

        return true;
    }

    return false;
}

bool systemGetMode(System* pSystem, SystemMode* peMode) {
    if (xlObjectTest(pSystem, &gClassSystem) && (peMode != NULL)) {
        *peMode = pSystem->eMode;
        return true;
    }

    return false;
}

bool fn_8000A830(System* pSystem, s32 nEvent, void *pArgument) {
    s32 i;

    for (i = 0; i < SOT_COUNT; i++) {
        if (pSystem->apObject[i] != NULL) {
            xlObjectEvent(pSystem->apObject[i], nEvent, pArgument);
        }
    }

    return true;
}

bool fn_8000A8A8(System* pSystem, s32 nEvent, void *pArgument) {
    fn_8000A830(pSystem, 0x1004, NULL);
    fn_8009A5B8(1);
    VIFlush();
    VIWaitForRetrace();
    fn_8008B764();
    fn_800914D0(0x1234);
    return true;
}

static inline bool systemSetRamMode(System* pSystem) {
    s32 nSize;
    u32* anMode;
    Cpu* pCPU;
    s32 nTimeRetrace;

    if (!ramGetBuffer(SYSTEM_RAM(gpSystem), (void**)&anMode, 0x300, NULL)) {
        return false;
    }

    anMode[0] = 1;
    anMode[1] = 0;
    anMode[2] = 0xB0000000;
    anMode[3] = 0;
    anMode[4] = 0x17D5;
    anMode[5] = 1;

    if (!ramGetSize(SYSTEM_RAM(gpSystem), &nSize)) {
        return false;
    }

    anMode[6] = nSize;

    nTimeRetrace = OSSecondsToTicks(1.0f / 60.0f);
    pCPU = SYSTEM_CPU(gpSystem);
    pCPU->UNKNOWN_1229C = nTimeRetrace;
    pCPU->nTimeRetrace = nTimeRetrace;

    return true;
}

bool systemReset(System* pSystem) {
    s64 nPC;
    s32 nOffsetRAM;
    SystemObjectType eObject;
    CpuBlock block;

    pSystem->nAddressBreak = -1;

    if (romGetImage(SYSTEM_ROM(pSystem), NULL)) {
        if (!systemSetupGameRAM(pSystem)) {
            return false;
        }

        if (!romGetPC(SYSTEM_ROM(pSystem), (u64*)&nPC)) {
            return false;
        }

        block.nSize = 0x100000;
        block.pfUnknown = NULL;
        block.nAddress0 = 0x10001000;
        block.nAddress1 = 0x10001000 & 0xFFFFFF;

        if (!fn_8000A6A4(pSystem, &block)) {
            return false;
        }

        if (!cpuReset(SYSTEM_CPU(gpSystem))) {
            return false;
        }

        if (!systemSetRamMode(pSystem)) {
            return false;
        }

        cpuSetXPC(SYSTEM_CPU(gpSystem), nPC, 0, 0);

        if (!systemSetupGameALL(pSystem)) {
            return false;
        }

        for (eObject = 0; eObject < SOT_COUNT; eObject++) {
            if (pSystem->apObject[eObject] != NULL) {
                xlObjectEvent(pSystem->apObject[eObject], 0x1003, NULL);
            }
        }
    }

    return true;
}

static inline bool systemTestClassObject(System* pSystem) {
    if (xlObjectTest(pSystem, &gClassSystem)) {
        pSystem->eMode = SM_STOPPED;
        pSystem->nAddressBreak = -1;

        return true;
    }

    return false;
}

bool systemExecute(System* pSystem, s32 nCount) {
    if (!cpuExecute(SYSTEM_CPU(gpSystem), pSystem->nAddressBreak)) {
        if (!systemTestClassObject(pSystem)) {
            return false;
        }

        return false;
    }

    if (pSystem->nAddressBreak == SYSTEM_CPU(gpSystem)->nPC) {
        if (!systemTestClassObject(pSystem)) {
            return false;
        }
    }

    return true;
}

bool systemCheckInterrupts(System* pSystem) {
    s32 iException;
    s32 nMaskFinal;
    bool bUsed;
    bool bDone;
    SystemException exception;
    CpuExceptionCode eCodeFinal;

    nMaskFinal = 0;
    eCodeFinal = CEC_NONE;
    bDone = false;
    pSystem->bException = false;

    for (iException = 0; iException < ARRAY_COUNT(pSystem->anException); iException++) {
        if (pSystem->anException[iException] != 0) {
            pSystem->bException = true;

            if (!bDone) {
                if (!systemGetException(pSystem, iException, &exception)) {
                    return false;
                }

                bUsed = false;

                if (exception.eCode == 0) {
                    if (cpuTestInterrupt(SYSTEM_CPU(gpSystem), exception.nMask) &&
                        ((exception.eTypeMips == MIT_NONE) ||
                         mipsSetInterrupt(SYSTEM_MIPS(gpSystem), exception.eTypeMips))) {
                        bUsed = true;
                    }
                } else {
                    bDone = true;

                    if (nMaskFinal == 0) {
                        bUsed = true;
                        eCodeFinal = exception.eCode;
                    }
                }

                if (bUsed) {
                    nMaskFinal |= exception.nMask;
                    pSystem->anException[iException] = 0;
                }
            }
        }
    }

    if (nMaskFinal != 0) {
        if (!cpuException(SYSTEM_CPU(gpSystem), CEC_INTERRUPT, nMaskFinal)) {
            return false;
        }
    } else {
        if ((eCodeFinal != CEC_NONE) && !cpuException(SYSTEM_CPU(gpSystem), eCodeFinal, 0)) {
            return false;
        }
    }

    return true;
}

bool systemExceptionPending(System* pSystem, SystemInterruptType nException) {
    if ((nException > -1) && (nException < ARRAY_COUNT(pSystem->anException))) {
        if (pSystem->anException[nException] != 0) {
            return true;
        }

        return false;
    }

    return false;
}

static inline bool systemClearExceptions(System* pSystem) {
    int iException;

    pSystem->bException = false;

    for (iException = 0; iException < 16; iException++) {
        pSystem->anException[iException] = 0;
    }

    return true;
}

static inline bool systemFreeDevices(System* pSystem) {
    int storageDevice; // SystemObjectType

    for (storageDevice = 0; storageDevice < SOT_COUNT; storageDevice++) {
        if (pSystem->apObject[storageDevice] != NULL && !xlObjectFree(&pSystem->apObject[storageDevice])) {
            return false;
        }
    }

    return true;
}

bool systemEvent(System* pSystem, s32 nEvent, void* pArgument) {
    Cpu* pCPU;
    SystemException exception;
    SystemObjectType eObject;
    SystemObjectType storageDevice;

    switch (nEvent) {
        case 2:
            pSystem->storageDevice = SOT_CPU;
            pSystem->eMode = SM_STOPPED;
            pSystem->eTypeROM = SRT_NONE;
            pSystem->nAddressBreak = -1;
            systemClearExceptions(pSystem);
            if (!systemMakeDevices(pSystem, pArgument)) {
                return false;
            }
            break;
        case 3:
            if (!systemFreeDevices(pSystem)) {
                return false;
            }
            break;
        case 0x1001:
            if (!systemGetException(pSystem, (SystemInterruptType)(s32)pArgument, &exception)) {
                return false;
            }
            if (exception.eTypeMips != MIT_NONE) {
                mipsResetInterrupt(SYSTEM_MIPS(gpSystem), exception.eTypeMips);
            }
            break;
        case 0x1000:
            if (((SystemInterruptType)(s32)pArgument > SIT_NONE) && ((SystemInterruptType)(s32)pArgument < SIT_COUNT)) {
                pSystem->bException = true;
                pSystem->anException[(SystemInterruptType)(s32)pArgument]++;
                break;
            }
            return false;
        case 0x1002:
            if (!cpuSetGetBlock(SYSTEM_CPU(gpSystem), pArgument, (GetBlockFunc)systemGetBlock)) {
                return false;
            }
            if (!cpuSetDevicePut(SYSTEM_CPU(gpSystem), pArgument, (Put8Func)systemPut8, (Put16Func)systemPut16,
                                 (Put32Func)systemPut32, (Put64Func)systemPut64)) {
                return false;
            }
            if (!cpuSetDeviceGet(SYSTEM_CPU(gpSystem), pArgument, (Get8Func)systemGet8, (Get16Func)systemGet16,
                                 (Get32Func)systemGet32, (Get64Func)systemGet64)) {
                return false;
            }
            break;
        case 0:
        case 1:
        case 5:
        case 6:
        case 7:
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
