#ifndef LIGHTNING3D_MATERIAL_HH_
#define LIGHTNING3D_MATERIAL_HH_

#include "Shader.hh"

#include <cassert>
#include <filesystem>
#include <unordered_map>
#include <vector>

namespace Lightning3D {
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

            // TODO: fix this
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

            // TODO: fix this
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
            std::vector<ConstantBuffer *> vsConstantBuffers;
            std::vector<ConstantBuffer *> psConstantBuffers;

            for (const auto &variableTemplate : m_vertexShaderReflection.GetVariables()) {
                vsConstantBuffers.push_back(variableTemplate.second.Instantiate());
            }

            for (const auto &variableTemplate : m_pixelShaderReflection.GetVariables()) {
                psConstantBuffers.push_back(variableTemplate.second.Instantiate());
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

            return MaterialTemplate(Shader(path, "VSMain", "vs_5_0"), Shader(path, "PSMain", "ps_5_0"));
        }

    private:
    };

    class MaterialCache {
    public:

    private:
    };
}

#endif