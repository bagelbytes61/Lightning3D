#include "DirectX11/LGDirectX11VertexBuffer.h"

#include "DirectX11/LGDirectX11Usage.h"

#include <exception>

using namespace Lightning3D;

DirectX11VertexBuffer::DirectX11VertexBuffer(ID3D11Buffer* resource, uint32_t size, uint32_t count, Usage usage)
    : m_resource(resource)
    , VertexBuffer(size, count, usage) {

}

DirectX11VertexBuffer::~DirectX11VertexBuffer() {

}