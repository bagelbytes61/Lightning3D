#include "WindowsWindow.hh"

#include "WindowsApp.hh"

using namespace Lightning3D;

WindowsWindow::WindowsWindow(WindowsApp *app, const WindowDesc &desc)
    : m_desc(desc)
    , m_hWnd(nullptr) {

    RECT clientArea = { };
    clientArea.right = desc.width;
    clientArea.bottom = desc.height;

    AdjustWindowRectEx(&clientArea, WS_OVERLAPPEDWINDOW, false, WS_EX_CLIENTEDGE);

    const auto adjustedWidth = clientArea.right - clientArea.left;
    const auto adjustedHeight = clientArea.bottom - clientArea.top;

    m_hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, MAKEINTATOM(app->GetClassHandle()), desc.title.c_str(), WS_OVERLAPPEDWINDOW, 0, 0, adjustedWidth, adjustedHeight, nullptr, nullptr, app->GetInstanceHandle(), app);

    ShowWindow(m_hWnd, SW_SHOW);
}

WindowsWindow::~WindowsWindow() {

}