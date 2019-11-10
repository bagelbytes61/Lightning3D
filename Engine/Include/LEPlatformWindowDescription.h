#ifndef LE_PLATFORM_WINDOW_DESCRIPTION_H_
#define LE_PLATFORM_WINDOW_DESCRIPTION_H_

#pragma once

#include <cstdint>

namespace Lightning3D {
    struct PlatformWindowDescription {
        uint32_t windowedWidth;
        uint32_t windowedHeight;

        uint32_t fullscreenWidth;
        uint32_t fullscreenHeight;

        bool windowed;

        const char* title;
    };
}

#endif