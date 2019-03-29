#ifndef LE_PLATFORM_TIME_H_
#define LE_PLATFORM_TIME_H_

#pragma once

#include <cstdint>
#include <memory>

namespace Lightning3D::Engine {
    class PlatformTime {
    public:
        static void Initialize();

        static uint64_t GetElapsedSeconds();
               
        static uint64_t GetElapsedMillis();
               
        static uint64_t GetElapsedMicros();

        static uint64_t GetElapsedCycles();

    protected:
       inline static uint64_t s_resolution;
    };
}

#endif