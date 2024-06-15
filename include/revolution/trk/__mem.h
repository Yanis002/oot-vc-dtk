#ifndef REVOSDK_TRK___MEM_H
#define REVOSDK_TRK___MEM_H

#include "dolphin/types.h"
#include "stddef.h"
#include "macros.h"

INIT void* memcpy(void*, const void*, size_t);
INIT void __fill_mem(void*, int, size_t);
INIT void* memset(void*, int, size_t);

#endif