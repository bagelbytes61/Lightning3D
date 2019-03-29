#include "LEPlatformApplication.h"

#include "LEEngine.h"

#include <iostream>
#include <string>

using namespace Lightning3D::Engine;

#if defined (LE_WINDOWS)
#include "Windows/LEWindowsApplication.h"
using PlatformImplementation = WindowsApplication;
#endif

static std::string InputModifiersToString(InputModifier modifiers) {
    std::string modifierString;

    if (static_cast<bool>(modifiers & InputModifier::LAlt)) {
        modifierString += "LAlt | ";
    }

    if (static_cast<bool>(modifiers & InputModifier::RAlt)) {
        modifierString += "RAlt | ";
    }

    if (static_cast<bool>(modifiers & InputModifier::LCtrl)) {
        modifierString += "LCtrl | ";
    }

    if (static_cast<bool>(modifiers & InputModifier::RCtrl)) {
        modifierString += "RCtrl | ";
    }

    if (static_cast<bool>(modifiers & InputModifier::LShft)) {
        modifierString += "LShft | ";
    }

    if (static_cast<bool>(modifiers & InputModifier::RShft)) {
        modifierString += "RShft | ";
    }

    return modifierString;
}

std::unique_ptr<PlatformApplication> PlatformApplication::Create() {
    return std::make_unique<PlatformImplementation>();
}

PlatformApplication::~PlatformApplication() {

}

bool PlatformApplication::OnKeyDown(int32_t keyCode, uint32_t charCode, InputModifier modifiers, bool repeat) {
    return false;
}

bool PlatformApplication::OnKeyUp(int32_t keyCode, uint32_t charCode, InputModifier modifiers) {
    return false;
}

bool PlatformApplication::OnMouseDown(MouseButton button, int32_t x, int32_t y, InputModifier modifiers) {
    std::cout << "MouseDown @ ( " << x << ", " << y << " ) Modifiers: " << InputModifiersToString(modifiers) << "\n";

    return false;
}

bool PlatformApplication::OnMouseUp(MouseButton button, int32_t x, int32_t y, InputModifier modifiers) {
    std::cout << "MouseUp @ ( " << x << ", " << y << " ) Modifiers: " << InputModifiersToString(modifiers) << "\n";

    return false;
}

bool PlatformApplication::OnMouseDoubleClick(MouseButton button, int32_t x, int32_t y, InputModifier modifiers) {
    std::cout << "MouseDoubleClick @ ( " << x << ", " << y << " ) Modifiers: " << InputModifiersToString(modifiers) << "\n";

    return false;
}

bool PlatformApplication::OnMouseMove(int32_t x, int32_t y, InputModifier modifiers) {
    std::cout << "MouseMove @ ( " << x << ", " << y << " ) Modifiers: " << InputModifiersToString(modifiers) << "\n";

    return false;
}

void PlatformApplication::OnWindowClose() {
    Engine::s_running = false;
}