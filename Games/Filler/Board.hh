#ifndef LIGHTNING3D_FILLER_BOARD_HH_
#define LIGHTNING3D_FILLER_BOARD_HH_

#include "../../GameObject.h"

#include "Cell.hh"
#include "Color.hh"

#include <vector>

#include <wrl/client.h>

namespace Lightning3D {
    class D3D12Renderer;
    class PipelineState;
    class MaterialTemplate;

    struct StaticMesh;
}

namespace Lightning3D::Games::Filler {
    class Player;

    class Board : public Lightning3D::GameObject {
    public:
        Board(unsigned int width, unsigned int height, Player &player1, Player &player2, Microsoft::WRL::ComPtr<ID3D12Device> device, const MaterialTemplate &materialTemplate);

        auto operator[](const Vec2I &location) -> Cell * {
            return At(location);
        }

        auto Draw(D3D12Renderer &renderer, PipelineState &pso) -> void;

        auto GetAdjacentCells(const Vec2I &location) -> std::vector<Cell *>;

        auto GetContour(const Player &player) -> std::vector<Cell *>;

    private:
        auto At(const Vec2I &location) -> Cell * {
            return m_cells[location.y * m_width + location.x].get();
        }

        auto At(const Vec2I &location) const -> const Cell * {
            return m_cells[location.y * m_width + location.x].get();
        }

    private:
        unsigned int m_width;
        unsigned int m_height;

        std::vector<std::unique_ptr<Cell>> m_cells;

        Player *m_player;
    };
}

#endif