#ifndef REVOSDK_SC_SCAPI_H
#define REVOSDK_SC_SCAPI_H

#include "dolphin/types.h"

typedef enum SCAspectRatio
{
    SC_ASPECT_4_3,
    SC_ASPECT_16_9
} SCAspectRatio;

u8 SCGetAspectRatio(void);

#endif