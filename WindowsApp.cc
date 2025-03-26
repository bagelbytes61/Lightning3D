#include "WindowsApp.hh"

#include "ApplicationMessageHandler.hh"
#include "InputModifier.hh"

#include <cassert>
#include <cstdint>

using namespace Lightning3D;

static auto GetActiveInputModifiers(void) -> InputModifier {
    InputModifier modifiers = InputModifier::None;

    if ((GetKeyState(VK_LSHIFT) & 0x8000)) {
        modifiers |= InputModifier::LShft;
    }

    if ((GetKeyState(VK_RSHIFT) & 0x8000)) {
        modifiers |= InputModifier::RShft;
    }

    if ((GetKeyState(VK_LCONTROL) & 0x8000)) {
        modifiers |= InputModifier::LCtrl;
    }

    if ((GetKeyState(VK_RCONTROL) & 0x8000)) {
        modifiers |= InputModifier::RCtrl;
    }

    if ((GetKeyState(VK_LMENU) & 0x8000)) {
        modifiers |= InputModifier::LAlt;
    }

    if ((GetKeyState(VK_RMENU) & 0x8000)) {
        modifiers |= InputModifier::RAlt;
    }

    return modifiers;
}

WindowsApp::WindowsApp()
    : m_hInstance(GetModuleHandle(nullptr)) {

    WNDCLASSEXA wndcls = { };
    wndcls.cbSize = sizeof(WNDCLASSEX);
    wndcls.style = 0;
    wndcls.lpfnWndProc = &WindowsApp::MessageRouter;
    wndcls.cbClsExtra = 0;
    wndcls.cbWndExtra = 0;
    wndcls.hInstance = m_hInstance;
    wndcls.hIcon = nullptr;
    wndcls.hbrBackground = nullptr;
    wndcls.lpszMenuName = nullptr;
    wndcls.lpszClassName = "Lightning3D";
    wndcls.hIconSm = nullptr;

    m_clsAtom = RegisterClassEx(&wndcls);
}

WindowsApp::~WindowsApp() {
    UnregisterClass(MAKEINTATOM(m_clsAtom), m_hInstance);
}

auto WindowsApp::Run(void) -> int {

    MSG msg = { };

    //do {
    while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    //} while (msg.message != WM_QUIT);

    return 0;
}

auto WindowsApp::SetMessageHandler(ApplicationMessageHandler *handler) -> void {
    m_applicationMessageHandler = handler;
}

auto WindowsApp::MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT {
    //assert(m_applicationMessageHandler != nullptr);

    switch (uMsg) {
        case WM_CLOSE: {
            m_applicationMessageHandler->OnWindowClose();
            break;
        }
        case WM_KEYDOWN: {
            m_applicationMessageHandler->OnKeyDown(wParam, ::MapVirtualKey(wParam, MAPVK_VK_TO_VSC), GetActiveInputModifiers(), (lParam & 0x4000000));
            break;
        }
        case WM_KEYUP: {
            m_applicationMessageHandler->OnKeyUp(wParam, ::MapVirtualKey(wParam, MAPVK_VK_TO_VSC), GetActiveInputModifiers());
            break;
        }
        case WM_MOUSEMOVE: {
            int32_t positionX = LOWORD(lParam);
            int32_t positionY = HIWORD(lParam);

            m_applicationMessageHandler->OnMouseMove(positionX, positionY, GetActiveInputModifiers());
            break;
        }
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_LBUTTONDBLCLK:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_RBUTTONDBLCLK:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_MBUTTONDBLCLK: {
            int32_t positionX = LOWORD(lParam);
            int32_t positionY = HIWORD(lParam);

            switch (uMsg) {
                case WM_LBUTTONDOWN:
                    m_applicationMessageHandler->OnMouseDown(MouseButton::Left, positionX, positionY, GetActiveInputModifiers());
                    break;
                case WM_LBUTTONUP:
                    m_applicationMessageHandler->OnMouseUp(MouseButton::Left, positionX, positionY, GetActiveInputModifiers());
                    break;
                case WM_LBUTTONDBLCLK:
                    m_applicationMessageHandler->OnMouseDoubleClick(MouseButton::Left, positionX, positionY, GetActiveInputModifiers());
                    break;
                case WM_RBUTTONDOWN:
                    m_applicationMessageHandler->OnMouseDown(MouseButton::Right, positionX, positionY, GetActiveInputModifiers());
                    break;
                case WM_RBUTTONUP:
                    m_applicationMessageHandler->OnMouseUp(MouseButton::Right, positionX, positionY, GetActiveInputModifiers());
                    break;
                case WM_RBUTTONDBLCLK:
                    m_applicationMessageHandler->OnMouseDoubleClick(MouseButton::Right, positionX, positionY, GetActiveInputModifiers());
                    break;
                case WM_MBUTTONDOWN:
                    m_applicationMessageHandler->OnMouseDown(MouseButton::Middle, positionX, positionY, GetActiveInputModifiers());
                    break;
                case WM_MBUTTONUP:
                    m_applicationMessageHandler->OnMouseUp(MouseButton::Middle, positionX, positionY, GetActiveInputModifiers());
                    break;
                case WM_MBUTTONDBLCLK:
                    m_applicationMessageHandler->OnMouseDoubleClick(MouseButton::Middle, positionX, positionY, GetActiveInputModifiers());
                    break;
            }
            break;
        }
    }

    return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}

auto WindowsApp::MessageRouter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT {
    WindowsApp *app = nullptr;

    if (uMsg == WM_CREATE) {
        auto create_struct = reinterpret_cast<LPCREATESTRUCT>(lParam);

        app = static_cast<WindowsApp *>(create_struct->lpCreateParams);

        ::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(app));
    } else {
        app = reinterpret_cast<WindowsApp *>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    if (app != nullptr) {
        return app->MessageHandler(hWnd, uMsg, wParam, lParam);
    }

    return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}
