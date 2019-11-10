#ifndef LG_DIRECTX11_VERTEX_BUFFER_H_
#define LG_DIRECTX11_VERTEX_BUFFER_H_

#pragma once

#include "LGVertexBuffer.h"

#include <d3d11.h>

#include <atlbase.h>

namespace Lightning3D {
    class DirectX11VertexBuffer : public VertexBuffer {
        friend class DirectX11Graphics;

    public:
        DirectX11VertexBuffer(ID3D11Buffer* resource, uint32_t size, uint32_t count, Usage usage);
        virtual ~DirectX11VertexBuffer() override final;

    private:
        CComPtr<ID3D11Buffer> m_resource;
    };
}

#endif