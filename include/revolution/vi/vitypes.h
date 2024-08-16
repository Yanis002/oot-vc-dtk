#ifndef RVL_SDK_VITYPES_H
#define RVL_SDK_VITYPES_H

#include "revolution/types.h"

// Merge format/mode to one value for TV info (see GXRenderModeObj)
#define VI_TV_INFO(format, mode) (((format) << 2) + (mode))
// Get TV format from TV info
#define VI_TV_INFO_FMT(info) ((info) >> 2)
// Get TV scan mode from TV info
#define VI_TV_INFO_MODE(info) ((info) & 0b00000011)

#define VI_INTERLACE 0
#define VI_NON_INTERLACE 1
#define VI_PROGRESSIVE 2
#define VI_3D 3

#define VI_NTSC 0
#define VI_PAL 1
#define VI_MPAL 2
#define VI_DEBUG 3
#define VI_DEBUG_PAL 4
#define VI_EURGB60 5
#define VI_GCA 6

typedef enum {
    VI_TV_FMT_NTSC,
    VI_TV_FMT_PAL,
    VI_TV_FMT_MPAL,
    VI_TV_FMT_DEBUG,
    VI_TV_FMT_DEBUG_PAL,
    VI_TV_FMT_EURGB60,
    VI_TV_FMT_6,
    VI_TV_FMT_7,
    VI_TV_FMT_8,
} VITvFormat;

typedef enum VIScanMode {
    VI_SCAN_MODE_INT,
    VI_SCAN_MODE_DS,
    VI_SCAN_MODE_PROG
} VIScanMode;

typedef enum VIXfbMode {
    VI_XFB_MODE_SF,
    VI_XFB_MODE_DF,
} VIXfbMode;

#endif
