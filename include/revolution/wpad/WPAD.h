#ifndef REVOSDK_WPAD_H
#define REVOSDK_WPAD_H

#include "dolphin/types.h"

typedef void (*WPADCallback)(s32, s32);

void WPADControlSpeaker(s32, s32, WPADCallback);
bool WPADCanSendStreamData(s32);
s32 WPADSendStreamData(s32, void *, s32);


#endif