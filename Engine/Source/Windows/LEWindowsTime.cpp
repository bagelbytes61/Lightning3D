#include "Windows/LEWindowsTime.h"

#include <stdexcept>

#include <Windows.h>

using namespace Lightning3D::Engine;

void WindowsTime::Initialize() {
    if (!::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&s_resolution))) {
        throw std::runtime_error("QueryPerformanceFrequency failed!");
    }
}

uint64_t WindowsTime::GetElapsedSeconds() {
    return GetElapsedCycles() / s_resolution;
}

uint64_t WindowsTime::GetElapsedMillis() {
    return GetElapsedCycles() / (s_resolution / 1000UL);
}

uint64_t WindowsTime::GetElapsedMicros() {
    return GetElapsedCycles() / (s_resolution / 1000000UL);
}

uint64_t WindowsTime::GetElapsedCycles() {
    uint64_t cycles;
    if (!::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&cycles))) {
        throw std::runtime_error("QueryPerformanceCounter failed!");
    }

    return cycles;
}