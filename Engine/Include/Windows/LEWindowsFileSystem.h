#ifndef LE_WINDOWS_FILE_SYSTEM_H_
#define LE_WINDOWS_FILE_SYSTEM_H_

#pragma once

#include "LEPlatformFileSystem.h"

namespace Lightning3D {
    class WindowsFileSystem : public PlatformFileSystem {
    public:
        static void CreateDirectory(const char* path);

    };
}

#endif