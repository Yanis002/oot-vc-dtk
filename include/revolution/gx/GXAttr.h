#ifndef REVOSDK_GX_ATTR_H
#define REVOSDK_GX_ATTR_H
#include "GXTexture.h"
#include "dolphin/types.h"

typedef enum _GXAttr {
    GX_ATTR_0,
    GX_ATTR_1,
    GX_ATTR_2,
    GX_ATTR_3,
    GX_ATTR_4,
    GX_ATTR_5,
    GX_ATTR_6,
    GX_ATTR_7,
    GX_ATTR_8,

    GX_ATTR_VTX,

    GX_ATTR_VTX_POS = GX_ATTR_VTX,
    GX_ATTR_VTX_NRM,

    GX_ATTR_VTX_CLR,
    GX_ATTR_VTX_CLR_COUNT = 2,

    GX_ATTR_VTX_TEX_COORD = GX_ATTR_VTX_CLR + GX_ATTR_VTX_CLR_COUNT,
    GX_ATTR_VTX_TEX_COORD_COUNT = 8,

    GX_ATTR_14 = 14,
    GX_ATTR_15,
    GX_ATTR_16,
    GX_ATTR_17,
    GX_ATTR_18,
    GX_ATTR_19,
    GX_ATTR_20,

    GX_ATTR_COUNT = GX_ATTR_VTX_TEX_COORD + GX_ATTR_VTX_TEX_COORD_COUNT,
    GX_ATTR_VTX_COUNT = GX_ATTR_COUNT - GX_ATTR_VTX,

    GX_ATTR_INVALID = 0xFF
} GXAttr;

typedef struct _GXVtxDescList {
    GXAttr mAttr; // at 0x0
    s32 WORD_0x4;
} GXVtxDescList[];

typedef struct _GXVtxAttrFmtList {
    GXAttr mAttr; // at 0x0
    s32 WORD_0x4;
    s32 WORD_0x8;
    char BYTE_0xC;
} GXVtxAttrFmtList[];

void GXSetVtxDesc(GXAttr, s32);

void GXClearVtxDesc(void);
void GXSetVtxAttrFmt(s32 formatIndex, GXAttr, s32, s32, s32 fracValue);

void GXSetArray(GXAttr, const void*, u8);
void GXInvalidateVtxCache(void);
void GXSetTexCoordGen2(GXTexCoordID, GXTexGenType function, GXTexGenSrc, s32, s32, s32);
void GXSetNumTexGens(s32);

#endif