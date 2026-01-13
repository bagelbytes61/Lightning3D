#ifndef LIGHTNING3D_FILLER_CELL_HH_
#define LIGHTNING3D_FILLER_CELL_HH_

#include "../../GameObject.h"

#include "Color.hh"
#include "Player.hh"
#include "../../Vector.hh"

namespace Lightning3D {
    class D3D12Renderer;
    class PipelineState;
    class Material;

    struct StaticMesh;
}

namespace Lightning3D::Games::Filler {
    class Cell : public Lightning3D::GameObject {
    public:
        Cell(const Vec2I &location, float width, float height, Color initialColor, Material *material);

        auto Claim(const Player &player) -> void {
            m_player = &player;
        }

        auto GetColor(void) const -> Color {
            return m_player != nullptr ? m_player->GetColor() : m_initialColor;
        }

        auto GetLocation(void) const -> const Vec2I & {
            return m_location;
        }

        auto GetOwner(void) const -> const Player * {
            return m_player;
        }

        auto Update(void) -> void;

        auto Draw(D3D12Renderer &renderer, PipelineState &pso) -> void;

        auto SetColor(Color color) -> void {
            m_initialColor = color;
        }

    private:
        Vec2I m_location;

        Color m_initialColor;

        const Player *m_player = nullptr;

        StaticMesh *rect = nullptr;
    };
}

#endif