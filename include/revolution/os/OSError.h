#ifndef REVOSDK_OS_ERROR_H
#define REVOSDK_OS_ERROR_H

#include "dolphin/types.h"

void OSReport(const char*, ...);
void OSPanic(const char*, s32, const char*, ...);
#define OSError(...) OSPanic(__FILE__, __LINE__, __VA_ARGS__)

#endif