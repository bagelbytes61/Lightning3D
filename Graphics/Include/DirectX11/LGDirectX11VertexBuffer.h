#ifndef LG_DIRECTX11_VERTEX_BUFFER_H_
#define LG_DIRECTX11_VERTEX_BUFFER_H_

#pragma once

#include "LGVertexBuffer.h"
#include "LGVertexBufferBase.h"

#include <d3d11.h>

#include <atlbase.h>

namespace Lightning3D::Graphics {
    enum struct LG : uint32_t {
        SetOnly  = D3D11_USAGE_IMMUTABLE,
        Standard = D3D11_USAGE_DEFAULT,
        Dynamic  = D3D11_USAGE_DYNAMIC
    };

    //class DirectX11VertexBuffer : public VertexBuffer, VertexBufferBase {
    //public:
    //    DirectX11VertexBuffer(uint32_t _ui32VertexSize, uint32_t _ui32VertexCount, uint32_t _ui32Usage);
    //    ~DirectX11VertexBuffer();

    //    virtual void Render(uint32_t primitiveCount) override final;

    //private:
    //    CComPtr<ID3D11Buffer> m_resource;
    //};
}

#endif