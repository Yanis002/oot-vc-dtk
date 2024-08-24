#include "revolution/os.h"

s64 fn_800945CC(void) {
    return __OSGetSystemTime();
}

s32 fn_800945D0(void) {
    return OS_TICKS_TO_SEC(OSGetTime()) + 0xBC17C200;
}

bool fn_80094610(void) {
    return false;
}
