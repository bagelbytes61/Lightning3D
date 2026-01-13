#include "ColorSelectionPanel.hh"

#include "Board.hh"

#include "../../Components/Sprite.hh"
#include "../../D3D12Renderer.hh"
#include "../../Material.hh"
#include "../../PipelineState.hh"

using namespace Lightning3D::Games::Filler;

ColorSelectionPanel::ColorSelectionPanel(Microsoft::WRL::ComPtr<ID3D12Device> device, const MaterialTemplate &materialTemplate, Board *board, const Player &player1, const Player &player2)
    : m_board(board) {

    for (size_t index = 0u; index < COLORS.size(); ++index) {
        auto selection = new Selection(static_cast<Color>(index), 50, 50);

        auto transform = selection->QueryComponent<Transform>();
        transform->position.x = index * 75.0f;
        transform->parent = QueryComponent<Transform>();

        auto aabb = selection->QueryComponent<AxisAlignedBoundingBox>();
        aabb->transform = transform;

        auto staticMesh = selection->QueryComponent<StaticMesh>();
        staticMesh->material = materialTemplate.Instantiate();

        staticMesh->material->SetVec4FVariable("Color", COLORS[static_cast<int>(selection->color)]);

        m_selections.push_back(std::unique_ptr<Selection>(selection));
    }

    m_selections[static_cast<int>(player1.GetColor())]->player = &player1;
    m_selections[static_cast<int>(player1.GetColor())]->disabled = true;

    m_selections[static_cast<int>(player2.GetColor())]->player = &player2;
    m_selections[static_cast<int>(player2.GetColor())]->disabled = true;

    m_currentSelection = m_selections[static_cast<int>(player1.GetColor())].get();
}

auto ColorSelectionPanel::Update(void) -> void {
    for (auto &selection : m_selections) {
        if (selection->dirty) {
            const Vec4F color = COLORS[static_cast<int>(selection->color)];

            auto transform = selection->QueryComponent<Transform>();
            auto staticMesh = selection->QueryComponent<StaticMesh>();

            if (selection->disabled) {
                transform->scale = Vec3F(0.75f, 0.75f, 1.0f);

                staticMesh->material->SetVec4FVariable("Color", Vec4F(color.x, color.y, color.z, 0.75f));
            } else {
                transform->scale = Vec3F(1.0f, 1.0f, 1.0f);

                staticMesh->material->SetVec4FVariable("Color", Vec4F(color.x, color.y, color.z, 1.0f));
            }

            selection->Update();

            selection->dirty = false;
        }
    }
}

auto ColorSelectionPanel::Draw(D3D12Renderer &renderer, PipelineState &pso) -> void {
    for (auto &selection : m_selections) {
        auto staticMesh = selection->QueryComponent<StaticMesh>();
        const auto transform = selection->QueryComponent<Transform>();

        const auto worldTransform = transform->LocalToWorld();

        const auto worldTransformMat = MatrixScale(worldTransform.scale.x, worldTransform.scale.y, 1.0f) * MatrixTranslation(worldTransform.position.x, worldTransform.position.y, 0.0f);

        renderer.Draw(staticMesh->vertices, worldTransformMat, staticMesh->material, pso.Get());
    }
}

auto ColorSelectionPanel::OnClick(int32_t x, int32_t y, Player &player) -> void {
    for (size_t index = 0u; index < COLORS.size(); ++index) {
        if (m_selections[index]->aabb->Contains(Vec2F(x, y))) {
            if (!m_selections[index]->disabled) {
                m_currentSelection = m_selections[index].get();

                m_currentSelection->player = &player;
                m_currentSelection->disabled = true;
                m_currentSelection->dirty = true;

                player.SetColor(m_currentSelection->color);

                auto contour = m_board->GetContour(player);

                for (auto cell : contour) {
                    for (auto adjacentCell : m_board->GetAdjacentCells(cell->GetLocation())) {
                        if (adjacentCell->GetColor() == player.GetColor()) {
                            adjacentCell->Claim(player);
                        }
                    }
                }

                for (auto &selection : m_selections) {
                    if (selection.get() != m_currentSelection && selection->player == &player) {
                        selection->disabled = false;
                        selection->dirty = true;
                    }
                }
            }
        }
    }
}