#ifndef LIGHTNING3D_D3D12_RENDERER_HH_
#define LIGHTNING3D_D3D12_RENDERER_HH_

#include "Drawable.hh"
#include "Sprite.hh"
#include "Material.hh"
#include "Matrix.hh"
#include "ResourcePool.hh" 
#include "ResourceView.hh"
#include "Vertex.hh"
#include "Viewport.hh"

#include <array>
#include <cassert>
#include <cstdint>
#include <memory>
#include <span>
#include <stdexcept>
#include <string>
#include <vector>

#include <d3d12.h>
#include <dxgi1_4.h>

#include <wrl/client.h>

namespace Lightning3D {
    class Drawable;
    class WindowsWindow;

    struct GPUDescriptorRegion {
        GPUDescriptorRegion() = default;

        GPUDescriptorRegion(D3D12_GPU_DESCRIPTOR_HANDLE baseDescriptorHandle, uint32_t numDescriptors, uint32_t descriptorHandleIncrementSize)
            : size(numDescriptors *descriptorHandleIncrementSize)
            , begin(baseDescriptorHandle)
            , end({ begin.ptr + size }) {

        }

        uint64_t size;

        D3D12_GPU_DESCRIPTOR_HANDLE begin;
        D3D12_GPU_DESCRIPTOR_HANDLE end;
    };

    class GPUDescriptorPool : public ResourcePool<D3D12_GPU_DESCRIPTOR_HANDLE> {
    public:
        GPUDescriptorPool(ID3D12Device *device, uint32_t numDescriptors)
            : ResourcePool(numDescriptors)
            , m_descriptorHandleIncrementSize(device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)) {

            assert(m_numResources >= 1u);

            D3D12_DESCRIPTOR_HEAP_DESC desc = { };
            desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
            desc.NumDescriptors = m_numResources;
            desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
            desc.NodeMask = 0u;

            device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_heap));

            const auto baseDescriptorHandle = m_heap->GetGPUDescriptorHandleForHeapStart();

            m_descriptorRegion = GPUDescriptorRegion(baseDescriptorHandle, m_numResources, m_descriptorHandleIncrementSize);

            for (size_t index = 0u; index < m_numResources; ++index) {
                m_resources[index].ptr = baseDescriptorHandle.ptr + index * m_descriptorHandleIncrementSize;
            }
        }

    private:
        uint32_t m_descriptorHandleIncrementSize;

        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_heap;

        GPUDescriptorRegion m_descriptorRegion;

        uint64_t m_headDescriptorHandleIndex{ 0u };
        uint64_t m_tailDescriptorHandleIndex{ 0u };

        std::vector<D3D12_GPU_DESCRIPTOR_HANDLE> m_descriptorHandles;
    };

    class ConstantBufferView : public ResourceView {
    public:
        ConstantBufferView() = default;

        ConstantBufferView(Microsoft::WRL::ComPtr<ID3D12Device> device, Microsoft::WRL::ComPtr<ID3D12Resource> cbvPool, D3D12_GPU_VIRTUAL_ADDRESS offsetFromHeapStart, uint32_t size, D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle)
            : ResourceView(device, cbvPool, offsetFromHeapStart, size) {

            D3D12_CONSTANT_BUFFER_VIEW_DESC desc = { };
            desc.BufferLocation = m_resourcePool->GetGPUVirtualAddress() + m_offsetFromHeapStart;
            desc.SizeInBytes = m_size;

            device->CreateConstantBufferView(&desc, descriptorHandle);
        }

    private:
    };

    class VertexBufferView : public ResourceView {
    public:
        VertexBufferView() = default;

        VertexBufferView(Microsoft::WRL::ComPtr<ID3D12Device> device, Microsoft::WRL::ComPtr<ID3D12Resource> vbPool, D3D12_GPU_VIRTUAL_ADDRESS offsetFromHeapStart, uint32_t size, uint32_t stride)
            : ResourceView(device, vbPool, offsetFromHeapStart, size)
            , m_stride(stride) {

        }

        auto GetView(void) const -> D3D12_VERTEX_BUFFER_VIEW {
            D3D12_VERTEX_BUFFER_VIEW view = { };
            view.BufferLocation = m_resourcePool->GetGPUVirtualAddress() + m_offsetFromHeapStart;
            view.SizeInBytes = m_size;
            view.StrideInBytes = m_stride;

            return view;
        }

    private:
        uint32_t m_stride;
    };

    template <typename ResourceViewType>
    class BufferViewPool : public ResourcePool<ResourceViewType> {
    public:
        BufferViewPool(Microsoft::WRL::ComPtr<ID3D12Device> device, uint32_t numBuffers, uint32_t bufferSize)
            : ResourcePool<ResourceViewType>(numBuffers) {

            HRESULT result;

            D3D12_HEAP_PROPERTIES heapProps = { };
            heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;

            D3D12_RESOURCE_DESC cbDesc = { };
            cbDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
            cbDesc.Alignment = 0u;
            cbDesc.Width = numBuffers * bufferSize;
            cbDesc.Height = 1u;
            cbDesc.DepthOrArraySize = 1u;
            cbDesc.MipLevels = 1u;
            cbDesc.Format = DXGI_FORMAT_UNKNOWN;
            cbDesc.SampleDesc.Count = 1u;
            cbDesc.SampleDesc.Quality = 0u;
            cbDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
            cbDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

            result = device->CreateCommittedResource(
                &heapProps,
                D3D12_HEAP_FLAG_NONE,
                &cbDesc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&m_resource));

            if (FAILED(result)) {
                throw std::runtime_error("Failed to create Buffer Resource: " + std::to_string(result));
            }
        }

    protected:
        Microsoft::WRL::ComPtr<ID3D12Resource> m_resource;
    };

    class ConstantBufferViewPool : public BufferViewPool<ConstantBufferView> {
    public:
        ConstantBufferViewPool(Microsoft::WRL::ComPtr<ID3D12Device> device, uint32_t numBuffers, uint32_t bufferSize)
            : BufferViewPool(device, numBuffers, (bufferSize + 0xff) & ~0xff) {
            HRESULT result;

            D3D12_DESCRIPTOR_HEAP_DESC heapDesc = { };
            heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
            heapDesc.NumDescriptors = m_numResources;
            heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

            result = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_descriptorHeap));
            if (FAILED(result)) {
                throw std::runtime_error("Failed to create CBV Descriptor Heap: " + std::to_string(result));
            }

            const auto descriptorHandleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

            for (size_t index = 0u; index < m_numResources; ++index) {
                m_resources[index] = ConstantBufferView(device, m_resource, index * ((bufferSize + 0xff) & ~0xff), (bufferSize + 0xff) & ~0xff, { m_descriptorHeap->GetCPUDescriptorHandleForHeapStart().ptr + index * descriptorHandleIncrementSize });
            }
        }

        auto GetDescriptorHeap(void) -> ID3D12DescriptorHeap * {
            return m_descriptorHeap.Get();
        }

    private:
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_descriptorHeap;
    };

    class VertexBufferViewPool : public BufferViewPool<VertexBufferView> {
    public:
        VertexBufferViewPool(Microsoft::WRL::ComPtr<ID3D12Device> device, uint32_t numBuffers, uint32_t bufferSize, uint32_t bufferStride)
            : BufferViewPool(device, numBuffers, bufferSize) {

            for (size_t index = 0u; index < m_numResources; ++index) {
                m_resources[index] = VertexBufferView(device, m_resource, index * bufferSize, bufferSize, bufferStride);
            }
        }

    private:
    };

    class BatchedDraw;

    class D3D12Renderer {
    public:
        D3D12Renderer(const WindowsWindow *window);

        ~D3D12Renderer();

        auto PreRender(ID3D12RootSignature *rootSignature, Viewport *viewport) -> void {
            m_commandAllocator->Reset();

            m_graphicsCommandList->Reset(m_commandAllocator.Get(), m_pipelineState.Get());

            m_graphicsCommandList->SetGraphicsRootSignature(rootSignature);

            D3D12_RESOURCE_BARRIER barrier;
            barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
            barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
            barrier.Transition.pResource = m_renderTargets[m_frameIndex].Get();
            barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
            barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
            barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
            m_graphicsCommandList->ResourceBarrier(1u, &barrier);

            auto rtvDescriptor = m_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
            rtvDescriptor.ptr += m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV) * m_frameIndex;

            auto dsDescriptor = m_dsDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

            m_graphicsCommandList->OMSetRenderTargets(1U, &rtvDescriptor, false, &dsDescriptor);

            const float clearColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
            //const float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };

            m_graphicsCommandList->ClearRenderTargetView(rtvDescriptor, clearColor, 0u, nullptr);

            m_graphicsCommandList->ClearDepthStencilView(m_dsDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0u, 0u, nullptr);

            m_viewport = viewport;

            m_viewport->PreRender(m_graphicsCommandList.Get());


        }

        auto Render(void) -> void;

        auto Draw(const std::vector<Vertex> &vertices, const Mat4x4 &worldTransform, Material *material, ID3D12PipelineState *pipelineState) -> void;

        auto GetDevice(void) const -> Microsoft::WRL::ComPtr<ID3D12Device> {
            return m_device;
        }

        auto GetCommandList(void) const -> Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> {
            return m_graphicsCommandList;
        }

        auto GetRootSignature(void) const ->  Microsoft::WRL::ComPtr<ID3D12RootSignature> {
            return m_rootSignature;
        }

    public:
        auto WaitForPreviousFrame(void) -> void {
            const auto fenceValue = m_fenceValue;
            m_commandQueue->Signal(m_fence.Get(), fenceValue);
            m_fenceValue += 1u;

            if (m_fence->GetCompletedValue() < fenceValue) {
                m_fence->SetEventOnCompletion(fenceValue, m_fenceEvent);
                WaitForSingleObject(m_fenceEvent, INFINITE);
            }

            m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
        }

    private:
        Microsoft::WRL::ComPtr<ID3D12Debug> m_debugController;
        Microsoft::WRL::ComPtr<ID3D12Device> m_device;
        Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_commandQueue;
        Microsoft::WRL::ComPtr<IDXGISwapChain3> m_swapChain;
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvDescriptorHeap;
        Microsoft::WRL::ComPtr<ID3D12Resource> m_renderTargets[2u];
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_cbDescriptorHeap;
        Microsoft::WRL::ComPtr<ID3D12Resource> m_cbs[2u];
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_dsDescriptorHeap;
        Microsoft::WRL::ComPtr<ID3D12Resource> m_depthStencilBuffer;
        Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_commandAllocator;
        Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
        Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;
        Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_graphicsCommandList;
        Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;

        HANDLE m_fenceEvent;
        UINT64 m_fenceValue{ 0U };
        UINT32 m_frameIndex{ 0U };

        std::unique_ptr<GPUDescriptorPool> m_descriptorPool{ nullptr };
        std::unique_ptr<ConstantBufferViewPool> m_cbvPool{ nullptr };
        std::unique_ptr<VertexBufferViewPool> m_vbvPool{ nullptr };

        std::array<std::vector<ConstantBufferView>, 2u> m_inflightCBVs;
        std::array<std::vector<VertexBufferView>, 2u> m_inflightVBVs;

        Viewport *m_viewport = nullptr;
    };

    class BatchedDraw : public Drawable {
    public:
        BatchedDraw(Microsoft::WRL::ComPtr<ID3D12Device> device, uint32_t vbSizeHint)
            : m_device(device) {

            m_vertices.reserve(vbSizeHint / sizeof(Vertex));
        }

        ~BatchedDraw() {

        }

        auto Draw(const std::span<const Vertex> &vertices) -> void {
            m_vertices.insert(m_vertices.end(), vertices.begin(), vertices.end());
        }

        //auto Draw(const Sprite &sprite) -> void {   
        //    //auto vertices = sprite.GetVertices();

        //    //const auto &position = sprite.GetPosition();
        //    ////const auto world = Translation(position.x, position.y, 0.0f);
        //    //const auto world = DirectX::XMMatrixTranslation(position.x, position.y, 0.0f);

        //    //for (auto &vertex : vertices) {
        //    //    //const auto wvp = world * LookAtLH({ 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }) * OrthographicLH(768.0f, 768.0f, 0.1f, 1000.0f);

        //    //    //const auto newPosition = Vec4F(vertex.position.x, vertex.position.y, 0.0f, 1.0f) * world;
        //    //    DirectX::XMFLOAT4 oldPos = { vertex.position.x, vertex.position.y, vertex.position.z, 1.0f };

        //    //    const auto newPos = DirectX::XMVector4Transform(DirectX::XMLoadFloat4(&oldPos), world);
        //    //    DirectX::XMFLOAT4 newPosition;
        //    //    DirectX::XMStoreFloat4(&newPosition, newPos);
        //    //    vertex.position = { newPosition.x, newPosition.y, newPosition.z };
        //    //}

        //    //BatchedDraw::Draw({ vertices.cbegin(), vertices.size() });
        //}

        auto Finish(void) -> void {
            //HRESULT result;

            //void *mappedPtr = nullptr;
            //result = m_vertexBuffer->Map(0u, nullptr, &mappedPtr);
            //if (FAILED(result)) {
            //    throw std::runtime_error(std::to_string(result));
            //}

            //memcpy(mappedPtr, m_vertices.data(), m_vertices.size() * sizeof(Vertex));
            //m_vertexBuffer->Unmap(0u, nullptr);

            //D3D12_VERTEX_BUFFER_VIEW vbView = { };
            //m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
            //m_vertexBufferView.SizeInBytes = m_vertices.size() * sizeof(Vertex);
            //m_vertexBufferView.StrideInBytes = sizeof(Vertex);
        }

        auto GetVertexBuffer(void) const -> const D3D12_VERTEX_BUFFER_VIEW & {
            return m_vertexBufferView;
        }

        auto GetVertices(void) const -> const std::vector<Vertex> & {
            return m_vertices;
        }

        auto GetVertexCount(void) const -> uint32_t {
            return m_vertices.size();
        }

        auto GetPosition(void) const -> Vec2F {
            return { 0.0f, 0.0f };
        }

    private:
        Microsoft::WRL::ComPtr<ID3D12Device> m_device;

        Microsoft::WRL::ComPtr<ID3D12Resource> m_vertexBuffer;

        D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

        std::vector<Vertex> m_vertices;
    };

} // namespace Lightning3D

#endif