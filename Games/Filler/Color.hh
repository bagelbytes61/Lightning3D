#ifndef LIGHTNING3D_FILLER_COLOR_HH_
#define LIGHTNING3D_FILLER_COLOR_HH_

#include "../../Vector.hh"

#include <random>

namespace Lightning3D::Games::Filler {
    enum struct Color {
        Black,
        Blue,
        Green,
        Purple,
        Red,
        Yellow,

        Unknown
    };

    inline auto BLACK = Vec4F{ 0.0f, 0.0f, 0.0f, 1.0f };
    inline auto BLUE = Vec4F{ 0.0f, 1.0f, 0.0f, 1.0f };
    inline auto GREEN = Vec4F{ 0.0f, 0.0f, 1.0f, 1.0f };
    inline auto PURPLE = Vec4F{ 1.0f, 1.0f, 0.0f, 1.0f };
    inline auto RED = Vec4F{ 1.0f, 0.0f, 0.0f, 1.0f };
    inline auto YELLOW = Vec4F{ 1.0f, 0.0f, 1.0f, 1.0f };

    inline const auto COLORS = std::array<Vec4F, 6u>{
        {
            BLACK, BLUE, GREEN, PURPLE, RED, YELLOW
        }
    };

    inline auto RandomColor(void) -> Color {
        return static_cast<Color>(std::rand() % 6);
    }
}

#endif