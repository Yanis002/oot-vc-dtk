#ifndef REVOSDK_TRK___MEM_H
#define REVOSDK_TRK___MEM_H

#include "dolphin/types.h"
#include "macros.h"
#include "stddef.h"

INIT void* memcpy(void*, const void*, size_t);
INIT void __fill_mem(void*, int, size_t);
INIT void* memset(void*, int, size_t);

#endif