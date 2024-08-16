#ifndef _DOLPHIN_OSALARM
#define _DOLPHIN_OSALARM

#include "dolphin/os/OSContext.h"
#include "dolphin/types.h"

typedef struct OSAlarm OSAlarm;
typedef void (*OSAlarmHandler)(OSAlarm* alarm, OSContext* context);

struct OSAlarm {
    /* 0x00 */ OSAlarmHandler handler;
    /* 0x04 */ u32 tag;
    /* 0x08 */ OSTime fire;
    /* 0x10 */ OSAlarm* prev;
    /* 0x14 */ OSAlarm* next;
    /* 0x18 */ OSTime period;
    /* 0x20 */ OSTime start;
    void* userData;
}; // size = 0x2C

void OSInitAlarm(void);
void OSCreateAlarm(OSAlarm* alarm);
void OSSetAlarm(OSAlarm* alarm, OSTime tick, OSAlarmHandler handler);
void OSCancelAlarm(OSAlarm* alarm);

bool OSCheckAlarmQueue(void);

#endif // _DOLPHIN_OSALARM
