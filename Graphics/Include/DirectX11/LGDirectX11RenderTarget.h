#ifndef LG_DIRECTX11_RENDER_TARGET_H_
#define LG_DIRECTX11_RENDER_TARGET_H_

#pragma once

#include "LGRenderTarget.h"

#include <d3d11.h>

#include <atlbase.h>

namespace Lightning3D {
    class DirectX11RenderTarget : public RenderTarget {
        friend class DirectX11Graphics;
    public:
        DirectX11RenderTarget(ID3D11RenderTargetView *resource);
        virtual ~DirectX11RenderTarget() override = default;


    private:
        CComPtr<ID3D11RenderTargetView> m_resource;
    };
}

#endif