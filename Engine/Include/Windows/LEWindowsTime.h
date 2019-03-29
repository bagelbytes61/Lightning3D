#ifndef LE_WINDOWS_TIME_H_
#define LE_WINDOWS_TIME_H_

#pragma once

#include "LEPlatformTime.h"

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

namespace Lightning3D::Engine {
    class WindowsTime : public PlatformTime {
    public:
        static void Initialize();

        static uint64_t GetElapsedSeconds();

        static uint64_t GetElapsedMillis();

        static uint64_t GetElapsedMicros();

        static uint64_t GetElapsedCycles();
    };
}

#endif