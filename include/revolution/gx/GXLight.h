#ifndef REVOSDK_GX_LIGHT_H
#define REVOSDK_GX_LIGHT_H
#include "GX.h"
#include "dolphin/types.h"

typedef struct _GXLightObj {
    char UNK_0x0[64];
} GXLightObj;

typedef enum _GXLightID {

} GXLightID;

typedef enum _GXSpotFn {
    GX_SPOT_FN_0
} GXSpotFn;

typedef enum _GXDistAttnFn {
    GX_DIST_ATTN_FN_0
} GXDistAttnFn;

void GXInitLightColor(GXLightObj*, GXColor*);
void GXInitLightPos(GXLightObj*, float, float, float);
void GXInitLightDir(GXLightObj*, float, float, float);
void GXInitSpecularDir(GXLightObj*, float, float, float);
void GXInitLightSpot(GXLightObj*, float, GXSpotFn);
void GXInitLightAttnA(GXLightObj*, float, float, float);
void GXInitLightDistAttn(GXLightObj*, float, float, GXDistAttnFn);
void GXInitLightAttnK(GXLightObj*, float, float, float);
void GXInitLightAttn(GXLightObj*, float, float, float, float, float, float);
void GXGetLightPos(const GXLightObj*, float*, float*, float*);
void GXGetLightDir(const GXLightObj*, float*, float*, float*);

void GXSetChanAmbColor(GXChannelID, GXColor);
void GXSetChanMatColor(GXChannelID, GXColor);
void GXSetNumChans(s32 nChans);
void GXSetChanCtrl(GXChannelID, s32, s32, s32, s32, s32, s32);

#endif