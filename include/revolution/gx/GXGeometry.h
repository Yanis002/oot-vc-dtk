#ifndef REVOSDK_GX_GEOMETRY_H
#define REVOSDK_GX_GEOMETRY_H
#include "GX.h"
#include "dolphin/types.h"

typedef enum _GXCullMode {

} GXCullMode;

void GXBegin(s32, s32 formatIndex, s32);
static void GXEnd(void) {}

void GXSetCullMode(s32);

void GXSetCoPlanar(s32);

#endif