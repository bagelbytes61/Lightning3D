#ifndef LG_DIRECTX11_INDEX_BUFFER_H_
#define LG_DIRECTX11_INDEX_BUFFER_H_

#pragma once

#include "LGIndexBuffer.h"

#include <d3d11.h>

#include <atlbase.h>

namespace Lightning3D {
    class DirectX11IndexBuffer : public IndexBuffer {
        friend class DirectX11Graphics;

    public:
        DirectX11IndexBuffer(ID3D11Buffer* resource, uint32_t size, uint32_t count, /*uint32_t topology, */Usage usage);
        virtual ~DirectX11IndexBuffer() override final;

    private:
        CComPtr<ID3D11Buffer> m_resource;
    };
}

#endif