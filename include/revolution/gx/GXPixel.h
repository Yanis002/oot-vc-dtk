#ifndef REVOSDK_GX_PIXEL_H
#define REVOSDK_GX_PIXEL_H
#include "GX.h"
#include "dolphin/types.h"

typedef enum _GXFogType {
    GX_FOG_TYPE_0,
    GX_FOG_TYPE_1,
    GX_FOG_TYPE_2,
} GXFogType;

// Unofficial name
typedef enum _GXPixelFmt {
    GX_PIXEL_FMT_0,
    GX_PF_RGBA6_Z24,
    GX_PIXEL_FMT_2,
    GX_PIXEL_FMT_3,
    GX_PIXEL_FMT_4,
    GX_PIXEL_FMT_5,
    GX_PIXEL_FMT_6,
    GX_PIXEL_FMT_7,
} GXPixelFmt;

void GXSetFog(GXFogType, GXColor, float startz, float endz, float nearz, float farz);

void GXInitFogAdjTable(u16* table, u16 width, const float (*)[4]);
void GXSetFogRangeAdj(u8, u16, u16* table);

void GXSetBlendMode(s32, s32, s32, s32);

void GXSetColorUpdate(u8);
void GXSetAlphaUpdate(u8);
void GXSetZMode(s32, s32, s32);
void GXSetZCompLoc(s32);

void GXSetPixelFmt(GXPixelFmt, s32);
void GXSetDither(u8);
void GXSetDstAlpha(s32, s32);

#endif