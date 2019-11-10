#include "Windows/LEWindowsWindow.h"

#include "LEPlatformWindowDescription.h"

#include <cassert>

using namespace Lightning3D;

WindowsWindow::WindowsWindow(WindowsApplication* application, std::shared_ptr<PlatformWindowDescription> description, HINSTANCE hInstance, LPCSTR className)
    : m_hWnd(nullptr)
    , PlatformWindow(std::move(description)) {

    RECT clientArea = { 0, 0,
        m_description->windowed ? m_description->windowedWidth  : m_description->fullscreenWidth  == 0 ? GetSystemMetrics(SM_CXSCREEN) : m_description->fullscreenWidth,
        m_description->windowed ? m_description->windowedHeight : m_description->fullscreenHeight == 0 ? GetSystemMetrics(SM_CYSCREEN) : m_description->fullscreenHeight };

    DWORD exStyle   = m_description->windowed ? WS_EX_OVERLAPPEDWINDOW : WS_EX_APPWINDOW;
    DWORD style     = m_description->windowed ? WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME : WS_POPUPWINDOW;

    AdjustWindowRectEx(&clientArea, style, FALSE, exStyle);

    INT adjustedWidth   = clientArea.right - clientArea.left;
    INT adjustedHeight  = clientArea.bottom - clientArea.top;

    INT positionX = (GetSystemMetrics(SM_CXSCREEN) - adjustedWidth) / 2;
    INT positionY = (GetSystemMetrics(SM_CYSCREEN) - adjustedHeight) / 2;

    m_hWnd = ::CreateWindowEx(exStyle,
        className, m_description->title,
        style,
        positionX, positionY,
        adjustedWidth, adjustedHeight,
        nullptr, nullptr,
        hInstance,
        application);

    assert(m_hWnd);

    ::ShowWindow(m_hWnd, SW_SHOW);
}

WindowsWindow::~WindowsWindow() {

}