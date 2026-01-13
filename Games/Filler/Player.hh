#ifndef LIGHTNING3D_FILLER_PLAYER_HH_
#define LIGHTNING3D_FILLER_PLAYER_HH_

#include "Color.hh"

namespace Lightning3D::Games::Filler {
    class Player {
    public:
        Player(Color initialColor, const Vec2I &startingLocation)
            : m_color(initialColor)
            , m_startingLocation(startingLocation) {
        }

        auto SetColor(Color color) -> void {
            m_color = color;
        }

        auto GetColor(void) const -> Color {
            return m_color;
        }

        auto GetStartingLocation(void) const -> const Vec2I & {
            return m_startingLocation;
        }

    private:
        Color m_color;

        Vec2I m_startingLocation;
    };
}

#endif