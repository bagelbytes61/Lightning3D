#ifndef LG_CLEAR_H_
#define LG_CLEAR_H_

#pragma once

#include <cstdint>
#include <type_traits>

namespace Lightning3D {
    enum struct Clear : uint32_t {
        Color   = 1 << 0,
        Depth   = 1 << 1,
        Stencil = 1 << 2
    };

    inline Clear operator& (Clear lhs, Clear rhs) {
        return static_cast<Clear>(
            static_cast<std::underlying_type_t<Clear>>(lhs) &
            static_cast<std::underlying_type_t<Clear>>(rhs));
    }

    inline Clear operator| (Clear lhs, Clear rhs) {
        return static_cast<Clear>(
            static_cast<std::underlying_type_t<Clear>>(lhs) |
            static_cast<std::underlying_type_t<Clear>>(rhs));
    }

    inline Clear operator^ (Clear lhs, Clear rhs) {
        return static_cast<Clear>(
            static_cast<std::underlying_type_t<Clear>>(lhs) ^
            static_cast<std::underlying_type_t<Clear>>(rhs));
    }
}

#endif