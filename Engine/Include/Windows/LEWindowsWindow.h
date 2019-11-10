#ifndef LE_WINDOWS_WINDOW_H_
#define LE_WINDOWS_WINDOW_H_

#pragma once

#include "LEPlatformWindow.h"

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

namespace Lightning3D {
    class WindowsApplication;

    class WindowsWindow : public PlatformWindow {
    public:
        WindowsWindow(WindowsApplication* application, std::shared_ptr<PlatformWindowDescription> description, HINSTANCE hInstance, LPCSTR className);
        virtual ~WindowsWindow() override final;

        virtual Handle  GetHandle() const override final;

    private:
        HWND m_hWnd;
    };

    inline Handle WindowsWindow::GetHandle() const {
        return m_hWnd;
    }
}

#endif