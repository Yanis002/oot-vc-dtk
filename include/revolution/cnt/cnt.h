#ifndef REVOSDK_CNT_H
#define REVOSDK_CNT_H

#include "dolphin/types.h"

typedef struct CNTFileInfo {
    /* 0x000 */ struct ARCHandle* parent;
    /* 0x004 */ s32 fileOffset;
    /* 0x008 */ s32 fileSize;
    /* 0x00C */ s32 unkC;
} CNTFileInfo; // size = 0x10

s32 contentFastOpenNAND(struct ARCHandle*, s32, CNTFileInfo*);
s32 contentConvertPathToEntrynumNAND(struct ARCHandle*, const char*);
s32 contentGetLengthNAND(const CNTFileInfo*);
s32 contentReadNAND(CNTFileInfo*, void*, s32, s32);
s32 contentCloseNAND(CNTFileInfo*);

#endif