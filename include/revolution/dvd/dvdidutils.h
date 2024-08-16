#ifndef RVL_SDK_DVD_ID_UTILS_H
#define RVL_SDK_DVD_ID_UTILS_H

#include "revolution/dvd/dvd.h"
#include "revolution/types.h"

#ifdef __cplusplus
extern "C" {
#endif

bool DVDCompareDiskID(const DVDDiskID* id1, const DVDDiskID* id2);

#ifdef __cplusplus
}
#endif

#endif
