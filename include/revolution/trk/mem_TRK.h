#ifndef REVOSDK_TRK_MEM_TRK_H
#define REVOSDK_TRK_MEM_TRK_H

#include "dolphin/types.h"
#include "macros.h"
#include "stddef.h"

INIT void * TRK_memset(void *, int, size_t);
INIT void * TRK_memcpy(void *, const void *, size_t);

void TRK_fill_mem(void *, int, size_t);

#endif