#ifndef LG_DIRECTX11_GRAPHICS_H_
#define LG_DIRECTX11_GRAPHICS_H_

#pragma once#

//#include "LGGraphics.h"

#include <d3d11.h>
#include <dxgi.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "dxgi.lib")

#include <atlbase.h>
#include <cstdint>

namespace Lightning3D::Graphics {
    //class DirectX11Graphics : public Graphics {
    //public:
    //    DirectX11Graphics();
    //    virtual ~DirectX11Graphics() override final;

    //    virtual void Clear(ClearFlag flags) override final;

    //    virtual void Present() override final;

    //private:
    //    CComPtr<ID3D11Device> m_device;

    //    CComPtr<ID3D11DeviceContext> m_deviceContext;

    //    CComPtr<IDXGISwapChain> m_swapChain;

    //    CComPtr<ID3D11RenderTargetView>  m_renderTarget;

    //    CComPtr<ID3D11DepthStencilView>  m_depthStencil;
    //};

    //inline void DirectX11Graphics::Clear(uint32_t _ui32Flags) {
    //    if (_ui32Flags & LG_CLEAR_COLOR) {
    //        const FLOAT fColor[4] = {
    //            0.0f,
    //            0.0f,
    //            0.0f,
    //            1.0f
    //        };

    //        s_pDeviceContext->ClearRenderTargetView(s_pRenderTarget, fColor);
    //    }

    //    s_pDeviceContext->ClearDepthStencilView(s_pDepthStencil, _ui32Flags & (LG_CLEAR_DEPTH | LG_CLEAR_STENCIL), 1.0f, 0);
    //}

    //inline void DirectX11Graphics::Present() {
    //    s_pSwapChain->Present(0, 0);
    //}

    //inline ID3D11Device* DirectX11Graphics::GetDevice() {
    //    return s_pDevice;
    //}

    //inline ID3D11DeviceContext* DirectX11Graphics::GetDeviceContext() {
    //    return s_pDeviceContext;
    //}
}

#endif