#include "DirectX11/LGDirectX11Graphics.h"

using namespace Lightning3D::Graphics;

//DirectX11Graphics::DirectX11Graphics() {
//    HRESULT hResult;
//
//    CComPtr<IDXGIFactory> pFactory;
//    
//    hResult = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&pFactory));
//    if (FAILED(hResult)) {
//
//    }
//
//    hResult = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG,
//        nullptr, 0, D3D11_SDK_VERSION, &m_device, nullptr, &m_deviceContext);
//    if (FAILED(hResult)) {
//
//    }
//
//    RECT clientArea = { 0 };
//    ::GetClientRect(_lpdiInit->hWnd, &clientArea);
//
//    DXGI_SWAP_CHAIN_DESC scDesc = { 0 };
//    scDesc.BufferDesc.Width = clientArea.right - clientArea.left;
//    scDesc.BufferDesc.Height = clientArea.bottom - clientArea.top;
//    scDesc.BufferDesc.RefreshRate = { 0, 0 };
//    scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
//    scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;
//    scDesc.SampleDesc.Count = 1;
//    scDesc.SampleDesc.Quality = 0;
//    scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//    scDesc.BufferCount = 2;
//    scDesc.OutputWindow = _lpdiInit->hWnd;
//    scDesc.Windowed = _lpdiInit->bWindowed;
//    scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
//    scDesc.Flags = 0;
//
//    hResult = pFactory->CreateSwapChain(m_device, &scDesc, &m_swapChain);
//    if (FAILED(hResult)) {
//
//    }
//}
//
//DirectX11Graphics::~DirectX11Graphics() {
//
//}
//
//void DirectX11Graphics::Clear(ClearFlag flags) {
//    if ((flags & ClearFlag::Color) == ClearFlag::Color) {
//        FLOAT color[] = {
//            0.0f,
//            0.0f,
//            0.0f,
//            1.0f
//        };
//
//        m_deviceContext->ClearRenderTargetView(m_renderTarget, color);
//    }
//}