#ifndef LIGHTNING3D_RESOURCE_VIEW_HH_
#define LIGHTNING3D_RESOURCE_VIEW_HH_

#include <cstdint>

#include <d3d12.h>

#include <wrl/client.h>

namespace Lightning3D {
    class ResourceView {
    public:
        ResourceView()
            : m_resourcePool(nullptr) {

        }

        ResourceView(Microsoft::WRL::ComPtr<ID3D12Device> device, Microsoft::WRL::ComPtr<ID3D12Resource> resourcePool, D3D12_GPU_VIRTUAL_ADDRESS offsetFromHeapStart, uint32_t size)
            : m_resourcePool(resourcePool)
            , m_offsetFromHeapStart(offsetFromHeapStart)
            , m_size(size) {

        }

        auto Map(void) -> void * {
            if (m_resourcePool == nullptr) {
                return nullptr;
            }

            void *mappedPtr = nullptr;
            m_resourcePool->Map(0u, nullptr, &mappedPtr);

            return static_cast<char *>(mappedPtr) + m_offsetFromHeapStart;
        }

        auto Unmap(void) -> void {
            m_resourcePool->Unmap(0u, nullptr);
        }

        auto GetGPUVirtualAddress(void) const -> D3D12_GPU_VIRTUAL_ADDRESS {
            return m_resourcePool->GetGPUVirtualAddress() + m_offsetFromHeapStart;
        }

    protected:
        Microsoft::WRL::ComPtr<ID3D12Resource> m_resourcePool;

        D3D12_GPU_VIRTUAL_ADDRESS m_offsetFromHeapStart = 0u;

        uint32_t m_size = 0u;
    };
}

#endif