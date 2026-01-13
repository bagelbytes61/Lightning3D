#include "Filler.hh"

#include "../../D3D12Renderer.hh"
#include "../../Material.hh"
#include "../../PipelineState.hh"

#include "Board.hh"
#include "ColorSelectionPanel.hh"
#include "Cell.hh"

using namespace Lightning3D::Games::Filler;

static auto GenerateRootSignature(ID3D12Device *device) -> Microsoft::WRL::ComPtr<ID3D12RootSignature> {
    HRESULT hr;

    D3D12_ROOT_DESCRIPTOR globalConstants;
    globalConstants.RegisterSpace = 0u;
    globalConstants.ShaderRegister = 0u;

    D3D12_ROOT_PARAMETER rootParam0 = { };
    rootParam0.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
    rootParam0.Descriptor = globalConstants;
    rootParam0.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

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

    Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;

    hr = device->CreateRootSignature(0U, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create Root Signature: " + std::to_string(hr));
    }

    return rootSignature;
}

Filler::Filler(Microsoft::WRL::ComPtr<ID3D12Device> device, const Vec2I &windowSize) {
    std::srand(std::time(nullptr));

    m_camera = std::make_unique<Camera>(Vec2F(0.0f, 0.0f), windowSize.x / static_cast<float>(windowSize.y), static_cast<float>(windowSize.x));

    m_viewport = std::make_unique<Viewport>(0.0f, 0.0f, windowSize.x, windowSize.y, 0.0f, 1.0f);

    m_vs = std::make_unique<Shader>("C:\\Users\\jlyda\\source\\repos\\Lightning3D\\shaders\\Default.hlsl", "VSMain", "vs_5_0");
    m_ps = std::make_unique<Shader>("C:\\Users\\jlyda\\source\\repos\\Lightning3D\\shaders\\Default.hlsl", "PSMain", "ps_5_0");


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

    static auto materialTemplate = MaterialTemplate(*m_vs.get(), *m_ps.get());

    auto psoBuilder = Lightning3D::PipelineStateBuilder();
    psoBuilder.SetRootSignature(m_rootSignature.Get());
    psoBuilder.SetVertexShader(m_vs->GetBytecode());
    psoBuilder.SetPixelShader(m_ps->GetBytecode());
    psoBuilder.SetInputLayout(materialTemplate.GetInputLayout());

    m_pso = psoBuilder.Finalize(device.Get());

    m_board = std::make_unique<Board>(BOARD_WIDTH, BOARD_HEIGHT, *m_player1.get(), *m_player2.get(), device, materialTemplate);
    m_colorSelectionPanel = std::make_unique<ColorSelectionPanel>(device, materialTemplate, m_board.get(), *m_player1.get(), *m_player2.get());

    m_viewport->SetCamera(m_camera.get());

    auto boardTransform = m_board->QueryComponent<Transform>();
    boardTransform->position += Vec3F(262.0f, 134.0f, 0.0f);

    auto colorSelectionPanelTransform = m_colorSelectionPanel->QueryComponent<Transform>();
    colorSelectionPanelTransform->position = 287.0f;
    colorSelectionPanelTransform->position.y = 700.0f;

    std::cout << "Player 1's turn" << std::endl;
}

Filler::~Filler() {

}

auto Filler::Draw(D3D12Renderer &renderer) -> void {
    m_colorSelectionPanel->Update();

    renderer.PreRender(m_rootSignature.Get(), m_viewport.get());
    m_board->Draw(renderer, *m_pso.get());
    m_colorSelectionPanel->Draw(renderer, *m_pso.get());
    renderer.Render();
}

auto Filler::OnMouseDown(MouseButton button, int32_t x, int32_t y, InputModifier modifiers) -> bool {
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