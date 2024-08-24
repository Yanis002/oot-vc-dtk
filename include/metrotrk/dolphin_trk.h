#ifndef METROTRK_DEBUGGER_OS_DOLPHIN_DOLPHIN_TRK_H
#define METROTRK_DEBUGGER_OS_DOLPHIN_DOLPHIN_TRK_H

#include "revolution/types.h"

#ifdef __cplusplus
extern "C" {
#endif

void InitMetroTRK(void);
void InitMetroTRK_BBA(void);

void EnableMetroTRKInterrupts(void);

#ifdef __cplusplus
}
#endif

#endif
