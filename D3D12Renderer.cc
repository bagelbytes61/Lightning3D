#include "D3D12Renderer.hh"
#include "Sprite.hh"

#include "Material.hh"
#include "Matrix.hh"
#include "WindowsWindow.hh"

#include <array>
#include <stdexcept>
#include <string>

#include <d3dcompiler.h>

#include <iostream>

using namespace Lightning3D;

D3D12Renderer::D3D12Renderer(const WindowsWindow *window) {
    HRESULT result;

    result = D3D12GetDebugInterface(IID_PPV_ARGS(&m_debugController));
    if (FAILED(result)) {
        throw std::runtime_error("Failed to acquire D3D12 Debug Interface: " + std::to_string(result));
    }

    m_debugController->EnableDebugLayer();

    Microsoft::WRL::ComPtr<IDXGIFactory4> factory;

    result = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&factory));
    if (FAILED(result)) {
        throw std::runtime_error("Failed to create DXGI Factory: " + std::to_string(result));
    }

    Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;

    for (unsigned int adapter_index = 0U; ; ++adapter_index) {
        if (DXGI_ERROR_NOT_FOUND == factory->EnumAdapters(adapter_index, &adapter)) {
            break;
        }

        if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_1, __uuidof(ID3D12Device), nullptr))) {
            break;
        }
    }

    if (adapter == nullptr) {
        throw std::runtime_error("Could not find adapter that supports requested feature level");
    }

    result = D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_1, IID_PPV_ARGS(&m_device));
    if (FAILED(result)) {
        throw std::runtime_error("Failed to create D3D12 Device: " + std::to_string(result));
    }

    {
        D3D12_COMMAND_QUEUE_DESC desc = { };
        desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

        result = m_device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_commandQueue));
        if (FAILED(result)) {
            throw std::runtime_error("Failed to create Command Queue: " + std::to_string(result));
        }
    }

    {
        Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain;

        DXGI_SWAP_CHAIN_DESC1 desc = { };
        desc.Width = window->GetWidth();
        desc.Height = window->GetHeight();
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1U;
        desc.SampleDesc.Quality = 0U;
        desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        desc.BufferCount = 2U;
        desc.Scaling = DXGI_SCALING_NONE;
        desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
        desc.Flags = 0U;

        result = factory->CreateSwapChainForHwnd(m_commandQueue.Get(), window->GetHandle(), &desc, nullptr, nullptr, &swapChain);
        if (FAILED(result)) {
            throw std::runtime_error("Failed to create Swap Chain: " + std::to_string(result));
        }

        swapChain.As(&m_swapChain);
    }

    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = { };
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        desc.NumDescriptors = 2u;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        desc.NodeMask = 0U;

        m_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_rtvDescriptorHeap));
        if (FAILED(result)) {
            throw std::runtime_error("Failed to create RTV Descriptor Heap: " + std::to_string(result));
        }
    }

    {
        auto descriptor = m_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

        const auto descriptorHandleIncrementSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

        for (unsigned int index = 0u; index < 2u; ++index) {
            result = m_swapChain->GetBuffer(index, IID_PPV_ARGS(&m_renderTargets[index]));
            if (FAILED(result)) {
                throw std::runtime_error("Failed to acquire back buffer " + std::to_string(index) + ":" + std::to_string(result));
            }

            m_device->CreateRenderTargetView(m_renderTargets[index].Get(), nullptr, descriptor);

            descriptor.ptr += descriptorHandleIncrementSize;
        }
    }

    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = { };
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        desc.NumDescriptors = 1u;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

        result = m_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_cbDescriptorHeap));
        if (FAILED(result)) {
            throw std::runtime_error("Failed to create CBV Descriptor Heap: " + std::to_string(result));
        }
    }

    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = { };
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
        desc.NumDescriptors = 1u;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

        result = m_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_dsDescriptorHeap));
        if (FAILED(result)) {
            throw std::runtime_error("Failed to create DSV Descriptor Heap: " + std::to_string(result));
        }
    }

    {
        D3D12_HEAP_PROPERTIES heapProperties = { };
        heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

        D3D12_RESOURCE_DESC resourceDesc = { };
        resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        resourceDesc.Alignment = 0u;
        resourceDesc.Width = window->GetWidth();
        resourceDesc.Height = window->GetHeight();
        resourceDesc.DepthOrArraySize = 1u;
        resourceDesc.MipLevels = 0u;
        resourceDesc.Format = DXGI_FORMAT_D32_FLOAT;
        resourceDesc.SampleDesc.Count = 1u;
        resourceDesc.SampleDesc.Quality = 0u;
        resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
        resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

        D3D12_CLEAR_VALUE clearValue = { };
        clearValue.Format = DXGI_FORMAT_D32_FLOAT;
        clearValue.DepthStencil.Depth = 1.0f;
        clearValue.DepthStencil.Stencil = 0u;

        result = m_device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &clearValue, IID_PPV_ARGS(&m_depthStencilBuffer));
        if (FAILED(result)) {
            throw std::runtime_error("Failed to create Depth Stencil Buffer Resource: " + std::to_string(result));
        }
    }

    {
        D3D12_DEPTH_STENCIL_VIEW_DESC desc = { };
        desc.Format = DXGI_FORMAT_D32_FLOAT;
        desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
        desc.Flags = D3D12_DSV_FLAG_NONE;
        desc.Texture2D.MipSlice = 0u;
    
        m_device->CreateDepthStencilView(m_depthStencilBuffer.Get(), &desc, m_dsDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
    }
    
    result = m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator));
    if (FAILED(result)) {
        throw std::runtime_error("Failed to create Command Allocator: " + std::to_string(result));
    }

    result = m_device->CreateCommandList(0U, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_graphicsCommandList));
    if (FAILED(result)) {
        throw std::runtime_error("Failed to create Command List: " + std::to_string(result));
    }

    m_graphicsCommandList->Close();

    result = m_device->CreateFence(0U, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
    if (FAILED(result)) {
        throw std::runtime_error("Failed to create Fence: " + std::to_string(result));
    }

    m_fenceEvent = CreateEvent(nullptr, false, false, nullptr);

    m_descriptorPool = std::make_unique<GPUDescriptorPool>(m_device.Get(), 256u);

    m_cbvPool = std::make_unique<ConstantBufferViewPool>(m_device.Get(), 2048u, 128u);

    m_vbvPool = std::make_unique<VertexBufferViewPool>(m_device, 512u, 4u * 1024u * 1024u, sizeof(Vertex));
}

D3D12Renderer::~D3D12Renderer() {

}

auto D3D12Renderer::Render(void) -> void {
    D3D12_RESOURCE_BARRIER barrier;
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = m_renderTargets[m_frameIndex].Get();
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    m_graphicsCommandList->ResourceBarrier(1U, &barrier);

    m_graphicsCommandList->Close();

    ID3D12CommandList *const commandLists[] = { m_graphicsCommandList.Get() };

    m_commandQueue->ExecuteCommandLists(1U, commandLists);

    m_swapChain->Present(1U, 0U);

    const int prevFrameIndex = m_frameIndex;
    WaitForPreviousFrame();

    auto &cbvs = m_inflightCBVs[prevFrameIndex];

    for (auto &&cbv : cbvs) {
        m_cbvPool->Release(cbv);
    }

    cbvs.clear();

    auto &vbvs = m_inflightVBVs[prevFrameIndex];

    for (auto &&vbv : vbvs) {
        m_vbvPool->Release(vbv);
    }

    vbvs.clear();
}

auto D3D12Renderer::Draw(const std::vector<Vertex> &vertices, const Mat4x4 &worldTransform, Material *material, ID3D12PipelineState *pipelineState) -> void {
    assert(m_viewport != nullptr);

    const auto camera = m_viewport->GetCamera();

    assert(camera != nullptr);

    m_graphicsCommandList->SetPipelineState(pipelineState);

    auto vbv = m_vbvPool->Request();

    {
        auto mappedPtr = vbv.Map();

        memcpy(mappedPtr, vertices.data(), vertices.size() * sizeof(Vertex));

        vbv.Unmap();
    }

    m_inflightVBVs[m_frameIndex].push_back(vbv);

    if (material != nullptr) { 
        const auto view = camera->GetViewMatrix();
        const auto projection = camera->GetProjectionMatrix();

        const auto wvp = worldTransform * view * projection;

        material->SetMat4x4Variable("LIGHTNING3D_MATRIX_WVP", wvp.Transpose());

        std::array<std::span<ConstantBuffer *>, 2u> constantBuffers = {
            {
                material->GetVSConstantBuffers(),
                material->GetPSConstantBuffers()
            }
        };

        {
            size_t index = 0;;

            for (auto cb : constantBuffers[0]) {
                auto cbv = m_cbvPool->Request();

                auto mappedPtr = cbv.Map();

                cb->Upload(mappedPtr);

                cbv.Unmap();

                m_graphicsCommandList->SetGraphicsRootConstantBufferView(index, cbv.GetGPUVirtualAddress());

                m_inflightCBVs[m_frameIndex].push_back(cbv);

                index += 1u;
            }

            for (auto cb : constantBuffers[1]) {
                auto cbv = m_cbvPool->Request();

                auto mappedPtr = cbv.Map();

                cb->Upload(mappedPtr);

                cbv.Unmap();

                m_graphicsCommandList->SetGraphicsRootConstantBufferView(index, cbv.GetGPUVirtualAddress());

                m_inflightCBVs[m_frameIndex].push_back(cbv);

                index += 1u;
            }
        }
    }

    m_graphicsCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    const D3D12_VERTEX_BUFFER_VIEW vbs[] = { vbv.GetView() };

    m_graphicsCommandList->IASetVertexBuffers(0u, 1u, vbs);
    m_graphicsCommandList->DrawInstanced(vertices.size(), 1u, 0u, 0u);
}
