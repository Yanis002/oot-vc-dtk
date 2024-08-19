#include "emulator/xlObject.h"
#include "emulator/video.h"

_XL_OBJECTTYPE gClassVideo = {
    "Video",
    sizeof(Video),
    NULL,
    (EventFunc)videoEvent
};

bool videoEvent(Video* pVideo, s32 nEvent, void* pArgument) {
    switch (nEvent) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 5:
        case 6:
            break;
        default:
            return false;
    }

    return true;
};
