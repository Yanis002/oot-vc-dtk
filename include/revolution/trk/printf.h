#ifndef REVOSDK_TRK_PRINTF_H
#define REVOSDK_TRK_PRINTF_H

#include "stddef.h"
#include "stdarg.h"

int printf(const char *, ...);
int vprintf(const char *, va_list);
int snprintf(char *, size_t, const char *, ...);
int vsprintf(char *, const char *, va_list);
int vsnprintf(char *, size_t, const char *, va_list);

#endif