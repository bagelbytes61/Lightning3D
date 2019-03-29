#ifndef LG_CLEAR_FLAG_H_
#define LG_CLEAR_FLAG_H_

#pragma once

#include <cstdint>
#include <type_traits>

namespace Lightning3D::Graphics {
    enum struct ClearFlag : uint32_t {
        Color   = 1 << 0,
        Depth   = 1 << 1,
        Stencil = 1 << 2
    };

    inline ClearFlag operator& (ClearFlag lhs, ClearFlag rhs) {
        return static_cast<ClearFlag>(
            static_cast<std::underlying_type_t<ClearFlag>>(lhs) &
            static_cast<std::underlying_type_t<ClearFlag>>(rhs));
    }

    inline ClearFlag operator| (ClearFlag lhs, ClearFlag rhs) {
        return static_cast<ClearFlag>(
            static_cast<std::underlying_type_t<ClearFlag>>(lhs) |
            static_cast<std::underlying_type_t<ClearFlag>>(rhs));
    }

    inline ClearFlag operator^ (ClearFlag lhs, ClearFlag rhs) {
        return static_cast<ClearFlag>(
            static_cast<std::underlying_type_t<ClearFlag>>(lhs) ^
            static_cast<std::underlying_type_t<ClearFlag>>(rhs));
    }
}

#endif