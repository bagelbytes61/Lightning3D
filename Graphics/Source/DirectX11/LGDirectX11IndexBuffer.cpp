#include "DirectX11/LGDirectX11IndexBuffer.h"

#include "DirectX11/LGDirectX11Usage.h"

#include <exception>

using namespace Lightning3D;

DirectX11IndexBuffer::DirectX11IndexBuffer(ID3D11Buffer* resource, uint32_t size, uint32_t count, Usage usage)
    : m_resource(resource)
    , IndexBuffer(size, count, usage) {

}

DirectX11IndexBuffer::~DirectX11IndexBuffer() {

}