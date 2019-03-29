//#include "DirectX11/LGDirectX11IndexBuffer.h"
//
////#include "DirectX11/LGDirectX11.h"
//
//#include <exception>
//
//using namespace Lightning3D::Graphics;
//
//DirectX11IndexBuffer::DirectX11IndexBuffer(uint32_t indexSize, uint32_t indexCount, uint32_t  topology, uint32_t usage)
//    : m_resource(nullptr)
//    , IndexBufferBase(indexSize, indexCount, topology, usage) {
//
//    D3D11_BUFFER_DESC bDesc = { 0 };
//    bDesc.ByteWidth = m_data.size();
//   // bDesc.Usage = static_cast<D3D11_USAGE>(m_usage);
//    bDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//    bDesc.CPUAccessFlags = 0;
//    bDesc.MiscFlags = 0;
//
//    D3D11_SUBRESOURCE_DATA bInit = { 0 };
//    bInit.pSysMem = m_data.data();
//    bInit.SysMemPitch = 0;
//    bInit.SysMemSlicePitch = 0;
//
//    //HRESULT hResult = DirectX11::GetDevice()->CreateBuffer(&bDesc, &bInit, &m_resource);
//    //if (FAILED(hResult)) {
//
//        //throw std::runtime_error("Failed to create index buffer!");
//    //}
//
//    m_format = m_indexSize == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
//}
//
//DirectX11IndexBuffer::~DirectX11IndexBuffer() {
//
//}
//
//void DirectX11IndexBuffer::Render(uint32_t startVertex, uint32_t primitiveCount) {
//    //DirectX11::GetDeviceContext()->IASetPrimitiveTopology(static_cast<D3D11_PRIMITIVE_TOPOLOGY>(m_topology));
//    //DirectX11::GetDeviceContext()->IASetIndexBuffer(m_resource, m_format, 0);
//    //DirectX11::GetDeviceContext()->DrawIndexed(m_indexCount, 0, startVertex);
//}