#ifndef LIGHTNING3D_FILLER_COLOR_SELECTION_PANEL_HH_
#define LIGHTNING3D_FILLER_COLOR_SELECTION_PANEL_HH_

#include "Color.hh"

#include "Player.hh"

#include "../../GameObject.h"

#include <memory>
#include <vector>

#include <wrl/client.h>

namespace Lightning3D {
    class D3D12Renderer;
    class PipelineState;
    class Material;
    class MaterialTemplate;

    struct StaticMesh;
}

namespace Lightning3D::Games::Filler {
    class Board;

    class ColorSelectionPanel : public Lightning3D::GameObject {
        struct Selection : GameObject {
            Selection(Color color, float width, float height)
                : color(color)
                , rect(AttachComponent<StaticMesh>(Rectangle(width, height, color)))
                , aabb(AttachComponent<AxisAlignedBoundingBox>(Vec2F(width / 2.0f, height / 2.0f), Vec2F(width / 2.0f, height / 2.0f))) {

                std::cout << this << std::endl;
            }

            auto Update(void) -> void override {
                //const auto worldTransform = QueryComponent<Transform>()->LocalToWorld();

                //rect->SetPosition(Vec2F(worldTransform.position.x, worldTransform.position.y));
                //rect->SetScale(Vec2F(worldTransform.scale.x, worldTransform.scale.y));
            }

            Color color;
            StaticMesh *rect; 
            AxisAlignedBoundingBox *aabb;
            Material *material;
            const Player *player = nullptr;
            bool disabled = false;
            bool dirty = true;
        };

    public:
        ColorSelectionPanel(Microsoft::WRL::ComPtr<ID3D12Device> device, const MaterialTemplate &materialTemplate, Board *board, const Player &player1, const Player &player2);

        auto Update(void) -> void;

        auto Draw(D3D12Renderer &renderer, PipelineState &pso) -> void;

        auto OnClick(int32_t x, int32_t y, Player &player) -> void;

        auto GetCurrentColor(void) -> Color {
            return m_currentSelection->color;
        }

    private:
        Board *m_board;

        std::vector<std::unique_ptr<Selection>> m_selections;

        Selection *m_currentSelection = nullptr;
    };
}

#endif