#ifndef REVOSDK_TPL_H
#define REVOSDK_TPL_H

#include "dolphin/types.h"
#include "revolution/gx/GXTexture.h"

typedef struct
{
    u32 mVersion; // 00 20 AF 30
    s32 WORD_0x4;
    s32 WORD_0x8;
} TPLPalette;

typedef struct
{
    u16 mHeight; // at 0x0
    u16 mWidth; // at 0x2
    GXTexFmt mTexFmt; // at 0x4
    void *mImage; // at 0x8
    GXTexWrapMode mWrapModeS; // at 0xC
    GXTexWrapMode mWrapModeT; // at 0x10
    GXTexFilter mMinFilter; // at 0x14
    GXTexFilter mMagFilter; // at 0x18
    f32 mLODBias; // at 0x1C
    u8 mEdgeLODEnable; // at 0x20
    u8 mMinLOD; // at 0x21
    u8 mMaxLOD; // at 0x22
} TPLDescriptorUnk0;

typedef struct
{
    u16 mPalEntryNum; // at 0x0
    char UNK_0x2[0x2];
    GXTlutFmt mPaletteFmt; // at 0x4
    void *mPalette; // at 0x8
} TPLDescriptorUnk1;

typedef struct
{
    TPLDescriptorUnk0 *PTR_0x0;
    TPLDescriptorUnk1 *PTR_0x4;
} TPLDescriptor;

void TPLBind(TPLPalette *);
TPLDescriptor * TPLGet(TPLPalette *, u32 id);

#endif