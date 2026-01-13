#ifndef LIGHTNING3D_SHADER_VARIABLE_HH_
#define LIGHTNING3D_SHADER_VARIABLE_HH_

namespace Lightning3D {
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

        virtual ~ShaderVariable() = default;

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
            : m_slot(slot)
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
}

#endif