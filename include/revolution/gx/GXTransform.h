#ifndef REVOSDK_GX_TRANSFORM_H
#define REVOSDK_GX_TRANSFORM_H
#include "GX.h"
#include "dolphin/types.h"

typedef enum _GXProjectionType {
    GX_PROJECTION_PERSP,
    GX_PROJECTION_ORTHO
} GXProjectionType;

void GXSetProjection(float (*)[4], u32);
void GXSetProjectionv(const f32*);
void GXGetProjectionv(f32*);

void GXLoadPosMtxImm(float (*)[4], u32);

void GXLoadTexMtxImm(float (*)[4], s32, s32 type);

void GXSetCurrentMtx(s32);

void GXSetViewportJitter(f32, f32, f32, f32, f32, f32, u32);
void GXSetViewport(f32, f32, f32, f32, f32, f32);
void GXGetViewportv(f32[6]);

void GXSetScissor(u32 left, u32 top, u32 width, u32 height);
void GXGetScissor(u32*, u32*, u32*, u32*);
void GXSetScissorBoxOffset(u32 xOffset, u32 yOffset);

void GXSetClipMode(s32);

#endif