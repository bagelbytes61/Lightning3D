#ifndef LIGHTNING3D_BOX_HH_
#define LIGHTNING3D_BOX_HH_

#include <array>
#include <stdexcept>
#include <string>

#include <d3d12.h>

#include <wrl/client.h>

namespace Lightning3D {

    class Box {
        static constexpr std::array<float, 9U> kVertices = {
            -100.0f, -100.0f, 0.0f,
            0.0f, 100.0f, 0.0f,
            100.0f, -100.0f, 0.0f
        };

    public:
        Box(Microsoft::WRL::ComPtr<ID3D12Device> device) {
            HRESULT result;

            D3D12_HEAP_PROPERTIES heapProperties = { };
            heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;

            D3D12_RESOURCE_DESC vertexBufferDesc = { };
            vertexBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
            vertexBufferDesc.Alignment = 0u;
            vertexBufferDesc.Width = 36u;
            vertexBufferDesc.Height = 1u;
            vertexBufferDesc.DepthOrArraySize = 1u;
            vertexBufferDesc.MipLevels = 1u;
            vertexBufferDesc.Format = DXGI_FORMAT_UNKNOWN;
            vertexBufferDesc.SampleDesc.Count = 1u;
            vertexBufferDesc.SampleDesc.Quality = 0u;
            vertexBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
            vertexBufferDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

            result = device->CreateCommittedResource(
                &heapProperties,
                D3D12_HEAP_FLAG_NONE,
                &vertexBufferDesc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&m_vertexBuffer));
            if (FAILED(result)) {
                throw std::runtime_error("Failed to create Vertex Buffer Resource: " + std::to_string(result));
            }

            D3D12_RANGE readRange = { };
            void *mappedVertexBufferPtr = nullptr;

            result = m_vertexBuffer->Map(0U, &readRange, &mappedVertexBufferPtr);
            if (FAILED(result)) {
                throw std::runtime_error(std::to_string(result));
            }
            memcpy(mappedVertexBufferPtr, kVertices.data(), 36u);
            m_vertexBuffer->Unmap(0u, nullptr);

            m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
            m_vertexBufferView.SizeInBytes = 36u;
            m_vertexBufferView.StrideInBytes = 12u;
        

        }

        ~Box() {

        }

        auto Draw(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList) -> void {
            commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            commandList->IASetVertexBuffers(0U, 1u, &m_vertexBufferView);
            commandList->DrawInstanced(6u, 1u, 0U, 0u);
        }

    private:
        Microsoft::WRL::ComPtr<ID3D12Resource> m_vertexBuffer;

        D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

        Microsoft::WRL::ComPtr<ID3D12Resource> m_constantBuffer;
    };

} // namespace Lightning3D

#endif