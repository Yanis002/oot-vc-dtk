#ifndef METROTRK_DEBUGGER_OS_DOLPHIN_DOLPHIN_TRK_GLUE_H
#define METROTRK_DEBUGGER_OS_DOLPHIN_DOLPHIN_TRK_GLUE_H

#include "metrotrk/UART.h"
#include "revolution/types.h"
#include "stddef.h"

#ifdef __cplusplus
extern "C" {
#endif

UARTError TRKWriteUARTN(const void* src, size_t n);

#ifdef __cplusplus
}
#endif

#endif
