#ifndef _SYSTEM_H
#define _SYSTEM_H

#include "emulator/cpu.h"
#include "emulator/mips.h"
#include "emulator/xlObject.h"
#include "revolution/types.h"

#define N64_BTN_A (1 << 31)
#define N64_BTN_B (1 << 30)
#define N64_BTN_Z (1 << 29)
#define N64_BTN_START (1 << 28)
#define N64_BTN_DUP (1 << 27)
#define N64_BTN_DDOWN (1 << 26)
#define N64_BTN_DLEFT (1 << 25)
#define N64_BTN_DRIGHT (1 << 24)
#define N64_BTN_L (1 << 21)
#define N64_BTN_R (1 << 20)
#define N64_BTN_CUP (1 << 19)
#define N64_BTN_CDOWN (1 << 18)
#define N64_BTN_CLEFT (1 << 17)
#define N64_BTN_CRIGHT (1 << 16)
#define N64_BTN_UNSET 0

typedef bool (*SystemCopyCallbackFunc)(void);

// note: each stick direction count as an input
typedef enum GcnButton {
    GCN_BTN_A = 0,
    GCN_BTN_B = 1,
    GCN_BTN_X = 2,
    GCN_BTN_Y = 3,
    GCN_BTN_L = 4,
    GCN_BTN_R = 5,
    GCN_BTN_Z = 6,
    GCN_BTN_START = 7,
    GCN_BTN_UNK8 = 8,
    GCN_BTN_UNK9 = 9,
    GCN_BTN_UNK10 = 10,
    GCN_BTN_UNK11 = 11,
    GCN_BTN_DPAD_UP = 12,
    GCN_BTN_DPAD_DOWN = 13,
    GCN_BTN_DPAD_LEFT = 14,
    GCN_BTN_DPAD_RIGHT = 15,
    GCN_BTN_CSTICK_UP = 16,
    GCN_BTN_CSTICK_DOWN = 17,
    GCN_BTN_CSTICK_LEFT = 18,
    GCN_BTN_CSTICK_RIGHT = 19,
    GCN_BTN_COUNT = 20,
} GcnButton;

// __anon_0x394CD
typedef enum SystemMode {
    SM_NONE = -1,
    SM_RUNNING = 0,
    SM_STOPPED = 1,
} SystemMode;

// __anon_0x370F1
typedef enum SystemRomType {
    SRT_NONE = 0,
    SRT_MARIO = 0,
    SRT_WAVERACE = 1,
    SRT_MARIOKART = 2,
    SRT_STARFOX = 3,
    SRT_ZELDA1 = 4,
    SRT_ZELDA2 = 5,
    SRT_1080 = 6,
    SRT_PANEL = 7,
    SRT_MARIOPARTY1 = 8,
    SRT_MARIOPARTY2 = 9,
    SRT_MARIOPARTY3 = 10,
    SRT_DRMARIO = 11,
    SRT_UNKNOWN = 12,
} SystemRomType;

// __anon_0x370F1
typedef enum SystemObjectType {
    SOT_NONE = -1,
    SOT_CPU = 0, // 0x10
    SOT_PIF = 1, // 0x14
    SOT_RAM = 2, // 0x18
    SOT_ROM = 3, // 0x1C
    SOT_RSP = 4, // 0x20
    SOT_RDP = 5, // 0x24
    SOT_MIPS = 6, // 0x28
    SOT_DISK = 7, // 0x2C
    SOT_AI = 8, // 0x30
    SOT_VI = 9, // 0x34
    SOT_SI = 10, // 0x38
    SOT_PERIPHERAL = 11, // 0x3C
    SOT_RDB = 12, // 0x40
    SOT_EEPROM = 13, // 0x44
    SOT_SRAM = 14, // 0x48
    SOT_FLASH = 15, // 0x4C
    SOT_CODE = 16, // 0x50
    SOT_HELP = 17, // 0x54
    SOT_LIBRARY = 18, // 0x58
    SOT_FRAME = 19, // 0x5C
    SOT_AUDIO = 20, // 0x60
    SOT_VIDEO = 21, // 0x64
    SOT_CONTROLLER = 22, // 0x68
    SOT_COUNT = 23,
} SystemObjectType;

// __anon_0x3979C
typedef enum SystemInterruptType {
    SIT_NONE = -1,
    SIT_SW0 = 0,
    SIT_SW1 = 1,
    SIT_CART = 2,
    SIT_COUNTER = 3,
    SIT_RDB = 4,
    SIT_SP = 5,
    SIT_SI = 6,
    SIT_AI = 7,
    SIT_VI = 8,
    SIT_PI = 9,
    SIT_DP = 10,
    SIT_CPU_BREAK = 11,
    SIT_SP_BREAK = 12,
    SIT_FAULT = 13,
    SIT_THREADSTATUS = 14,
    SIT_PRENMI = 15,
    SIT_COUNT = 16,
} SystemInterruptType;

typedef struct SystemDeviceInfo {
    /* 0x00 */ s32 nType;
    /* 0x04 */ u32 nAddress0;
    /* 0x08 */ u32 nAddress1;
} SystemDeviceInfo; // size = 0xC

typedef struct SystemDevice {
    /* 0x00 */ SystemObjectType storageDevice;
    /* 0x04 */ _XL_OBJECTTYPE* pClass;
    /* 0x08 */ s32 nSlotUsed;
    /* 0x0C */ SystemDeviceInfo aDeviceSlot[3];
} SystemDevice; // size = 0x30

// __anon_0x393FF
typedef struct SystemException {
    /* 0x00 */ char* szType;
    /* 0x04 */ u32 nMask;
    /* 0x08 */ CpuExceptionCode eCode;
    /* 0x10 */ MipsInterruptType eTypeMips;
    /* 0x0C */ SystemInterruptType eType;
} SystemException; // size = 0x14

// __anon_0x37040
typedef struct SystemRomCopy {
    /* 0x00 */ s32 nSize;
    /* 0x04 */ s32 nOffsetRAM;
    /* 0x08 */ s32 nOffsetROM;
    /* 0x0C */ SystemCopyCallbackFunc pCallback;
} SystemRomCopy; // size = 0x10

// __anon_0x37240
typedef struct System {
    // ok
    /* 0x00 */ bool bException;
    /* 0x04 */ SystemMode eMode;
    /* 0x08 */ SystemObjectType storageDevice;
    /* 0x0C */ SystemRomType eTypeROM;
    /* 0x10 */ void* apObject[SOT_COUNT];

    // not ok
    /* 0x6C */ s32 controllerChannel;
    /* 0x70 */ u64 nAddressBreak;

    /* 0x78 */ s32 UNKNOWN_78[19];
    /* 0xC4 */ void* pSound;
    /* 0xC8 ok */ u8 anException[16];

    // removed?
    /* 0xD8 */ SystemRomCopy romCopy;
    /* 0xE8 */ bool bJapaneseVersion;
} System; // size = 0xEC

// __anon_0x3459E
typedef struct SystemRomConfig {
    /* 0x0000 */ char rom[36];
    /* 0x0024 */ s32 controllerConfiguration[4][GCN_BTN_COUNT];
    /* 0x0164 */ s32 rumbleConfiguration;
    /* 0x0168 */ SystemObjectType storageDevice;
    /* 0x016C */ s32 normalControllerConfig;
    /* 0x0170 */ s32 currentControllerConfig;
} SystemRomConfig; // size = 0x174

#define SYSTEM_CPU(pSystem) ((Cpu*)(((System*)(pSystem))->apObject[SOT_CPU]))
#define SYSTEM_PIF(pSystem) ((Pif*)(((System*)(pSystem))->apObject[SOT_PIF]))
#define SYSTEM_RAM(pSystem) ((Ram*)(((System*)(pSystem))->apObject[SOT_RAM]))
#define SYSTEM_ROM(pSystem) ((Rom*)(((System*)(pSystem))->apObject[SOT_ROM]))
#define SYSTEM_RSP(pSystem) ((Rsp*)(((System*)(pSystem))->apObject[SOT_RSP]))
#define SYSTEM_RDP(pSystem) ((Rdp*)(((System*)(pSystem))->apObject[SOT_RDP]))
#define SYSTEM_MIPS(pSystem) ((Mips*)(((System*)(pSystem))->apObject[SOT_MIPS]))
#define SYSTEM_DISK(pSystem) ((Disk*)(((System*)(pSystem))->apObject[SOT_DISK]))

//! TODO: replace void* by the struct names
#define SYSTEM_AI(pSystem) ((void*)(((System*)(pSystem))->apObject[SOT_AI]))
#define SYSTEM_VI(pSystem) ((void*)(((System*)(pSystem))->apObject[SOT_VI]))
#define SYSTEM_SI(pSystem) ((void*)(((System*)(pSystem))->apObject[SOT_SI]))

#define SYSTEM_PERIPHERAL(pSystem) ((Peripheral*)(((System*)(pSystem))->apObject[SOT_PERIPHERAL]))
#define SYSTEM_RDB(pSystem) ((Rdb*)(((System*)(pSystem))->apObject[SOT_RDB]))

//! TODO: replace void* by the struct name
#define SYSTEM_EEPROM(pSystem) ((void*)(((System*)(pSystem))->apObject[SOT_EEPROM]))

#define SYSTEM_SRAM(pSystem) ((Sram*)(((System*)(pSystem))->apObject[SOT_SRAM]))
#define SYSTEM_FLASH(pSystem) ((Flash*)(((System*)(pSystem))->apObject[SOT_FLASH]))

//! TODO: replace void* by the struct name
#define SYSTEM_CODE(pSystem) ((void*)(((System*)(pSystem))->apObject[SOT_CODE]))

//! TODO: replace void* by the struct name
#define SYSTEM_HELP(pSystem) ((void*)(((System*)(pSystem))->apObject[SOT_HELP]))

#define SYSTEM_LIBRARY(pSystem) ((Library*)(((System*)(pSystem))->apObject[SOT_LIBRARY]))
#define SYSTEM_FRAME(pSystem) ((Frame*)(((System*)(pSystem))->apObject[SOT_FRAME]))
#define SYSTEM_AUDIO(pSystem) ((Audio*)(((System*)(pSystem))->apObject[SOT_AUDIO]))
#define SYSTEM_VIDEO(pSystem) ((Video*)(((System*)(pSystem))->apObject[SOT_VIDEO]))

#define SYSTEM_SOUND(pSystem) ((Sound*)(((System*)(pSystem))->pSound))

extern u32 nTickMultiplier;
extern f32 fTickScale;
extern u32 gnFlagZelda;

bool systemCopyROM(System* pSystem, s32 nOffsetRAM, s32 nOffsetROM, s32 nSize, SystemCopyCallbackFunc pCallback);
bool systemSetMode(System* pSystem, SystemMode eMode);
bool systemGetMode(System* pSystem, SystemMode* peMode);
bool systemSetStorageDevice(System* pSystem, SystemObjectType eStorageDevice);
bool systemGetStorageDevice(System* pSystem, SystemObjectType* pStorageDevice);
bool systemReset(System* pSystem);
bool systemExecute(System* pSystem, s32 nCount);
bool systemCheckInterrupts(System* pSystem);
bool systemExceptionPending(System* pSystem, SystemInterruptType nException);
bool systemEvent(System* pSystem, s32 nEvent, void* pArgument);

extern _XL_OBJECTTYPE gClassSystem;
extern System* gSystem;

#endif
