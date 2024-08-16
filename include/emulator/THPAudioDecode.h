#ifndef THP_AUDIO_DECODE_H
#define THP_AUDIO_DECODE_H

#include "revolution.h"

bool CreateAudioDecodeThread(OSPriority prio, void* param);
void AudioDecodeThreadStart(void);
void AudioDecodeThreadCancel(void);
void* PopFreeAudioBuffer(void);
void PushFreeAudioBuffer(void* buf);
void* PopDecodedAudioBuffer(s32 flags);
void PushDecodedAudioBuffer(void* buf);

#endif
