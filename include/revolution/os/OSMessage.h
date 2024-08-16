#ifndef REVOSDK_OS_MESSAGE
#define REVOSDK_OS_MESSAGE

#include "dolphin/types.h"

typedef void* OSMessage;

struct OSMessageQueue {
    s32 WORD_0x0;
    s32 WORD_0x4;
    s32 WORD_0x8;
    s32 WORD_0xC;
    s32 WORD_0x10;
    s32 WORD_0x14;
    s32 WORD_0x18;
    s32 WORD_0x1C;
};

s32 OSInitMessageQueue(struct OSMessageQueue*, OSMessage* buffer, s32 mesgCount);

bool OSSendMessage(struct OSMessageQueue*, OSMessage, s32);
bool OSReceiveMessage(struct OSMessageQueue*, OSMessage*, s32);
bool OSJamMessage(struct OSMessageQueue*, s32, s32);

#endif