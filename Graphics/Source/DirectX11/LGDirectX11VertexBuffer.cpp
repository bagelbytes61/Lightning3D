#include "DirectX11/LGDirectX11VertexBuffer.h"

//#include "DirectX11/LGDirectX11.h"
//
//#include <exception>
//
//using namespace Lightning3D::Graphics;
//
//DirectX11VertexBuffer::DirectX11VertexBuffer(uint32_t vertexSize, uint32_t vertexCount, uint32_t usage)
//    : m_resource(nullptr)
//    , VertexBufferBase(vertexSize, vertexCount, usage) {
//
//    D3D11_BUFFER_DESC bDesc = { 0 };
//    bDesc.ByteWidth = m_data.size();
//    bDesc.Usage = static_cast<D3D11_USAGE>(m_usage);
//    bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//    bDesc.CPUAccessFlags = 0;
//    bDesc.MiscFlags = 0;
//
//    D3D11_SUBRESOURCE_DATA bInit = { 0 };
//    bInit.pSysMem = m_data.data();
//    bInit.SysMemPitch = 0;
//    bInit.SysMemSlicePitch = 0;
//
//    HRESULT hResult = DirectX11::GetDevice()->CreateBuffer(&bDesc, &bInit, &m_resource);
//    if (FAILED(hResult)) {
//        throw std::runtime_error("Failed to create vertex buffer!");
//    }
//}
//
//DirectX11VertexBuffer::~DirectX11VertexBuffer() {
//
//}
//
//void DirectX11VertexBuffer::Render(uint32_t primitiveCount) {
//
//    //CDirectX11::GetDeviceContext()->Draw();
//}