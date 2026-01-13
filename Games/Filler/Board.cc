#include "Board.hh"

#include "Cell.hh"
#include "Color.hh"

#include "../../Components/Sprite.hh"
#include "../../D3D12Renderer.hh"
#include "../../Material.hh"
#include "../../PipelineState.hh"

#include <cassert>

using namespace Lightning3D::Games::Filler;

Board::Board(unsigned int width, unsigned int height, Player &player1, Player &player2, Microsoft::WRL::ComPtr<ID3D12Device> device, const MaterialTemplate &materialTemplate)
    : m_width(width)
    , m_height(height)
    , m_player(&player1) {

    assert(m_width > 1 && m_height > 1 && m_width == m_height);

    std::vector<Color> initialCellColors;

    initialCellColors.resize(m_width * m_height, Color::Unknown);

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            //if (x == player1StartingLocation.x && y == player1StartingLocation.y || x == player2StartingLocation.x && y == player2StartingLocation.y) {
            //    continue;
            //}

            auto color = RandomColor();

            if (x > 0) {
                if (y > 0) {
                    while (initialCellColors[(y - 1) * m_width + x] == color || initialCellColors[y * m_width + x - 1] == color) {
                        color = RandomColor();
                    }
                } else {
                    while (initialCellColors[y * m_width + x - 1] == color) {
                        color = RandomColor();
                    }
                }
            } else if (y > 0) {
                while (initialCellColors[(y - 1) * m_width + x] == color) {
                    color = RandomColor();
                }
            }

            auto cell = m_cells.emplace_back(new Cell(Vec2I(x, y), 50u, 50u, color, materialTemplate.Instantiate())).get();

            auto cellTransform = cell->QueryComponent<Transform>();
            cellTransform->parent = QueryComponent<Transform>();

            initialCellColors[y * height + x] = color;
        }
    }

    player1.SetColor(At(player1.GetStartingLocation())->GetColor());
    player2.SetColor(At(player2.GetStartingLocation())->GetColor());

    At(player1.GetStartingLocation())->Claim(player1);
    At(player2.GetStartingLocation())->Claim(player2);
}

auto Board::Draw(D3D12Renderer &renderer, PipelineState &pso) -> void {
    for (auto &cell : m_cells) {
        cell->Update();
        cell->Draw(renderer, pso);
    }
}

auto Board::GetAdjacentCells(const Vec2I &location) -> std::vector<Cell *> {
    std::vector<Cell *> adjacent;

    if (location.x - 1 >= 0) {
        adjacent.push_back(At(Vec2I(location.x - 1, location.y)));
    }

    if (location.x + 1 < 10) {
        adjacent.push_back(At(Vec2I(location.x + 1, location.y)));
    }

    if (location.y - 1 >= 0) {
        adjacent.push_back(At(Vec2I(location.x, location.y - 1)));
    }

    if (location.y + 1 < 10) {
        adjacent.push_back(At(Vec2I(location.x, location.y + 1)));
    }

    return adjacent;
}

auto Board::GetContour(const Player &player) -> std::vector<Cell *> {
    std::vector<Cell *> contour;

    Cell *cell = At(player.GetStartingLocation());
    Cell *prevCell = cell;

    contour.push_back(cell);

    do {
        const auto location = cell->GetLocation();

        const struct Direction {
            int x;
            int y;
        } directions[4u] = {
            {  1,   0 },
            {  0,   1 },
            { -1,   0 },
            {  0,  -1 }
        };

        int prevDirectionIndex = 0;

        if (cell->GetLocation().y < prevCell->GetLocation().y) {
            prevDirectionIndex = 1;
        } else if (cell->GetLocation().y > prevCell->GetLocation().y) {
            prevDirectionIndex = 3;
        } else if (cell->GetLocation().x < prevCell->GetLocation().x) {
            prevDirectionIndex = 0;
        } else if (cell->GetLocation().x > prevCell->GetLocation().x) {
            prevDirectionIndex = 2;
        }

        for (int index = prevDirectionIndex + 1; index != prevDirectionIndex + 5; ++index) {
            const auto &direction = directions[index % 4];

            if ((location.x + direction.x < 0) || (location.y + direction.y < 0) || (location.x + direction.x >= m_width) || (location.y + direction.y >= m_height)) {
                continue;
            }

            if (Cell *adjacentCell = At(Vec2I(location.x + direction.x, location.y + direction.y)); 1 && adjacentCell->GetColor() == player.GetColor()) {
                prevCell = cell;

                cell = adjacentCell;
                contour.push_back(cell);

                break;
            }
        }
    } while (cell->GetLocation() != player.GetStartingLocation());

    return contour;
}