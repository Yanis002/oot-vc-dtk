#ifndef REVOSDK_GX_TEV_H
#define REVOSDK_GX_TEV_H
#include "GX.h"
#include "dolphin/types.h"

typedef enum _GXTevSwapSel {
    GX_TEV_SWAP_SEL_0,
    GX_TEV_SWAP_SEL_1,
    GX_TEV_SWAP_SEL_2,
    GX_TEV_SWAP_SEL_3,
    GX_TEV_SWAP_SEL_COUNT
} GXTevSwapSel;

typedef enum _GXTevColorChan {

} GXTevColorChan;

typedef enum _GXTevStageID {
    GX_TEV_STAGE_ID_0,
    GX_TEV_STAGE_ID_1,
    GX_TEV_STAGE_ID_2,
    GX_TEV_STAGE_ID_COUNT = 16
} GXTevStageID;

typedef enum _GXTevColorArg {

} GXTevColorArg;

typedef enum _GXTevRegID {
    GX_TEV_REG_ID_0,
    GX_TEV_REG_ID_1,
    GX_TEV_REG_ID_2,
    GX_TEV_REG_ID_3,
} GXTevRegID;

void GXSetTevOp(GXTevStageID, s32 tevMode);
void GXSetTevColorIn(GXTevStageID, s32 a, s32 b, s32 c, s32 d);
void GXSetTevAlphaIn(GXTevStageID, s32 a, s32 b, s32 c, s32 d);
void GXSetTevColorOp(GXTevStageID, s32, s32, s32, s32, s32);
void GXSetTevAlphaOp(GXTevStageID, s32, s32, s32, s32, s32);
void GXSetTevColor(GXTevRegID, GXColor);

void GXSetTevKColor(GXTevRegID, GXColor);
void GXSetTevKColorSel(GXTevStageID, s32);
void GXSetTevKAlphaSel(GXTevStageID, s32);

void GXSetTevSwapModeTable(GXTevSwapSel, s32, s32, s32, s32);

void GXSetAlphaCompare(s32, s32, s32, s32, s32);
void GXSetZTexture(s32, s32 zTexFormat, s32);

void GXSetNumTevStages(u8);

#endif