#ifndef _EEPROM_H
#define _EEPROM_H

#include "emulator/xlObject.h"
#include "revolution/types.h"

typedef struct EEPROM {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ void* unk_04;
    /* 0x08 */ void* pRAM;
} EEPROM; // size = 0x0C

// bool fn_80044708(EEPROM* pEEPROM, s32 arg2, s32 arg3);
bool eepromEvent(EEPROM* pEEPROM, s32 nEvent, void* pArgument);

extern _XL_OBJECTTYPE gClassEEPROM;

#endif
