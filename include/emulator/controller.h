#ifndef _CONTROLLER_H
#define _CONTROLLER_H

#include "emulator/xlObject.h"
#include "emulator/system.h"
#include "revolution/types.h"

typedef struct lbl_801C7DB8_Struct {
    /* 0x00 */ void* unk_00;
    /* 0x04 */ u32 unk_04;
    /* 0x08 */ u32 unk_08;
    /* 0x0C */ u32 unk_0C;
} lbl_801C7DB8_Struct; // size = 0x10

typedef struct ControllerThread {
    /* 0x000 */ OSThread thread;
    /* 0x318 */ u8 unk_318[0xA];
} ControllerThread; // size = 0x328

typedef struct Controller {
    /* 0x000 */ u32 unk_00[19];
    /* 0x04C */ u32 unk_4C[4];
    /* 0x05C */ u8 unk_5C[0x20];
    /* 0x07C */ s32 unk_7C;
    /* 0x080 */ s32 unk_80;
    /* 0x084 */ s32 unk_84;
    /* 0x088 */ s32 unk_88;
    /* 0x08C */ s32 unk_8C;
    /* 0x090 */ s32 unk_90;
    /* 0x094 */ s32 unk_94;
    /* 0x098 */ s32 unk_98;
    /* 0x09C */ s32 unk_9C;
    /* 0x0A0 */ s32 unk_A0;
    /* 0x0A4 */ s32 unk_A4;
    /* 0x0A8 */ s32 unk_A8;
    /* 0x0AC */ s32 unk_AC;
    /* 0x0B0 */ s32 unk_B0;
    /* 0x0B4 */ s32 unk_B4;
    /* 0x0B8 */ s32 unk_B8;
    /* 0x0BC */ s32 unk_BC;
    /* 0x0C0 */ s32 unk_C0;
    /* 0x0C4 */ s32 unk_C4;
    /* 0x0C8 */ s32 unk_C8;
    /* 0x0CC */ s32 unk_CC;
    /* 0x0D0 */ s32 unk_D0;
    /* 0x0D4 */ s32 unk_D4;
    /* 0x0D8 */ s32 unk_D8;
    /* 0x0DC */ u32 controllerConfiguration[4][GCN_BTN_COUNT];
    /* 0x21C */ s32 unk_21C;
    /* 0x220 */ s32 unk_220;
    /* 0x224 */ s32 unk_224;
    /* 0x228 */ u32 unk_228;
    /* 0x22C */ u32 unk_22C;
    /* 0x230 */ u32 unk_230;
    /* 0x234 */ u32 unk_234;
    /* 0x238 */ u32 unk_238;
    /* 0x23C */ u32 unk_23C;
    /* 0x240 */ u32 unk_240;
    /* 0x244 */ u32 unk_244;
    /* 0x248 */ s32 unk_248;
    /* 0x24C */ s32 unk_24C;
    /* 0x250 */ u8 unk_250[0x20];
    /* 0x270 */ u32 unk_270;
    /* 0x274 */ u32 unk_274;
    /* 0x278 */ u32 unk_278;
    /* 0x27C */ u32 unk_27C;
    /* 0x280 */ u32 unk_280;
    /* 0x284 */ u32 unk_284;
    /* 0x288 */ u32 unk_288;
    /* 0x28C */ u32 unk_28C;
} Controller; // size = 0x290

bool simulatorSetControllerMap(Controller* pController, s32 channel, u32* mapData);
bool simulatorCopyControllerMap(Controller* pController, u32* mapDataOutput, u32* mapDataInput);
bool simulatorDetectController(Controller* pController, s32 arg1);
bool fn_80062C18(Controller* pController, s32 arg1, s32* arg2, s32* arg3, s32* arg4, s32* arg5, s32* arg6, s32* arg7);
bool fn_80062CE4(Controller* pController, s32 arg1);
bool fn_800631B8(Controller* pController, s32 arg1);
bool controllerEvent(Controller* pController, s32 nEvent, void* pArgument);

extern _XL_OBJECTTYPE gClassController;

#endif
