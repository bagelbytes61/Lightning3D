#include "Windows/LEWindowsFileSystem.h"

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#undef CreateDirectory

using namespace Lightning3D::Engine;

void WindowsFileSystem::CreateDirectory(const char* path) {
    ::CreateDirectoryA(path, nullptr);
}