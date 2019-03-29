#include "LEPlatformWindow.h"

#include "LEPlatformWindowDescription.h"

using namespace Lightning3D::Engine;

PlatformWindow::~PlatformWindow() {

}

PlatformWindow::PlatformWindow(std::shared_ptr<PlatformWindowDescription> description)
    : m_description(description) {

}