#ifndef REVOSDK_OS_ALARM_H
#define REVOSDK_OS_ALARM_H

#include "dolphin/types.h"

struct OSAlarm {
    char UNK_0x0[0x30];
} __attribute__((aligned(4)));

typedef void (*OSAlarmHandler)(struct OSAlarm*, struct OSContext*);

void OSCreateAlarm(struct OSAlarm*);

void OSSetAlarmUserData(struct OSAlarm*, void*);
void* OSGetAlarmUserData(struct OSAlarm*);

void OSCancelAlarm(struct OSAlarm*);

void OSSetAlarm(struct OSAlarm*, s64, OSAlarmHandler);
void OSSetPeriodicAlarm(struct OSAlarm*, s64, s64, OSAlarmHandler);

#endif