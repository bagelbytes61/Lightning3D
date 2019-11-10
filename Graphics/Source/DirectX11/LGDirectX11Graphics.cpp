#include "DirectX11/LGDirectX11Graphics.h"

#include "DirectX11/LGDirectX11IndexBuffer.h"
#include "DirectX11/LGDirectX11Usage.h"
#include "DirectX11/LGDirectX11RenderTarget.h"
#include "DirectX11/LGDirectX11VertexBuffer.h"

#include <stdexcept>
#include <string>

using namespace Lightning3D;


DirectX11Graphics::DirectX11Graphics(Handle winHandle, uint32_t width, uint32_t height, bool windowed) {
    HRESULT hResult;

    CComPtr<IDXGIFactory> factory;
    
    hResult = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void **>(&factory));
    if (FAILED(hResult)) {
        throw std::runtime_error("CreateDXGIFactory failed w/ error: ");
    }

    hResult = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG,
        nullptr, 0, D3D11_SDK_VERSION, &m_device, nullptr, &m_deviceContext);
    if (FAILED(hResult)) {
        throw std::runtime_error("D3D11CreateDevice failed w/ error: ");
    }

    RECT clientArea = { 0 };
    ::GetClientRect(reinterpret_cast<HWND>(winHandle), &clientArea);

    DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
    swapChainDesc.BufferDesc.Width = clientArea.right - clientArea.left;
    swapChainDesc.BufferDesc.Height = clientArea.bottom - clientArea.top;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate = { 60, 1 };
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 2;
    swapChainDesc.OutputWindow = reinterpret_cast<HWND>(winHandle);
    swapChainDesc.Windowed = windowed;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.Flags = 0;

    hResult = factory->CreateSwapChain(m_device, &swapChainDesc, &m_swapChain);
    if (FAILED(hResult)) {
        throw std::runtime_error("IDXGIFactory::CreateSwapChain failed w/ error: ");
    }

    CComPtr<ID3D11Texture2D> backbuffer;
    hResult = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&backbuffer));
    if (FAILED(hResult)) {
        throw std::runtime_error("IDXGISwapChain::GetBuffer failed w/ error: " + std::to_string(hResult));
    }
    
    hResult = m_device->CreateRenderTargetView(backbuffer, nullptr, &m_renderTarget);
    if (FAILED(hResult)) {
        throw std::runtime_error("ID3D11Device:CreateRenderTarget failed w/ error: ");
    }
}

DirectX11Graphics::~DirectX11Graphics() {

}

VertexBuffer *DirectX11Graphics::CreateVertexBuffer(uint32_t size, uint32_t count, Usage usage, void* initialData) {
    ID3D11Buffer* resource = nullptr;

    D3D11_BUFFER_DESC desc = { 0 };
    desc.ByteWidth = size * count;
    desc.Usage = d3d11Usage[usage];
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags = usage == Usage::Dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
    desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA init = { 0 };
    init.pSysMem = initialData;
    init.SysMemPitch = 0;
    init.SysMemSlicePitch = 0;

    HRESULT hResult = m_device->CreateBuffer(&desc, &init, &resource);
    if (FAILED(hResult)) {
        throw std::runtime_error("ID3D11Device::CreateBuffer failed w/ error: ");
    }

    return new DirectX11VertexBuffer(resource, size, count, usage);
}

IndexBuffer *DirectX11Graphics::CreateIndexBuffer(uint32_t size, uint32_t count, Usage usage, void* initialData) {
    ID3D11Buffer* resource = nullptr;

    D3D11_BUFFER_DESC desc = { 0 };
    desc.ByteWidth = size * count;
    desc.Usage = d3d11Usage[usage];
    desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    desc.CPUAccessFlags = usage == Usage::Dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
    desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA init = { 0 };
    init.pSysMem = initialData;
    init.SysMemPitch = 0;
    init.SysMemSlicePitch = 0;

    HRESULT hResult = m_device->CreateBuffer(&desc, &init, &resource);
    if (FAILED(hResult)) {
        throw std::runtime_error("ID3D11Device::CreateBuffer failed w/ error: ");
    }

    return new DirectX11IndexBuffer(resource, size, count, usage);
}

UniformBuffer *DirectX11Graphics::CreateUniformBuffer(uint32_t size, Usage usage, void* initialData) {
    return nullptr;
}

Texture1D *DirectX11Graphics::CreateTexture1D(uint32_t width, Usage usage) {
    return nullptr;
}

Texture2D *DirectX11Graphics::CreateTexture2D(uint32_t width, uint32_t height, Usage usage) {
    return nullptr;
}

Texture3D *DirectX11Graphics::CreateTexture3D(uint32_t width, uint32_t height, uint32_t depth, Usage usage) {
    return nullptr;
}

DepthStencil *DirectX11Graphics::CreateDepthStencil(Texture2D* texture) {
    return nullptr;
}

RenderTarget *DirectX11Graphics::CreateRenderTarget(Texture2D* texture) {
    ID3D11RenderTargetView *resource = nullptr;

    m_device->CreateRenderTargetView


    return new DirectX11RenderTarget(resource);
}

PipelineState *DirectX11Graphics::CreatePipelineState(PipelineStateDescription &&description) {
    return nullptr;
}

void DirectX11Graphics::ClearDepthStencil(DepthStencil *stencil) {

}

void DirectX11Graphics::ClearRenderTarget(RenderTarget *target) {
    const FLOAT clearColor[] = { 0.f, 0.f, 0.f, 1.f };

    if (!target) {
        m_deviceContext->ClearRenderTargetView(m_renderTarget, clearColor);
    }
    else {
        
    }
}

void DirectX11Graphics::Draw(Topology topology, uint32_t startVertex, uint32_t vertexCount) {
    m_deviceContext->Draw(vertexCount, startVertex);
}

void DirectX11Graphics::Draw(Topology topology, uint32_t baseVertex, uint32_t indexStart, uint32_t indexCount) {
    m_deviceContext->DrawIndexed(indexCount, indexStart, baseVertex);
}

void DirectX11Graphics::Present() {
    m_swapChain->Present(0, 0);
}