#ifndef LE_WINDOWS_APPLICATION_H_
#define LE_WINDOWS_APPLICATION_H_

#pragma once

#include "LEPlatformApplication.h"

#include <memory>

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

namespace Lightning3D::Engine {

    class WindowsApplication : public PlatformApplication {
    public:
        WindowsApplication();
        virtual ~WindowsApplication() override final;

        virtual void PumpMessages() override final;

        virtual void SetMessageHandler(std::shared_ptr<PlatformApplicationMessageHandler> handler) override final;

        virtual void InitializeWindow(std::shared_ptr<PlatformWindow>& window, std::shared_ptr<PlatformWindowDescription> description) override final;

    private:
        static  LRESULT CALLBACK MessageRouter(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

        LRESULT CALLBACK MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    private:
        HINSTANCE m_hInstance;

        LPCSTR m_className;
    };
}

#endif