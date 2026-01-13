#ifndef LIGHTNING3D_FILLER_FILLER_HH_
#define LIGHTNING3D_FILLER_FILLER_HH_

#include "../../ApplicationMessageHandler.hh"
#include "../../Camera.hh"

#include <algorithm>
#include <array>
#include <cassert>
#include <memory>
#include <random>
#include <tuple>
#include <vector>

#include <wrl/client.h>

namespace Lightning3D {
    class D3D12Renderer;
    class Shader;
    class PipelineState;
}

namespace Lightning3D::Games::Filler {
    class Board;
    class ColorSelectionPanel;
    class Player;

    class Filler : public ApplicationMessageHandler {
        static constexpr auto BOARD_WIDTH = 10;
        static constexpr auto BOARD_HEIGHT = 10;

    public:
        Filler(Microsoft::WRL::ComPtr<ID3D12Device> device, const Vec2I &windowSize);

        ~Filler();

        auto Draw(D3D12Renderer &renderer) -> void;

    private:
        virtual auto OnMouseDown(MouseButton button, int32_t x, int32_t y, InputModifier modifiers) -> bool override;

    private:
        std::unique_ptr<Camera> m_camera;
        std::unique_ptr<Viewport> m_viewport;

        std::unique_ptr<Shader> m_vs;
        std::unique_ptr<Shader> m_ps;

        std::unique_ptr<Player> m_player1;
        std::unique_ptr<Player> m_player2;

        Player *m_activePlayer = nullptr;

        std::unique_ptr<Board> m_board;
        std::unique_ptr<ColorSelectionPanel> m_colorSelectionPanel;

        Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
        std::unique_ptr<PipelineState> m_pso;
    };
}

#endif
