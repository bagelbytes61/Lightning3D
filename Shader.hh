#ifndef LIGHTNING3D_SHADER_HH_
#define LIGHTNING3D_SHADER_HH_

#include "ShaderVariable.hh"

#include <d3dcompiler.h>

namespace Lightning3D {
#define GET_N_COMPONENT_INPUT_ELEMENT_FORMAT(COMPONENTS, TYPE)                            \
    [](uint32_t type) -> DXGI_FORMAT {                                                    \
        switch (type) {                                                                   \
            case D3D_REGISTER_COMPONENT_UINT32:  return DXGI_FORMAT_##COMPONENTS##_UINT;  \
            case D3D_REGISTER_COMPONENT_SINT32:  return DXGI_FORMAT_##COMPONENTS##_SINT;  \
            case D3D_REGISTER_COMPONENT_FLOAT32: return DXGI_FORMAT_##COMPONENTS##_FLOAT; \
        }                                                                                 \
    }(TYPE)

    class ShaderReflection {
    public:
        ShaderReflection(const D3D12_SHADER_BYTECODE &bytecode) {
            HRESULT hr;

            Microsoft::WRL::ComPtr<ID3D12ShaderReflection> m_shaderReflection;

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

            Microsoft::WRL::ComPtr<ID3DBlob> errors;

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
        Microsoft::WRL::ComPtr<ID3DBlob> m_bytecode;
    };
}

#endif