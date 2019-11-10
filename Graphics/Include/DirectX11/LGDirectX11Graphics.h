#ifndef LG_DIRECTX11_GRAPHICS_H_
#define LG_DIRECTX11_GRAPHICS_H_

#pragma once#

#include "LGGraphics.h"

#include <d3d11.h>
#include <dxgi.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "dxgi.lib")

#include <atlbase.h>
#include <cstdint>

namespace Lightning3D {
    class DirectX11Graphics : public Graphics {
    public:
        DirectX11Graphics(Handle winHandle, uint32_t width, uint32_t height, bool windowed);
        virtual ~DirectX11Graphics() override;

        virtual VertexBuffer  *CreateVertexBuffer(uint32_t size, uint32_t count, Usage usage, void *initialData) override final;
        virtual IndexBuffer   *CreateIndexBuffer(uint32_t size, uint32_t count, Usage usage, void *initialData) override final;
        virtual UniformBuffer *CreateUniformBuffer(uint32_t size, Usage usage, void *initialData) override final;

        virtual Texture1D *CreateTexture1D(uint32_t width, Usage usage) override final;
        virtual Texture2D *CreateTexture2D(uint32_t width, uint32_t height, Usage usage) override final;
        virtual Texture3D *CreateTexture3D(uint32_t width, uint32_t height, uint32_t depth, Usage usage) override final;

        virtual DepthStencil *CreateDepthStencil(Texture2D *texture) override final;
        virtual RenderTarget *CreateRenderTarget(Texture2D *texture) override final;

        virtual PipelineState *CreatePipelineState(PipelineStateDescription &&description) override final;

        virtual void Draw(Topology topology, uint32_t startVertex, uint32_t vertexCount) override final;
        virtual void Draw(Topology topology, uint32_t baseVertex, uint32_t startIndex, uint32_t indexCount) override final;

        virtual void ClearDepthStencil(DepthStencil *stencil) override final;
        virtual void ClearRenderTarget(RenderTarget *target) override final;

        virtual void Present() override final;

    private:
        CComPtr<ID3D11Device> m_device;

        CComPtr<ID3D11DeviceContext> m_deviceContext;

        CComPtr<IDXGISwapChain> m_swapChain;

        CComPtr<ID3D11RenderTargetView>  m_renderTarget;

        CComPtr<ID3D11DepthStencilView>  m_depthStencil;
    };


}

#endif