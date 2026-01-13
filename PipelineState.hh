#ifndef LIGHTNING3D_PIPELINE_STATE_HH_
#define LIGHTNING3D_PIPELINE_STATE_HH_

#include <cstddef>
#include <cstdint>
#include <memory>
#include <span>

#include <d3d12.h>

#include <wrl/client.h>

namespace Lightning3D {
    inline auto Fnv1(const void *buf, size_t len) -> uint64_t {
        uint64_t hash = 14695981039346656037ui64;

        for (size_t index = 0u; index < len; ++index) {
            hash *= 1099511628211ui64;
            hash ^= static_cast<const unsigned char *>(buf)[index];
        }

        return hash;
    }

    struct PipelineStateDesc {
        auto Hash(void) const -> uint64_t {
            uint64_t hash = 0ui64;

            hash ^= Fnv1(vsBytecode.data(), vsBytecode.size());
            hash ^= Fnv1(psBytecode.data(), psBytecode.size());

            return hash;
        }

        std::span<std::byte> vsBytecode;
        std::span<std::byte> psBytecode;
    };

    class PipelineState {
    public:
        PipelineState() = default;

        PipelineState(ID3D12Device *device, const D3D12_GRAPHICS_PIPELINE_STATE_DESC &desc) {
            HRESULT result;

            result = device->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&m_pipelineState));
            if (FAILED(result)) {
                throw std::runtime_error("Failed to create Pipeline State: " + std::to_string(result));
            }

            m_hash ^= Fnv1(desc.VS.pShaderBytecode, desc.VS.BytecodeLength);
            m_hash ^= Fnv1(desc.PS.pShaderBytecode, desc.PS.BytecodeLength);
        }

        auto Get(void) const -> ID3D12PipelineState * {
            return m_pipelineState.Get();
        }

        auto GetHash(void) -> const uint64_t {
            return m_hash;
        }

    private:
        Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;

        uint64_t m_hash{ 0ui64 };
    };


    class PipelineStateBuilder {
        static constexpr D3D12_RENDER_TARGET_BLEND_DESC defaultRTBlendDesc = {
            .BlendEnable = TRUE,
            .LogicOpEnable = FALSE,

            .SrcBlend = D3D12_BLEND_SRC_ALPHA,
            .DestBlend = D3D12_BLEND_INV_SRC_ALPHA,
            .BlendOp = D3D12_BLEND_OP_ADD,

            .SrcBlendAlpha = D3D12_BLEND_ONE,
            .DestBlendAlpha = D3D12_BLEND_ZERO,
            .BlendOpAlpha = D3D12_BLEND_OP_ADD,

            .LogicOp = D3D12_LOGIC_OP_NOOP,
            .RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL
        };

        static constexpr D3D12_RASTERIZER_DESC defaultRasterizerDesc = {
            .FillMode = D3D12_FILL_MODE_SOLID,
            .CullMode = D3D12_CULL_MODE_NONE,
            .DepthClipEnable = true,
            .ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF
        };

        static constexpr D3D12_DEPTH_STENCILOP_DESC defaultDepthStenclOpDesc = {
            .StencilFailOp = D3D12_STENCIL_OP_KEEP,
            .StencilDepthFailOp = D3D12_STENCIL_OP_KEEP,
            .StencilPassOp = D3D12_STENCIL_OP_KEEP,
            .StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS
        };

        static constexpr D3D12_DEPTH_STENCIL_DESC defaultDepthStencilDesc = {
            .DepthEnable = true,
            .DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL,
            .DepthFunc = D3D12_COMPARISON_FUNC_LESS,
            .StencilEnable = false,
            .StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK,
            .StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK,
            .FrontFace = defaultDepthStenclOpDesc,
            .BackFace = defaultDepthStenclOpDesc
        };

    public:
        PipelineStateBuilder() {
            m_desc.BlendState.RenderTarget[0] = defaultRTBlendDesc;
            m_desc.SampleMask = UINT_MAX;
            m_desc.RasterizerState = defaultRasterizerDesc;
            m_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
            m_desc.NumRenderTargets = 1U;
            m_desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
            m_desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
            m_desc.SampleDesc.Count = 1U;
            m_desc.SampleDesc.Quality = 0U;
            m_desc.SampleMask = 0xFFFFFFFF;
            m_desc.DepthStencilState = defaultDepthStencilDesc;
        }

        auto SetRootSignature(ID3D12RootSignature *rootSignature) -> void {
            m_desc.pRootSignature = rootSignature;
        }

        auto SetVertexShader(D3D12_SHADER_BYTECODE bytecode) -> void {
            m_desc.VS = bytecode;
        }

        auto SetPixelShader(D3D12_SHADER_BYTECODE bytecode) -> void {
            m_desc.PS = bytecode;
        }

        auto SetInputLayout(const D3D12_INPUT_LAYOUT_DESC &inputLayout) -> void {
            m_desc.InputLayout = inputLayout;
        }

        auto Finalize(ID3D12Device *device) -> std::unique_ptr<PipelineState> {
            return std::make_unique<PipelineState>(device, m_desc);
        }

    private:
        D3D12_GRAPHICS_PIPELINE_STATE_DESC m_desc{};
    };

    class PipelineStateCache {

    };
}

#endif