#ifndef LIGHTNING3D_RESOURCE_POOL_HH_
#define LIGHTNING3D_RESOURCE_POOL_HH_

#include <cstdint>
#include <vector>

namespace Lightning3D {
    template <typename ResourceType>
    class ResourcePool {
    public:
        ResourcePool(uint32_t numResources)
            : m_numResources(numResources)
            , m_resources(numResources) {

        }

        auto Request(void) -> ResourceType {
            assert(m_headIndex - m_tailIndex != m_numResources);

            auto index = m_headIndex++;

            return m_resources[index % m_numResources];
        }

        auto Release(ResourceType resource) -> void {
            assert(m_headIndex != m_tailIndex);

            m_resources[m_tailIndex++ % m_numResources] = resource;
        }

    protected:
        uint32_t m_numResources;

        uint64_t m_headIndex{ 0u };
        uint64_t m_tailIndex{ 0u };

        std::vector<ResourceType> m_resources;
    };
}

#endif