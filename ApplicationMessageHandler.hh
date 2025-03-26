#ifndef LIGHTNING3D_APPLICATION_MESSAGE_HANDLER_HH_
#define LIGHTNING3D_APPLICATION_MESSAGE_HANDLER_HH_

#include "InputModifier.hh"

#include <cstdint>

namespace Lightning3D {
    enum struct Key : uint32_t {
        Back,
        Tab,
        Clear,
        Enter,
        Shift,
        Ctrl,
        Alt,
        Pause,
        Caps,
        Escape,
        Space,
        PageUp,
        PageDown,
        End,
        Home,
        Left,
        Up,
        Right,
        Down,
        Select,
        Print,
        Execute,
        PrintScreen,
        Insert,
        Delete,
        Help,
        _0,
        _1,
        _2,
        _3,
        _4,
        _5,
        _6,
        _7,
        _8,
        _9,
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,
        NumPad_0,
        NumPad_1,
        NumPad_2,
        NumPad_3,
        NumPad_4,
        NumPad_5,
        NumPad_6,
        NumPad_7,
        NumPad_8,
        NumPad_9,
        NumPad_Multiply,
        NumPad_Add,
        NumPad_Enter,
        NumPad_Subtract,
        NumPad_Divide,
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        F13,
        F14,
        F15,
        F16,
        F17,
        F18,
        F19,
        F20,
        F21,
        F22,
        F23,
        F24,
        NumLock,
        Scroll
    };

    enum struct MouseButton : uint32_t {
        None,

        Left,
        Right,
        Middle,
    };

    struct ApplicationMessageHandler {
        virtual ~ApplicationMessageHandler() = default;

        virtual auto OnKeyDown(int32_t keyCode, uint32_t charCode, InputModifier modifiers, bool repeat) -> bool {
            return false;
        }

        virtual auto OnKeyUp(int32_t keyCode, uint32_t charCode, InputModifier modifiers) -> bool {
            return false;
        }

        virtual auto OnMouseDown(MouseButton button, int32_t x, int32_t y, InputModifier modifiers) -> bool {
            return false;
        }

        virtual auto OnMouseUp(MouseButton button, int32_t x, int32_t y, InputModifier modifiers) -> bool {
            return false;
        }

        virtual auto OnMouseDoubleClick(MouseButton button, int32_t x, int32_t y, InputModifier modifiers) -> bool {
            return false;
        }

        virtual auto OnMouseMove(int32_t x, int32_t y, InputModifier modifiers) -> bool {
            return false;
        }

        virtual auto OnWindowClose(void) -> void { }
    };
} // namespace Lightning3D

#endif