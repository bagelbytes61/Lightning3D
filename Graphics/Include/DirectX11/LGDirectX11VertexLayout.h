#ifndef LG_DIRECTX11_VERTEX_LAYOUT_H_
#define LG_DIRECTX11_VERTEX_LAYOUT_H_

#pragma once

#include "LGVertexLayout.h"

#include <d3d11.h>

#include <atlbase.h>

namespace Lightning3D {
    class DirectX11VertexLayout : public VertexLayout {
    public:
        virtual ~DirectX11VertexLayout() override final = default;

    private:
        CComPtr<ID3D11InputLayout> m_inputLayout;
    };
}

#endif