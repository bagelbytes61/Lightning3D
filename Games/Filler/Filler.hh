#ifndef LIGHTNING3D_FILLER_FILLER_HH_
#define LIGHTNING3D_FILLER_FILLER_HH_

#include "../../ApplicationMessageHandler.hh"
#include "../../GameObject.h"
#include "../../Sprite.hh"
#include "../../SpriteFactory.hh"

#include <algorithm>
#include <array>
#include <cassert>
#include <memory>
#include <random>
#include <tuple>
#include <vector>

namespace Lightning3D::Games::Filler {
    inline auto GenerateRootSignature(ID3D12Device *device) -> ComPtr<ID3D12RootSignature> {
        HRESULT hr;

        //D3D12_DESCRIPTOR_RANGE descriptorRange = { };
        //descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
        //descriptorRange.NumDescriptors = 2u;
        //descriptorRange.BaseShaderRegister = 0u;
        //descriptorRange.RegisterSpace = 0u;
        //descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

        //D3D12_ROOT_DESCRIPTOR_TABLE descriptorTable = { };
        //descriptorTable.NumDescriptorRanges = 1u;
        //descriptorTable.pDescriptorRanges = &descriptorRange;

        D3D12_ROOT_DESCRIPTOR globalConstants;
        globalConstants.RegisterSpace = 0u;
        globalConstants.ShaderRegister = 0u;

        //D3D12_ROOT_DESCRIPTOR objectConstants;
        //objectConstants.RegisterSpace = 0u;
        //objectConstants.ShaderRegister = 1u;

        D3D12_ROOT_PARAMETER rootParam0 = { };
        rootParam0.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
        rootParam0.Descriptor = globalConstants;
        rootParam0.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

        //D3D12_ROOT_PARAMETER rootParam1 = { };
        //rootParam1.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
        //rootParam1.Descriptor = objectConstants;
        //rootParam1.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;


        D3D12_ROOT_PARAMETER rootParamPS = { };
        rootParamPS.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
        rootParamPS.Descriptor.RegisterSpace = 0u;
        rootParamPS.Descriptor.ShaderRegister = 1u;
        rootParamPS.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

        D3D12_ROOT_PARAMETER rootParams[] = { rootParam0, rootParamPS };

        D3D12_ROOT_SIGNATURE_DESC desc = { };
        desc.NumParameters = 2u;
        desc.pParameters = rootParams;
        desc.NumStaticSamplers = 0u;
        desc.pStaticSamplers = nullptr;
        desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

        Microsoft::WRL::ComPtr<ID3DBlob> signature;
        Microsoft::WRL::ComPtr<ID3DBlob> error;

        hr = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
        if (FAILED(hr)) {
            throw std::runtime_error("Failed to serialize Root Signature: " + std::to_string(hr));
        }

        ComPtr<ID3D12RootSignature> rootSignature;

        hr = device->CreateRootSignature(0U, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
        if (FAILED(hr)) {
            throw std::runtime_error("Failed to create Root Signature: " + std::to_string(hr));
        }

        return rootSignature;
    }

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

    class Cell : public GameObject {
    public:
        Cell(const Vec2I &location, float width, float height, Color initialColor, Material *material)
            : m_location(location)
            , m_initialColor(initialColor)
            , m_material(material) {

            auto rectangle = AttachComponent(Rectangle(width, height, COLORS[static_cast<int>(initialColor)]));

            //m_material->SetVec4FVariable("Color", COLORS[static_cast<int>(initialColor)]);

            auto transform = QueryComponent<Transform>();
            transform->position.x = m_location.x * width;
            transform->position.y = m_location.y * height;
        }

        Cell(const Cell &other)
            : m_material(other.m_material) {

            std::cout << "Cell copy ctor" << std::endl;
        }

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

        auto Update(void) -> void {
            //auto [transform, sprite] = QueryComponents<Transform, Sprite>();

            //const auto worldTransform = transform->LocalToWorld();

            //rect->SetPosition(Vec2F(worldTransform.position.x, worldTransform.position.y));
            //rect->SetScale(Vec2F(worldTransform.scale.x, worldTransform.scale.y));
            
            //if (m_player != nullptr) {
            //    rect->GetMaterial()->SetVec4FVariable("Color", COLORS[static_cast<size_t>(m_player->GetColor())]);
            //}
        }

        auto Draw(D3D12Renderer &renderer, PipelineState &pso) -> void {
            auto rect = QueryComponent<Rectangle>();

            renderer.Draw(*rect, pso.Get());
        }

        auto SetColor(Color color) -> void {
            m_initialColor = color;
        }

    private:
        Vec2I m_location;

        Color m_initialColor;

        const Player *m_player = nullptr;

        Material *m_material;
    };


    class Board : public GameObject {
    public:
        Board(unsigned int width, unsigned int height, Player &player1, Player &player2, ComPtr<ID3D12Device> device, const MaterialTemplate &materialTemplate)
            : m_width(width)
            , m_height(height)
            , m_player(&player1) {

            assert(m_width > 1 && m_height > 1 && m_width == m_height);

            std::vector<Color> initialCellColors;

            initialCellColors.resize(m_width * m_height, Color::Unknown);

            //const auto player1StartingLocation = player1.GetStartingLocation();
            //const auto player2StartingLocation = player1.GetStartingLocation();

            //initialCellColors[player1StartingLocation.y * m_width + player1StartingLocation.x] = player1.GetColor();
            //initialCellColors[player2StartingLocation.y * m_width + player2StartingLocation.x] = player2.GetColor();

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

        auto operator[](const Vec2I &location) -> Cell * {
            return At(location);
        }

        auto Draw(D3D12Renderer &renderer, PipelineState &pso) -> void {
            for (auto &cell : m_cells) {
                cell->Update();
                cell->Draw(renderer, pso);
            }
        }

        auto GetAdjacentCells(const Vec2I &location) -> std::vector<Cell *> {
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

        auto GetContour(const Player &player) -> std::vector<Cell *> {
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

    class ColorSelectionPanel : public GameObject {
        struct Selection : GameObject {
            Selection(Color color, float width, float height)
                : color(color)
                //, rect(AttachComponent<Rectangle>(width, height))
                , aabb(AttachComponent<AxisAlignedBoundingBox>(Vec2F(width / 2.0f, height / 2.0f), Vec2F(width / 2.0f, height / 2.0f))) {

                std::cout << this << std::endl;
            }

            auto Update(void) -> void override {
                const auto worldTransform = QueryComponent<Transform>()->LocalToWorld();

                //rect->SetPosition(Vec2F(worldTransform.position.x, worldTransform.position.y));
                //rect->SetScale(Vec2F(worldTransform.scale.x, worldTransform.scale.y));
            }

            Color color;
            //Rectangle *rect;
            AxisAlignedBoundingBox *aabb;
            Material *material;
            const Player *player = nullptr;
            bool disabled = false;
            bool dirty = true;
        };

    public:
        ColorSelectionPanel(ComPtr<ID3D12Device> device, const MaterialTemplate &materialTemplate, Board *board, const Player &player1, const Player &player2)
            : m_board(board) {

            for (size_t index = 0u; index < COLORS.size(); ++index) {
                auto selection = new Selection(static_cast<Color>(index), 50, 50);

                auto transform = selection->QueryComponent<Transform>();
                transform->position.x = index * 75.0f;
                transform->parent = QueryComponent<Transform>();

                auto aabb = selection->QueryComponent<AxisAlignedBoundingBox>();
                aabb->transform = transform;

                selection->material = materialTemplate.Instantiate();

                selection->material->SetVec4FVariable("Color", COLORS[static_cast<int>(selection->color)]);

                //selection->rect->SetMaterial(selection->material);

                m_selections.push_back(std::unique_ptr<Selection>(selection));
            }

            m_selections[static_cast<int>(player1.GetColor())]->player = &player1;
            m_selections[static_cast<int>(player1.GetColor())]->disabled = true;

            m_selections[static_cast<int>(player2.GetColor())]->player = &player2;
            m_selections[static_cast<int>(player2.GetColor())]->disabled = true;

            m_currentSelection = m_selections[static_cast<int>(player1.GetColor())].get();
        }

        auto Update(void) -> void {
            for (auto &selection : m_selections) {
                if (selection->dirty) {
                    const Vec4F color = COLORS[static_cast<int>(selection->color)];

                    auto transform = selection->QueryComponent<Transform>();

                    if (selection->disabled) {
                        transform->scale = Vec3F(0.75f, 0.75f, 1.0f);

                        selection->material->SetVec4FVariable("Color", Vec4F(color.x, color.y, color.z, 0.75f));
                    } else {
                        transform->scale = Vec3F(1.0f, 1.0f, 1.0f);

                        selection->material->SetVec4FVariable("Color", Vec4F(color.x, color.y, color.z, 1.0f));
                    }

                    selection->Update();

                    selection->dirty = false;
                }
            }
        }

        auto Draw(D3D12Renderer &renderer, PipelineState &pso) -> void {
            for (auto &selection : m_selections) {
                //renderer.Draw(*selection->rect, pso.Get());
            }
        }

        auto OnClick(int32_t x, int32_t y, Player &player) -> void {
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

        auto GetCurrentColor(void) -> Color {
            return m_currentSelection->color;
        }

    private:
        Board *m_board;

        std::vector<std::unique_ptr<Selection>> m_selections;

        Selection *m_currentSelection = nullptr;
    };

    class Filler : public ApplicationMessageHandler {
        static constexpr auto BOARD_WIDTH = 10;
        static constexpr auto BOARD_HEIGHT = 10;

    public:
        Filler(ComPtr<ID3D12Device> device, const Vec2I &windowSize)
            : m_camera(Vec2F(0.0f, 0.0f), windowSize.x / static_cast<float>(windowSize.y), static_cast<float>(windowSize.x))
            , m_viewport(0.0f, 0.0f, windowSize.x, windowSize.y, 0.0f, 1.0f)
            , m_vs("C:\\Users\\jlyda\\source\\repos\\Lightning3D\\shaders\\Default.hlsl", "VSMain", "vs_5_0")
            , m_ps("C:\\Users\\jlyda\\source\\repos\\Lightning3D\\shaders\\Default.hlsl", "PSMain", "ps_5_0") {

            std::srand(std::time(nullptr));

            auto [player1Color, player2Color] = []() {
                Color c1; Color c2;
                do {
                    c1 = RandomColor();
                    c2 = RandomColor();
                } while (c1 == c2);
                return std::make_tuple(c1, c2);
            }();

            m_player1 = std::make_unique<Player>(player1Color, Vec2I(0, BOARD_HEIGHT - 1));
            m_player2 = std::make_unique<Player>(player2Color, Vec2I(BOARD_WIDTH - 1, 0));

            m_activePlayer = m_player1.get();

            m_rootSignature = GenerateRootSignature(device.Get());

            static auto materialTemplate = MaterialTemplate(m_vs, m_ps);

            auto psoBuilder = Lightning3D::PipelineStateBuilder();
            psoBuilder.SetRootSignature(m_rootSignature.Get());
            psoBuilder.SetVertexShader(m_vs.GetBytecode());
            psoBuilder.SetPixelShader(m_ps.GetBytecode());
            psoBuilder.SetInputLayout(materialTemplate.GetInputLayout());

            m_pso = psoBuilder.Finalize(device.Get());

            m_board = std::make_unique<Board>(BOARD_WIDTH, BOARD_HEIGHT, *m_player1.get(), *m_player2.get(), device, materialTemplate);
            m_colorSelectionPanel = std::make_unique<ColorSelectionPanel>(device, materialTemplate, m_board.get(), *m_player1.get(), *m_player2.get());

            m_viewport.SetCamera(&m_camera);

            auto boardTransform = m_board->QueryComponent<Transform>();
            boardTransform->position += Vec3F(262.0f, 134.0f, 0.0f);

            auto colorSelectionPanelTransform = m_colorSelectionPanel->QueryComponent<Transform>();
            colorSelectionPanelTransform->position = 287.0f;
            colorSelectionPanelTransform->position.y = 700.0f;

            std::cout << "Player 1's turn" << std::endl;
        }

        auto Draw(D3D12Renderer &renderer) -> void {
            m_colorSelectionPanel->Update();

            renderer.PreRender(m_rootSignature.Get(), &m_viewport);
            m_board->Draw(renderer, m_pso);
            m_colorSelectionPanel->Draw(renderer, m_pso);
            renderer.Render();
        }

    private:
        virtual auto OnMouseDown(MouseButton button, int32_t x, int32_t y, InputModifier modifiers) -> bool override {
            std::cout << x << ", " << y << std::endl;

            m_colorSelectionPanel->OnClick(x, y, *m_activePlayer);

            if (m_activePlayer == m_player1.get()) {
                m_activePlayer = m_player2.get();
            } else {
                m_activePlayer = m_player1.get();
            }

            if (m_activePlayer == m_player1.get()) {
                std::cout << "Player 1's turn" << std::endl;
            } else {
                std::cout << "Player 2's turn" << std::endl;
            }

            return true;
        }

    private:
        Camera m_camera;
        Viewport m_viewport;

        Shader m_vs;
        Shader m_ps;

        //SpriteFactory m_spriteFactory;

        std::unique_ptr<Player> m_player1;
        std::unique_ptr<Player> m_player2;

        Player *m_activePlayer = nullptr;

        std::unique_ptr<Board> m_board;
        std::unique_ptr<ColorSelectionPanel> m_colorSelectionPanel;

        Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
        PipelineState m_pso;
    };
}

#endif

//std::vector<Lightning3D::Rectangle> boxes;
//
//auto batchedDraw = Lightning3D::BatchedDraw(renderer.GetDevice(), 2u * 1024u * 1024u * 1024u);
//
//auto vs = Lightning3D::;
//auto ps = Lightning3D::Shader();
//
//auto material = Lightning3D::Material(vs, ps);
//
//material.SetMat4x4Variable("ViewProj", Lightning3D::OrthographicLH(768.0f, 768.0f, -1.0f, 1.0f).Transposition());
//
//const auto color = Vec4F(0.0f, 1.0f, 0.0f, 1.0f);
//material.SetVec4FVariable("Color", color);
//
//auto rootSignature = GenerateRootSignature(renderer.GetDevice().Get());
//
//auto psoBuilder = Lightning3D::PipelineStateBuilder();
//psoBuilder.SetRootSignature(rootSignature.Get());
//psoBuilder.SetVertexShader(vs.GetBytecode());
//psoBuilder.SetPixelShader(ps.GetBytecode());
//psoBuilder.SetInputLayout(material.GetInputLayout());
//
//auto pso = psoBuilder.Finalize(renderer.GetDevice().Get());