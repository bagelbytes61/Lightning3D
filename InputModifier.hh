#ifndef LE_INPUT_MODIFIER_H_
#define LE_INPUT_MODIFIER_H_

#pragma once

#include <cstdint>
#include <type_traits>

namespace Lightning3D {
    enum struct InputModifier : uint32_t {
        None = 0,

        LAlt = 1 << 0u,
        RAlt = 1 << 1u,
        LCtrl = 1 << 2u,
        RCtrl = 1 << 3u,
        LShft = 1 << 4u,
        RShft = 1 << 5u
    };

    inline auto operator&(InputModifier lhs, InputModifier rhs) -> InputModifier {
        return static_cast<InputModifier>(
            static_cast<std::underlying_type_t<InputModifier>>(lhs) &
            static_cast<std::underlying_type_t<InputModifier>>(rhs));
    }

    inline auto operator|(InputModifier lhs, InputModifier rhs) -> InputModifier {
        return static_cast<InputModifier>(
            static_cast<std::underlying_type_t<InputModifier>>(lhs) |
            static_cast<std::underlying_type_t<InputModifier>>(rhs));
    }

    inline auto operator^(InputModifier lhs, InputModifier rhs) -> InputModifier {
        return static_cast<InputModifier>(
            static_cast<std::underlying_type_t<InputModifier>>(lhs) ^
            static_cast<std::underlying_type_t<InputModifier>>(rhs));
    }

    inline auto operator&=(InputModifier &lhs, InputModifier rhs) -> InputModifier & {
        return lhs = lhs & rhs;
    }

    inline auto operator|=(InputModifier &lhs, InputModifier rhs) -> InputModifier & {
        return lhs = lhs | rhs;
    }

    inline auto operator^= (InputModifier &lhs, InputModifier rhs) -> InputModifier & {
        return lhs = lhs ^ rhs;
    }
}

#endif