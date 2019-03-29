#include "LEPlatformTime.h"

using namespace Lightning3D::Engine;

#if defined (LE_WINDOWS)
#include "Windows/LEWindowsTime.h"
using PlatformImplementation = WindowsTime;
#endif

void PlatformTime::Initialize() {
    PlatformImplementation::Initialize();
}

uint64_t PlatformTime::GetElapsedSeconds() {
    return PlatformImplementation::GetElapsedSeconds();
}

uint64_t PlatformTime::GetElapsedMillis() {
    return PlatformImplementation::GetElapsedMillis();
}

uint64_t PlatformTime::GetElapsedMicros() {
    return PlatformImplementation::GetElapsedMicros();
}

uint64_t PlatformTime::GetElapsedCycles() {
    return PlatformImplementation::GetElapsedCycles();
}






















//#include "LETime.h"
//
//#define WIN32_LEAN_AND_MEAN
//
//#include <Windows.h>
//
//#include <ctime>
//
//using namespace Lightning3D::Engine;
//
//CTime::CTime() 
//    : m_ui64Resolution(0ULL)
//    , m_ui64CurrRealTime(0ULL)
//    , m_ui64LastRealTime(0ULL)
//    , m_ui64CurrVirtTime(0ULL)
//    , m_ui64LastVirtTime(0ULL)
//    , m_ui64CurrRealSecs(0ULL)
//    , m_ui64CurrVirtSecs(0ULL)
//    , m_ui64CurrRealMillis(0ULL)
//    , m_ui64CurrVirtMillis(0ULL)
//    , m_ui64CurrRealMicros(0ULL)
//    , m_ui64CurrVirtMicros(0ULL)
//    , m_bHiRes(false) {
//
//    m_bHiRes = ::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&m_ui64Resolution));
//    if (!m_bHiRes) {
//        m_ui64Resolution = CLOCKS_PER_SEC;
//    }
//
//    m_ui64LastRealTime = GetRealTime();
//}
//
//CTime::~CTime() {
//
//}
//
//void CTime::Update(bool _bUpdateVirtuals) {
//    uint64_t ui64RealTime = GetRealTime();
//    uint64_t ui64Dif;
//
//    if (ui64RealTime >= m_ui64LastRealTime) {
//        ui64Dif = ui64RealTime - m_ui64LastRealTime;
//    }
//    else {
//        ui64Dif = (0xFFFFFFFFFFFFFFFFULL - m_ui64LastRealTime) + ui64RealTime + 1ULL;
//    }
//
//    m_ui64LastRealTime = ui64RealTime;
//
//    UpdateBy(ui64Dif, _bUpdateVirtuals);
//}
//
//void CTime::UpdateBy(uint64_t _ui64Amt, bool _bUpdateVirtuals) {
//    m_ui64LastRealTime = m_ui64CurrRealTime;
//    m_ui64CurrRealTime += _ui64Amt;
//
//    m_ui64LastVirtTime = m_ui64CurrVirtTime;
//
//    m_ui64CurrRealSecs = m_ui64CurrRealTime / m_ui64Resolution;
//    m_ui64CurrRealMillis = m_ui64CurrRealTime * 1000ULL / m_ui64Resolution;
//    m_ui64CurrRealMicros = m_ui64CurrRealTime * 1000000ULL / m_ui64Resolution;
//
//    if (_bUpdateVirtuals) {
//        m_ui64LastVirtTime = m_ui64CurrVirtTime;
//        m_ui64CurrVirtTime += _ui64Amt;
//
//        m_ui64CurrVirtSecs = m_ui64CurrVirtTime / m_ui64Resolution;
//        m_ui64CurrVirtMillis = m_ui64CurrVirtTime * 1000ULL / m_ui64Resolution;
//        m_ui64CurrVirtMicros = m_ui64CurrVirtTime * 1000000ULL / m_ui64Resolution;
//    }
//}
//    
//uint64_t CTime::GetRealTime() const {
//    if (m_bHiRes) {
//        uint64_t ui64RealTime;
//        if (::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&ui64RealTime))) {
//            return ui64RealTime;
//        }
//    }
//
//    return ::clock();
//}
