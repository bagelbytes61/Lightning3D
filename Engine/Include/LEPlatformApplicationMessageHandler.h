#ifndef LE_APPLICATION_MESSAGE_HANDLER_H_
#define LE_APPLICATION_MESSAGE_HANDLER_H_

#pragma once

#include <cstdint>

#include "LEInputModifier.h"

namespace Lightning3D::Engine {
    enum struct MouseButton : uint32_t {
        None,

        Left,
        Right,
        Middle,
    };

    struct PlatformApplicationMessageHandler {
        virtual bool OnKeyDown(int32_t keyCode, uint32_t charCode, InputModifier modifiers, bool repeat) = 0;

        virtual bool OnKeyUp(int32_t keyCode, uint32_t charCode, InputModifier modifiers) = 0;

        virtual bool OnMouseDown(MouseButton button, int32_t x, int32_t y, InputModifier modifiers) = 0;

        virtual bool OnMouseUp(MouseButton button, int32_t x, int32_t y, InputModifier modifiers) = 0;

        virtual bool OnMouseDoubleClick(MouseButton button, int32_t x, int32_t y, InputModifier modifiers) = 0;

        virtual bool OnMouseMove(int32_t x, int32_t y, InputModifier modifiers) = 0;

        virtual void OnWindowClose() = 0;
    };

    inline bool PlatformApplicationMessageHandler::OnKeyDown(int32_t keyCode, uint32_t charCode, InputModifier modifiers, bool repeat) {
        return false;
    }

    inline bool PlatformApplicationMessageHandler::OnKeyUp(int32_t keyCode, uint32_t charCode, InputModifier modifiers) {
        return false;
    }

    inline bool PlatformApplicationMessageHandler::OnMouseDown(MouseButton button, int32_t x, int32_t y, InputModifier modifiers) {
        return false;
    }

    inline bool PlatformApplicationMessageHandler::OnMouseUp(MouseButton, int32_t x, int32_t y, InputModifier modifiers) {
        return false;
    }

    inline bool PlatformApplicationMessageHandler::OnMouseDoubleClick(MouseButton button, int32_t x, int32_t y, InputModifier modifiers) {
        return false;
    }

    inline bool PlatformApplicationMessageHandler::OnMouseMove(int32_t x, int32_t y, InputModifier modifiers) {
        return false;
    }

    inline void PlatformApplicationMessageHandler::OnWindowClose() {

    }
}

#endif