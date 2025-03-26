#ifndef LIGHTNING3D_WINDOWS_APP_HH_
#define LIGHTNING3D_WINDOWS_APP_HH_

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

namespace Lightning3D {
    struct ApplicationMessageHandler;
    struct WindowsWindow;

    class WindowsApp {
    public:
        WindowsApp();

        ~WindowsApp();

        auto Run(void) -> int;

        auto SetMessageHandler(ApplicationMessageHandler *handler) -> void;

        auto GetInstanceHandle(void) const -> HINSTANCE {
            return m_hInstance;
        }

        auto GetClassHandle(void) const -> ATOM {
            return m_clsAtom;
        }

    private:
        auto MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)->LRESULT;

    private:
        static auto MessageRouter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)->LRESULT;

    private:
        HINSTANCE m_hInstance;

        ATOM m_clsAtom;

        ApplicationMessageHandler *m_applicationMessageHandler{ nullptr };
    };

} // namespace Lightning3D

#endif