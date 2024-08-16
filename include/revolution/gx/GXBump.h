#ifndef REVOSDK_GX_BUMP_H
#define REVOSDK_GX_BUMP_H
#include "GXTev.h"
#include "GXTexture.h"
#include "dolphin/types.h"
#include "math/mtx.h"

typedef enum _GXIndTexStageID {
    GX_IND_TEX_STAGE_0
} GXIndTexStageID;

typedef enum _GXIndTexScale {
    GX_IND_TEX_SCALE_0
} GXIndTexScale;

typedef enum _GXIndTexMtxID {
    GX_IND_TEX_MTX_0
} GXIndTexMtxID;

void GXSetIndTexCoordScale(GXIndTexStageID, GXIndTexScale, GXIndTexScale);
void GXSetIndTexMtx(GXIndTexMtxID, const f32*, s32);

void GXSetNumIndStages(s32);
void GXSetTevDirect(s32);

void __GXSetIndirectMask(s32);

void GXSetTevSwapMode(GXTevStageID, s32, s32);

void GXSetTevOrder(GXTevStageID, GXTexCoordID, GXTexMapID, s32 colorChanID);

#endif