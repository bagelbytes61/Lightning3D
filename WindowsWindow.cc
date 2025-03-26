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

    const auto adjustedWith = clientArea.right - clientArea.left;
    const auto adjustedHeight = clientArea.bottom - clientArea.top;

    m_hWnd = CreateWindowEx(0U, MAKEINTATOM(app->GetClassHandle()), desc.title.c_str(), 0U, 0, 0, adjustedWith, adjustedHeight, nullptr, nullptr, app->GetInstanceHandle(), app);

    ShowWindow(m_hWnd, SW_SHOW);
}

WindowsWindow::~WindowsWindow() {

}