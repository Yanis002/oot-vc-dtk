#ifndef REVOSDK_OS_CONTEXT
#define REVOSDK_OS_CONTEXT

#include "revolution/types.h"

typedef enum OSContextState {
    OS_CONTEXT_STATE_FP_SAVED = (1 << 0),
} OSContextState;

typedef struct OSContext {
    u32 gprs[32]; // at 0x0
    u32 cr; // at 0x80
    u32 lr; // at 0x84
    u32 ctr; // at 0x88
    u32 xer; // at 0x8C
    f64 fprs[32]; // at 0x90
    u32 fpscr_pad; // at 0x190
    u32 fpscr; // at 0x194
    u32 srr0; // at 0x198
    u32 srr1; // at 0x19C
    u16 mode; // at 0x1A0
    u16 state; // at 0x1A2
    u32 gqrs[8]; // at 0x1A4
    u32 psf_pad; // at 0x1C4
    f64 psfs[32]; // at 0x1C8
} OSContext;

#endif
