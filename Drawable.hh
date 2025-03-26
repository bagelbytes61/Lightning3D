#ifndef LIGHTNING3D_DRAWABLE_HH_
#define LIGHTNING3D_DRAWABLE_HH_

#include "Matrix.hh"
#include "Vector.hh"
#include "Vertex.hh"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <ranges>
#include <span>
#include <unordered_map>
#include <vector>

#include <d3d12.h>
#include <d3dcompiler.h>
#include <d3d12shader.h>

#include <wrl/client.h>

using namespace Microsoft::WRL;

namespace Lightning3D {
#define GET_N_COMPONENT_INPUT_ELEMENT_FORMAT(COMPONENTS, TYPE)                            \
    [](uint32_t type) -> DXGI_FORMAT {                                                    \
        switch (type) {                                                                   \
            case D3D_REGISTER_COMPONENT_UINT32:  return DXGI_FORMAT_##COMPONENTS##_UINT;  \
            case D3D_REGISTER_COMPONENT_SINT32:  return DXGI_FORMAT_##COMPONENTS##_SINT;  \
            case D3D_REGISTER_COMPONENT_FLOAT32: return DXGI_FORMAT_##COMPONENTS##_FLOAT; \
        }                                                                                 \
    }(TYPE)


    enum struct ShaderVariableType : uint8_t {
        Unknown,

        ConstantBuffer,
        Mat4x4,
        Vec4F
    };

    class ShaderVariable {
    public:
        ShaderVariable(std::string name, ShaderVariableType type)
            : m_name(std::move(name))
            , m_type(type) {

        }

        //ShaderVariable(const ShaderVariable &) = delete;
        //ShaderVariable(ShaderVariable &&) = delete;

        virtual ~ShaderVariable() = default;

        //auto operator=(const ShaderVariable &)->ShaderVariable & = delete;
        //auto operator=(ShaderVariable &&)->ShaderVariable & = delete;

        auto GetName(void) const -> const std::string & {
            return m_name;
        }

        auto GetType(void) const -> ShaderVariableType {
            return m_type;
        }

    private:
        std::string m_name;

        ShaderVariableType m_type;
    };

    class ConstantBufferVariable : public ShaderVariable {
    public:
        ConstantBufferVariable(std::string name, void *storage, uint32_t size)
            : ShaderVariable(std::move(name), ShaderVariableType::Unknown)
            , m_storage(storage)
            , m_size(size) {

        }

        auto Set(const void *value) -> void {
            std::memcpy(m_storage, value, m_size);
        }

        auto Get(void) -> const void * {
            return m_storage;
        }

    private:
        void *m_storage;

        uint32_t m_size;
    };

    class ConstantBufferVariableTemplate {
    public:
        ConstantBufferVariableTemplate(std::string name, uint32_t size)
            : m_name(std::move(name))
            , m_size(size) {

        }

        auto Instantiate(void *storage, uint32_t &size) const -> ConstantBufferVariable {
            size = m_size;

            return { m_name, storage, m_size };
        }

    private:
        std::string m_name;

        uint32_t m_size;
    };

    class ConstantBuffer : public ShaderVariable {
    public:
        //ConstantBuffer() = default;

        ConstantBuffer(std::unordered_map<std::string, ConstantBufferVariable> variables, uint32_t size, std::unique_ptr<std::byte[]> buffer)
            : ShaderVariable("", ShaderVariableType::ConstantBuffer)
            , m_variables(std::move(variables))
            , m_size(size)
            , m_buffer(std::move(buffer)) {

        }

        auto SetVec4FVariable(const std::string &name, const Vec4F &value) -> bool {
            auto result = m_variables.find(name);
            if (result == m_variables.end()) {
                return false;
            }

            auto &var = result->second;

            var.Set(&value);

            return true;
        }

        auto GetVariables(void) -> std::unordered_map<std::string, ConstantBufferVariable> & {
            return m_variables;
        }

        auto Upload(void *mappedPtr) -> void {
            std::memcpy(mappedPtr, m_buffer.get(), m_size);
        }

    private:
        std::unordered_map<std::string, ConstantBufferVariable> m_variables;

        //uint32_t m_slot;
        uint32_t m_size;

        std::unique_ptr<std::byte[]> m_buffer;
    };

    class ConstantBufferTemplate {
    public:
        ConstantBufferTemplate(uint32_t slot, uint32_t size)
            //: ShaderVariable("", ShaderVariableType::ConstantBuffer)
            :
            m_slot(slot)
            , m_size(size) {

        }

        auto Instantiate(void) const -> ConstantBuffer * {
            auto storage = std::make_unique<std::byte[]>(m_size);

            std::unordered_map<std::string, ConstantBufferVariable> variables;

            uint32_t offset = 0u;

            for (const auto &[name, t] : m_variableTemplates) {
                uint32_t size = 0u;
                variables.emplace(std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple(t.Instantiate(storage.get() + offset, size)));

                offset += size;
            }

            return new ConstantBuffer(std::move(variables), m_size, std::move(storage));
        }

        auto AddMat4x4Variable(std::string name, uint32_t offset) -> bool {
            if (!m_variableTemplates.emplace(std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple(name, sizeof(Mat4x4))).second) {
                return false;
            }

            return true;
        }

        auto AddVec4FVariable(std::string name, uint32_t offset) -> bool {
            if (!m_variableTemplates.emplace(std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple(name, sizeof(Vec4F))).second) {
                return false;
            }

            return true;
        }

        auto AddVec3FVariable(std::string name, uint32_t offset) -> bool {
            if (!m_variableTemplates.emplace(std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple(name, sizeof(Vec3F))).second) {
                return false;
            }

            return true;
        }

    private:
        std::unordered_map<std::string, ConstantBufferVariableTemplate> m_variableTemplates;

        uint32_t m_slot;
        uint32_t m_size;
    };

    class ShaderReflection {
    public:
        ShaderReflection(const D3D12_SHADER_BYTECODE &bytecode) {
            HRESULT hr;

            ComPtr<ID3D12ShaderReflection> m_shaderReflection;

            hr = D3DReflect(bytecode.pShaderBytecode, bytecode.BytecodeLength, IID_PPV_ARGS(&m_shaderReflection));
            if (FAILED(hr)) {

            }

            D3D12_SHADER_DESC shaderDesc;
            hr = m_shaderReflection->GetDesc(&shaderDesc);
            if (FAILED(hr)) {

            }

            for (uint32_t index = 0u; index < shaderDesc.InputParameters; ++index) {
                D3D12_SIGNATURE_PARAMETER_DESC signatureParameterDesc;
                hr = m_shaderReflection->GetInputParameterDesc(index, &signatureParameterDesc);
                if (FAILED(hr)) {

                }

                D3D12_INPUT_ELEMENT_DESC desc = { };
                // fix this later
                desc.SemanticName = (new std::string(signatureParameterDesc.SemanticName))->c_str();
                desc.SemanticIndex = signatureParameterDesc.SemanticIndex;

                if (signatureParameterDesc.Mask & 8u) {
                    desc.Format = GET_N_COMPONENT_INPUT_ELEMENT_FORMAT(R32G32B32A32, signatureParameterDesc.ComponentType);
                } else if (signatureParameterDesc.Mask & 4u) {
                    desc.Format = GET_N_COMPONENT_INPUT_ELEMENT_FORMAT(R32G32B32, signatureParameterDesc.ComponentType);
                } else if (signatureParameterDesc.Mask & 2u) {
                    desc.Format = GET_N_COMPONENT_INPUT_ELEMENT_FORMAT(R32G32, signatureParameterDesc.ComponentType);
                } else if (signatureParameterDesc.Mask & 1u) {
                    desc.Format = GET_N_COMPONENT_INPUT_ELEMENT_FORMAT(R32, signatureParameterDesc.ComponentType);
                }

                desc.InputSlot = 0u;
                desc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
                desc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
                desc.InstanceDataStepRate = 0u;

                m_inputElements.push_back(desc);
            }

            for (uint32_t index = 0u; index < shaderDesc.BoundResources; ++index) {
                D3D12_SHADER_INPUT_BIND_DESC bindDesc = { };

                m_shaderReflection->GetResourceBindingDesc(index, &bindDesc);

                const auto cbuffer = m_shaderReflection->GetConstantBufferByIndex(index);

                D3D12_SHADER_BUFFER_DESC cbufferDesc;
                hr = cbuffer->GetDesc(&cbufferDesc);
                if (FAILED(hr)) {

                }

                auto result = m_cbTemplates.try_emplace(cbufferDesc.Name, bindDesc.BindPoint, cbufferDesc.Size);
                if (!result.second) {

                }

                auto &cbTemplate = result.first->second;

                for (uint32_t index = 0u; index < cbufferDesc.Variables; ++index) {
                    auto var = cbuffer->GetVariableByIndex(index);

                    D3D12_SHADER_VARIABLE_DESC varDesc;
                    hr = var->GetDesc(&varDesc);
                    if (FAILED(hr)) {

                    }
                    //var->GetInterfaceSlot();
                    const auto type = var->GetType();

                    D3D12_SHADER_TYPE_DESC typeDesc;
                    hr = type->GetDesc(&typeDesc);
                    if (FAILED(hr)) {

                    }

                    //typeDesc.Class
                    if (std::strcmp(typeDesc.Name, "float4") == 0) {
                        cbTemplate.AddVec4FVariable(varDesc.Name, varDesc.StartOffset);
                    } else if (std::strcmp(typeDesc.Name, "float4x4") == 0) {
                        cbTemplate.AddMat4x4Variable(varDesc.Name, varDesc.StartOffset);
                    } else if (std::strcmp(typeDesc.Name, "float3") == 0) {
                        cbTemplate.AddVec3FVariable(varDesc.Name, varDesc.StartOffset);
                    }

                }
            }
        }

        ShaderReflection(const ShaderReflection &) = delete;
        ShaderReflection(ShaderReflection &&) = delete;

        ~ShaderReflection() {

        }

        auto operator=(const ShaderReflection &)->ShaderReflection & = delete;
        auto operator=(ShaderReflection &&)->ShaderReflection & = delete;

        auto GetInputLayout(void) const ->  std::vector<D3D12_INPUT_ELEMENT_DESC> {
            return m_inputElements;
            //return
            //{
            //    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
            //    //{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
            //};
        }

        //auto GetVariable(const std::string &name) const -> ShaderVariable * {
        //    std::string token;

        //    for (size_t index = 0u; index < name.length();) {
        //        const auto delimiter_pos = name.find_first_of('.', index);
        //    
        //        const auto token = name.substr(index, delimiter_pos);

        //        auto result = m_variables.find(token);
        //    }

        //    auto result = m_variables.find(name);
        //    if (result == m_variables.end()) {
        //        return nullptr;
        //    }

        //    return result->second.get();
        //}

        auto GetVariables(void) const -> const std::unordered_map<std::string, ConstantBufferTemplate> & {
            return m_cbTemplates;
        }

    private:
        std::vector<D3D12_INPUT_ELEMENT_DESC> m_inputElements;

        std::unordered_map<std::string, ConstantBufferTemplate> m_cbTemplates;
    };

    class Shader {
    public:
        Shader(const std::filesystem::path &path, const std::string &entryPoint, const std::string &target) {
            std::vector<char> source;

            try {
                const auto size = std::filesystem::file_size(path);

                source.resize(size);

            } catch (const std::filesystem::filesystem_error &err) {
                throw err;
            }

            if (auto file = std::unique_ptr<FILE, decltype(&std::fclose)>(std::fopen(path.string().c_str(), "rb"), &std::fclose)) {
                if (std::fread(source.data(), 1u, source.size(), file.get()) != source.size()) {
                    if (std::feof(file.get())) {
                        throw std::runtime_error("Failed to read shader '" + path.string() + "': unexpected end-of-file");
                    } else if (std::ferror(file.get())) {
                        throw std::runtime_error("Failed to read shader '" + path.string() + "': stream error");
                    } else {
                        assert(false);
                    }
                }
            } else {
                throw std::runtime_error("Failed to open shader '" + path.string() + "': " + std::strerror(errno));
            }

            HRESULT result;

            ComPtr<ID3DBlob> errors;

            result = D3DCompile(source.data(), source.size(), nullptr, nullptr, nullptr, entryPoint.c_str(), target.c_str(), 0u, 0u, &m_bytecode, &errors);
            if (FAILED(result)) {
                throw std::runtime_error("Failed to compile shader: " + std::string(static_cast<const char *>(errors->GetBufferPointer()), errors->GetBufferSize()));
            }
        }

        auto GetBytecode(void) const -> D3D12_SHADER_BYTECODE {
            return {
                m_bytecode->GetBufferPointer(),
                m_bytecode->GetBufferSize()
            };
        }

    private:
        ComPtr<ID3DBlob> m_bytecode;
    };

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
        ComPtr<ID3D12PipelineState> m_pipelineState;

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

        static constexpr D3D12_DEPTH_STENCIL_DESC defaultDepthStencilDesc = {

        };

    public:
        PipelineStateBuilder() {
            m_desc.BlendState.RenderTarget[0] = defaultRTBlendDesc;
            m_desc.SampleMask = UINT_MAX;
            m_desc.RasterizerState = defaultRasterizerDesc;
            m_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
            m_desc.NumRenderTargets = 1U;
            m_desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
            m_desc.DSVFormat;
            m_desc.SampleDesc.Count = 1U;
            m_desc.SampleDesc.Quality = 0U;
            m_desc.SampleMask = 0xFFFFFFFF;
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

        auto Finalize(ID3D12Device *device) -> PipelineState {
            return PipelineState(device, m_desc);
        }

    private:
        D3D12_GRAPHICS_PIPELINE_STATE_DESC m_desc{};
    };

    class PipelineStateCache {

    };

    class Material {
    public:
        Material(std::vector<ConstantBuffer *> vsConstantBuffers, std::vector<ConstantBuffer *> psConstantBuffers)
            : m_vsConstantBuffers(std::move(vsConstantBuffers))
            , m_psConstantBuffers(std::move(psConstantBuffers)) {

            for (auto &cb : m_vsConstantBuffers) {
                for (auto &variable : cb->GetVariables()) {
                    m_shaderVariables.emplace(variable.first, &variable.second);
                }
            }

            for (auto &cb : m_psConstantBuffers) {
                for (auto &variable : cb->GetVariables()) {
                    m_shaderVariables.emplace(variable.first, &variable.second);
                }
            }

            assert(m_shaderVariables.find("LIGHTNING3D_MATRIX_WVP") != m_shaderVariables.cend());
        }

        Material(const Material &other)
            : Material(other.m_vsConstantBuffers, other.m_psConstantBuffers) {
            std::cout << "Material copy constructor" << std::endl;
        }

        auto SetMat4x4Variable(const std::string &name, const Mat4x4 &value) -> bool {
            return SetVariable(name, ShaderVariableType::Mat4x4, value);
        }

        auto SetVec4FVariable(const std::string &name, const Vec4F &value) -> bool {
            return SetVariable(name, ShaderVariableType::Vec4F, value);
        }

        auto SetRaw(const std::string &name, const void *value) -> bool {
            auto result = m_shaderVariables.find(name);
            if (result == m_shaderVariables.end()) {
                return false;
            }

            //if (result->second->GetType() != type) {
            //    return false;
            //}

            auto var = static_cast<ConstantBufferVariable *>(result->second);

            var->Set(value);

            return true;
        }

        //auto SetPSVariable(const std::string &name, const void *value) -> bool {
        //    auto var = m_pixelShaderReflection.GetVariable(name);
        //    if (var == nullptr) {
        //        return false;
        //    }

        //    //switch (var->GetType()) {
        //    //    case 
        //    //}

        //    return true;
        //}

        //auto GetInputLayout(void) const -> D3D12_INPUT_LAYOUT_DESC {
        //    return m_vertexShaderReflection.GetInputLayout();
        //}

        auto GetVSConstantBuffers(void) -> std::span<ConstantBuffer *> {
            return m_vsConstantBuffers;
        }

        auto GetPSConstantBuffers(void) -> std::span<ConstantBuffer *> {
            return m_psConstantBuffers;
        }

    private:
        template<typename T>
        auto SetVariable(const std::string &name, ShaderVariableType type, const T &value) -> bool {
            auto result = m_shaderVariables.find(name);
            if (result == m_shaderVariables.end()) {
                return false;
            }

            //if (result->second->GetType() != type) {
            //    return false;
            //}

            auto var = static_cast<ConstantBufferVariable *>(result->second);

            var->Set(&value);

            return true;
        }
    private:
        std::vector<ConstantBuffer *> m_vsConstantBuffers;
        std::vector<ConstantBuffer *> m_psConstantBuffers;

        std::unordered_map<std::string, ShaderVariable *> m_shaderVariables;
    };

    class MaterialTemplate {
    public:
        MaterialTemplate(const Shader &vertexShader, const Shader &pixelShader)
            : m_vertexShaderReflection(vertexShader.GetBytecode())
            , m_pixelShaderReflection(pixelShader.GetBytecode()) {

            m_inputElements = m_vertexShaderReflection.GetInputLayout();
        }

        auto Instantiate(void) const -> Material * {
            //std::unordered_map<std::string, ShaderVariable> shaderVariables;

            std::vector<ConstantBuffer *> vsConstantBuffers;
            std::vector<ConstantBuffer *> psConstantBuffers;

            for (const auto &variableTemplate : m_vertexShaderReflection.GetVariables()) {
                //if (var.second.GetType() == ShaderVariableType::ConstantBuffer) {
                vsConstantBuffers.push_back(variableTemplate.second.Instantiate());

                //for (const auto &cbufferVar : var.second.)) {
                //    m_shaderVariables.emplace(cbufferVar.first, static_cast<ShaderVariable *>(&cbufferVar.second));
                //}
                //}
            }

            for (const auto &variableTemplate : m_pixelShaderReflection.GetVariables()) {
                //if (var.second->GetType() == ShaderVariableType::ConstantBuffer) {
                psConstantBuffers.push_back(variableTemplate.second.Instantiate());

                //for (auto &cbufferVar : static_cast<ConstantBuffer *>(var.second.get())->GetVariables()) {
                //    m_shaderVariables.emplace(cbufferVar.first, &cbufferVar.second);
                //}
            //}
            }

            return new Material{ std::move(vsConstantBuffers), std::move(psConstantBuffers) };
        }

        auto GetInputLayout(void) const -> D3D12_INPUT_LAYOUT_DESC {
            return { m_inputElements.data(), static_cast<UINT>(m_inputElements.size()) };
        }


    private:
        ShaderReflection m_vertexShaderReflection;
        ShaderReflection m_pixelShaderReflection;

        std::vector<D3D12_INPUT_ELEMENT_DESC> m_inputElements;
    };

    class MaterialBuilder {
    public:
    };

    class MaterialLoader {
    public:

        auto Load(const std::filesystem::path &path) -> MaterialTemplate {
            //HRESULT result;

            //ComPtr<ID3DBlob> errors;

            //ComPtr<ID3DBlob> vsByteCode;
            //ComPtr<ID3DBlob> psByteCode;

            //result = D3DCompile(source.data(), source.size(), nullptr, nullptr, nullptr, "VSMain", "vs_5_0", 0u, 0u, &vsByteCode, &errors);
            //if (FAILED(result)) {
            //    throw std::runtime_error("Failed to compile Material: " + std::string(static_cast<const char *>(errors->GetBufferPointer()), errors->GetBufferSize()));
            //}

            //result = D3DCompile(source.data(), source.size(), nullptr, nullptr, nullptr, "PSMain", "ps_5_0", 0u, 0u, &psByteCode, &errors);
            //if (FAILED(result)) {
            //    throw std::runtime_error("Failed to compile Material: " + std::string(static_cast<const char *>(errors->GetBufferPointer()), errors->GetBufferSize()));
            //}

            // Eventually cache these?

            return MaterialTemplate(Shader(path, "VSMain", "vs_5_0"), Shader(path, "PSMain", "ps_5_0"));
        }

    private:
    };

    class MaterialCache {
    public:

    private:
    };

    class Drawable {
    public:
        virtual ~Drawable() = default;

        //virtual auto GetPipelineState(void) const->PipelineState;

        //virtual auto GetVertexBuffer(void) const -> const D3D12_VERTEX_BUFFER_VIEW & = 0;

        virtual auto GetVertices(void) const -> const std::vector<Vertex> & = 0;

        virtual auto GetVertexCount(void) const -> uint32_t = 0;

        virtual auto GetWorldTransform(void) const -> Mat4x4 = 0;

        virtual auto GetPosition(void) const -> Vec2F = 0;

        virtual auto GetScale(void) const -> Vec2F = 0;

        auto SetMaterial(Material *material) -> void {
            m_material = material;
        }

        auto GetMaterial(void) -> Material * {
            return m_material;
        }

    private:
        Material *m_material{ nullptr };
    };
}

#endif