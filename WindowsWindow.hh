#ifndef LIGHTNING3D_WINDOWS_WINDOW_HH_
#define LIGHTNING3D_WINDOWS_WINDOW_HH_

#include <string>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

namespace Lightning3D {
    struct WindowDesc {
        std::string title;
        unsigned int width;
        unsigned int height;
        bool fullscreen;
    };

    class WindowsApp;

    class WindowsWindow {
    public:
        WindowsWindow(WindowsApp *app, const WindowDesc &desc);

        ~WindowsWindow();

        auto GetWidth(void) const -> unsigned int {
            return m_desc.width;
        }

        auto GetHeight(void) const -> unsigned int {
            return m_desc.height;
        }

        auto GetFullscreen(void) const -> unsigned int {
            return m_desc.fullscreen;
        }

        auto GetHandle(void) const -> HWND {
            return m_hWnd;
        }

    private:
        WindowDesc m_desc;

        HWND m_hWnd;
    };

} // namespace Lightning3D

#endif