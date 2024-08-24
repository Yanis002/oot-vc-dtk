#ifndef RVL_SDK_OS_EXEC_H
#define RVL_SDK_OS_EXEC_H

#include "revolution/types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OSExecParams {
    int valid; // at 0x0
    u32 restartCode; // at 0x4
    u32 bootDol; // at 0x8
    void* regionStart; // at 0xC
    void* regionEnd; // at 0x10
    int argsUseDefault; // at 0x14
    void* argsAddr; // at 0x18
} OSExecParams;

extern bool __OSInReboot;

void __OSGetExecParams(OSExecParams* out);
void __OSLaunchMenu(void);

#ifdef __cplusplus
}
#endif

#endif
