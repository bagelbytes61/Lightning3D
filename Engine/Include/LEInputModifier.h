#ifndef LE_INPUT_MODIFIER_H_
#define LE_INPUT_MODIFIER_H_

#pragma once

#include <cstdint>
#include <type_traits>

namespace Lightning3D::Engine {
    enum struct InputModifier : uint32_t {
        None = 0,

        LAlt  = 1 << 0,
        RAlt  = 1 << 1,
        LCtrl = 1 << 2,
        RCtrl = 1 << 3,
        LShft = 1 << 4,
        RShft = 1 << 5
    };

    inline InputModifier operator&(InputModifier lhs, InputModifier rhs) {
        return static_cast<InputModifier>(
            static_cast<std::underlying_type_t<InputModifier>>(lhs) & 
            static_cast<std::underlying_type_t<InputModifier>>(rhs));
    }

    inline InputModifier operator|(InputModifier lhs, InputModifier rhs) {
        return static_cast<InputModifier>(
            static_cast<std::underlying_type_t<InputModifier>>(lhs) |
            static_cast<std::underlying_type_t<InputModifier>>(rhs));
    }

    inline InputModifier operator^(InputModifier lhs, InputModifier rhs) {
        return static_cast<InputModifier>(
            static_cast<std::underlying_type_t<InputModifier>>(lhs) ^
            static_cast<std::underlying_type_t<InputModifier>>(rhs));
    }

    inline InputModifier& operator&=(InputModifier& lhs, InputModifier rhs) {
        return lhs = lhs & rhs;
    }

    inline InputModifier& operator|=(InputModifier& lhs, InputModifier rhs) {
        return lhs = lhs | rhs;
    }

    inline InputModifier& operator^= (InputModifier& lhs, InputModifier rhs) {
        return lhs = lhs ^ rhs;
    }
}

#endif