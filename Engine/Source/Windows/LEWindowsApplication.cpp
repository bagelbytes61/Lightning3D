#include "Windows/LEWindowsApplication.h"

#include "LEPlatformApplicationMessageHandler.h"
#include "LEPlatformWindowDescription.h"
#include "LEInputModifier.h"

#include "Windows/LEWindowsWindow.h"

using namespace Lightning3D;

static InputModifier GetActiveInputModifiers() {
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

WindowsApplication::WindowsApplication() 
    : m_hInstance(::GetModuleHandle(nullptr))
    , m_className("Lightning3D") {

    WNDCLASS wndclass = { };
    wndclass.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_OWNDC;
    wndclass.lpfnWndProc = MessageRouter;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = m_hInstance;
    wndclass.hIcon = nullptr;
    wndclass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW);
    wndclass.lpszMenuName = nullptr;
    wndclass.lpszClassName = m_className;

    ::RegisterClass(&wndclass);
}

WindowsApplication::~WindowsApplication() {
    UnregisterClass(m_className, m_hInstance);
}

void WindowsApplication::PumpMessages() {
    MSG msg = { };
    while (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
    }
}

void WindowsApplication::SetMessageHandler(std::shared_ptr<PlatformApplicationMessageHandler> handler) {
    PlatformApplication::SetMessageHandler(std::move(handler));
}

void WindowsApplication::InitializeWindow(std::shared_ptr<PlatformWindow>& pWindow, std::shared_ptr<PlatformWindowDescription> description) {
    pWindow = std::make_shared<WindowsWindow>(this, std::move(description), m_hInstance, m_className);
}

LRESULT CALLBACK WindowsApplication::MessageRouter(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    WindowsApplication* application;

    if (msg == WM_NCCREATE) {
        LPCREATESTRUCT createStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);

        application = static_cast<WindowsApplication*>(createStruct->lpCreateParams);

        ::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(application));
    }
    else {
        application = reinterpret_cast<WindowsApplication*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    if (application) {
        return application->MessageHandler(hWnd, msg, wParam, lParam);
    }

    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK WindowsApplication::MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CLOSE: {
            m_messageHandler->OnWindowClose();
            break;
        }
        case WM_KEYDOWN: {
            m_messageHandler->OnKeyDown(wParam, ::MapVirtualKey(wParam, MAPVK_VK_TO_VSC), GetActiveInputModifiers(), (lParam & 0x4000000));
            break;
        }
        case WM_KEYUP: {
            m_messageHandler->OnKeyUp(wParam, ::MapVirtualKey(wParam, MAPVK_VK_TO_VSC), GetActiveInputModifiers());
            break;
        }
        case WM_MOUSEMOVE: {
            int32_t positionX = LOWORD(lParam);
            int32_t positionY = HIWORD(lParam);

            m_messageHandler->OnMouseMove(positionX, positionY, GetActiveInputModifiers());
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

            switch (msg) {
                case WM_LBUTTONDOWN:
                    m_messageHandler->OnMouseDown(MouseButton::Left, positionX, positionY, GetActiveInputModifiers());
                    break;
                case WM_LBUTTONUP:
                    m_messageHandler->OnMouseUp(MouseButton::Left, positionX, positionY, GetActiveInputModifiers());
                    break;
                case WM_LBUTTONDBLCLK:
                    m_messageHandler->OnMouseDoubleClick(MouseButton::Left, positionX, positionY, GetActiveInputModifiers());
                    break;
                case WM_RBUTTONDOWN:
                    m_messageHandler->OnMouseDown(MouseButton::Right, positionX, positionY, GetActiveInputModifiers());
                    break;
                case WM_RBUTTONUP:
                    m_messageHandler->OnMouseUp(MouseButton::Right, positionX, positionY, GetActiveInputModifiers());
                    break;
                case WM_RBUTTONDBLCLK:
                    m_messageHandler->OnMouseDoubleClick(MouseButton::Right, positionX, positionY, GetActiveInputModifiers());
                    break;
                case WM_MBUTTONDOWN:
                    m_messageHandler->OnMouseDown(MouseButton::Middle, positionX, positionY, GetActiveInputModifiers());
                    break;
                case WM_MBUTTONUP:
                    m_messageHandler->OnMouseUp(MouseButton::Middle, positionX, positionY, GetActiveInputModifiers());
                    break;
                case WM_MBUTTONDBLCLK:
                    m_messageHandler->OnMouseDoubleClick(MouseButton::Middle, positionX, positionY, GetActiveInputModifiers());
                    break;
                }
            }
            break;
        }

    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}