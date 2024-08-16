#ifndef REVOSDK_GX_TEXTURE_H
#define REVOSDK_GX_TEXTURE_H
#include "GX.h"
#include "dolphin/types.h"

typedef enum _GXTexMapID {
    GX_TEX_MAP_ID_0,
    GX_TEX_MAP_ID_1,
    GX_TEX_MAP_ID_2,
    GX_TEX_MAP_ID_3,
    GX_TEX_MAP_ID_4,
    GX_TEX_MAP_ID_5,
    GX_TEX_MAP_ID_6,
    GX_TEX_MAP_ID_7,

    GX_TEX_MAP_ID_INVALID = 0xFF
} GXTexMapID;

typedef enum _GXTexFmt {
    GX_TEX_FMT_0,
    GX_TEX_FMT_1,
    GX_TEX_FMT_2,
    GX_TEX_FMT_3,
    GX_TEX_FMT_4,
    GX_TEX_FMT_5,
    GX_TEX_FMT_6,
    GX_TEX_FMT_7,

    // CI formats
    GX_TEX_FMT_8,
    GX_TEX_FMT_9,
    GX_TEX_FMT_10,

    GX_TEX_FMT_11,
    GX_TEX_FMT_12,
    GX_TEX_FMT_13,
    GX_TEX_FMT_14,
    GX_TEX_FMT_15,
    GX_TEX_FMT_16,
    GX_TEX_FMT_17,
    GX_TEX_FMT_18,
    GX_TEX_FMT_19,
    GX_TEX_FMT_20,
    GX_TEX_FMT_21,
    GX_TEX_FMT_22
} GXTexFmt;

typedef enum _GXTlutFmt {
    GX_TLUT_FORMAT_0
} GXTlutFmt;

typedef enum _GXTexFilter {
    GX_LINEAR = 1,
    GX_LIN_MIP_LIN = 5
} GXTexFilter;

typedef enum _GXAnisotropy {

} GXAnisotropy;

typedef enum _GXTexWrapMode {
    GX_TEXWRAPMODE_0,
    GX_TEXWRAPMODE_1,
    GX_TEXWRAPMODE_2,
    GX_MAX_TEXWRAPMODE
} GXTexWrapMode;

typedef enum _GXCITexFmt {

} GXCITexFmt;

typedef enum _GXTexCoordID {
    GX_TEX_COORD_ID_0,
    GX_TEX_COORD_ID_COUNT = 8,
    GX_TEX_COORD_ID_INVALID = 0xFF
} GXTexCoordID;

typedef enum _GXTexGenType {
    GX_TEX_GEN_TYPE_0,
    GX_TEX_GEN_TYPE_1,
} GXTexGenType;

typedef enum _GXTexGenSrc {
    GX_TEX_GEN_SRC_0,
    GX_TEX_GEN_SRC_1,
    GX_TEX_GEN_SRC_2,
    GX_TEX_GEN_SRC_3,
    GX_TEX_GEN_SRC_4,
} GXTexGenSrc;

typedef struct _GXTlutObj {

} GXTlutObj;

typedef struct _GXTexObj {
    s32 mFlags; // at 0x0
    char UNK_0x4[0x4];
    s32 mDimensions; // at 0x8 (Height/width are split between these bits)
    char UNK_0xC[0x4];
    s32 mUserData; // at 0x10
    GXTexFmt mFormat; // at 0x14
    s32 mTLUT; // at 0x18
} GXTexObj;

s32 GXGetTexBufferSize(s32 width, s32 height, GXTexFmt format, s32, s32);

void GXInitTexObj(GXTexObj*, void* image, u16 width, s32 height, s32 texFormat, s32 wrapModeS, s32 wrapModeT,
                  bool mipmap);
void GXInitTexObjCI(GXTexObj*, void*, s32 width, s32 height, s32 texFormat, s32 wrapModeS, s32 wrapModeT, bool mipmap,
                    s32 tlut);
void GXInitTexObjLOD(GXTexObj*, int min_filt, int mag_filt, s32 biasClampEnable, s32 edgeLodEnable, s32 anisotropy,
                     f32 minLod, f32 maxLod, f32 lodBias);
void GXLoadTexObj(GXTexObj*, GXTexMapID texMapID);
s32 GXGetTexObjTlut(GXTexObj*);
void GXInitTlutObj(GXTlutObj*, void* palette, s32 paletteFmt, s32 paletteEntryNum);

void GXInvalidateTexAll();

void GXSetTexCoordScaleManually(s32, s32, s32, s32);
void GXSetTexCoordCylWrap(s32, s32, s32);

#endif