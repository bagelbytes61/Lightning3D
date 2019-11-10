#ifndef LE_PLATFORM_FILE_SYSTEM_H_
#define LE_PLATFORM_FILE_SYSTEM_H_

#pragma once

#include <memory>

namespace Lightning3D {
    class PlatformFileSystem {
    public:
        static void CreateDirectory(const char* path);
    };
}

#endif