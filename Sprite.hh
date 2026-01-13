#ifndef LIGHTNING3D_SPRITE_HH_
#define LIGHTNING3D_SPRITE_HH_

#include "Drawable.hh"
#include "Vector.hh"
#include "Vertex.hh"

#include <stdexcept>
#include <string>
#include <vector>

#include <d3d12.h>

#include <wrl/client.h>

namespace Lightning3D {
    //class Sprite : public Drawable {
    //public:
    //    Sprite(const std::vector<Vertex> &vertices)
    //        : m_vertices(vertices) {
    //    }

    //    auto GetVertices(void) const -> const std::vector<Vertex> & {
    //        return m_vertices;
    //    }

    //    //auto GetVertexBuffer(void) const -> const D3D12_VERTEX_BUFFER_VIEW & override {
    //    //    return m_vertexBufferView;
    //    //}

    //    auto GetVertexCount(void) const -> uint32_t override {
    //        return m_vertices.size();
    //    }

    //    auto GetWorldTransform(void) const -> Mat4x4 {
    //        return MatrixTranslation(m_position.x, m_position.y, 0.0f) * MatrixScale(m_scale.x, m_scale.y, 1.0f);
    //    }

    //    auto GetPosition(void) const -> Vec2F override {
    //        return m_position;
    //    }

    //    auto GetScale(void) const->Vec2F override {
    //        return m_scale;
    //    }

    //    auto SetPosition(const Vec2F &position) -> void {
    //        m_position = position;
    //    }

    //    auto SetScale(const Vec2F &scale) -> void {
    //        m_scale = scale;
    //    }

    //private:
    //    std::vector<Vertex> m_vertices;

    //    Vec2F m_position = { 0.0f, 0.0f };
    //    Vec2F m_scale = { 1.0f, 1.0f };
    //};

    //class Rectangle : public Sprite {
    //public:
    //    Rectangle(float width, float height)
    //        : Sprite(GenerateVertices(width, height)) {

    //    }

    //private:
    //    static auto GenerateVertices(float width, float height) -> std::vector<Vertex> {
    //        return {
    //            { Vec3F(0.0f, height, 0.0f) },
    //            { Vec3F(width, 0.0f, 0.0f) },
    //            { Vec3F(0.0f, 0.0f, 0.0f) },

    //            { Vec3F(0.0f, height, 0.0f) },
    //            { Vec3F(width, height, 0.0f) },
    //            { Vec3F(width, 0.0f, 0.0f) }
    //        };
    //    }
    //};
}

#endif


//{ Vec3F(width / 2.0f, height / 2.0f, 0.0f) },
//{ Vec3F(-width / 2.0f, height / 2.0f, 0.0f) },
//{ Vec3F(-width / 2.0f, -height / 2.0f, 0.0f) },

//{ Vec3F(width / 2.0f, -height / 2.0f, 0.0f) },
//{ Vec3F(width / 2.0f, height / 2.0f, 0.0f) },
//{ Vec3F(-width / 2.0f, -height / 2.0f, 0.0f) }

//HRESULT result;

//D3D12_HEAP_PROPERTIES heapProperties = { };
//heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;

//D3D12_RESOURCE_DESC vertexBufferDesc = { };
//vertexBufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//vertexBufferDesc.Alignment = 0u;
//vertexBufferDesc.Width = vertices.size() * sizeof(Vertex);
//vertexBufferDesc.Height = 1u;
//vertexBufferDesc.DepthOrArraySize = 1u;
//vertexBufferDesc.MipLevels = 1u;
//vertexBufferDesc.Format = DXGI_FORMAT_UNKNOWN;
//vertexBufferDesc.SampleDesc.Count = 1u;
//vertexBufferDesc.SampleDesc.Quality = 0u;
//vertexBufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//vertexBufferDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

//result = device->CreateCommittedResource(
//    &heapProperties,
//    D3D12_HEAP_FLAG_NONE,
//    &vertexBufferDesc,
//    D3D12_RESOURCE_STATE_GENERIC_READ,
//    nullptr,
//    IID_PPV_ARGS(&m_vertexBuffer));
//if (FAILED(result)) {
//    throw std::runtime_error("Failed to create Vertex Buffer Resource: " + std::to_string(result));
//}

//D3D12_RANGE readRange = { };
//void *mappedVertexBufferPtr = nullptr;

//result = m_vertexBuffer->Map(0u, &readRange, &mappedVertexBufferPtr);
//if (FAILED(result)) {
//    throw std::runtime_error(std::to_string(result));
//}
//memcpy(mappedVertexBufferPtr, vertices.data(), vertices.size() * sizeof(Vertex));
//m_vertexBuffer->Unmap(0u, nullptr);

//m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
//m_vertexBufferView.SizeInBytes = vertices.size() * sizeof(Vertex);
//m_vertexBufferView.StrideInBytes = sizeof(Vertex);


//D3D12_HEAP_PROPERTIES heapProps = { };
//heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;

//D3D12_RESOURCE_DESC cbDesc = { };
//cbDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//cbDesc.Alignment = 0u;
//cbDesc.Width = 1024u * 64u;
//cbDesc.Height = 1u;
//cbDesc.DepthOrArraySize = 1u;
//cbDesc.MipLevels = 1u;
//cbDesc.Format = DXGI_FORMAT_UNKNOWN;
//cbDesc.SampleDesc.Count = 1u;
//cbDesc.SampleDesc.Quality = 0u;
//cbDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//cbDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

//result = device->CreateCommittedResource(
//    &heapProps,
//    D3D12_HEAP_FLAG_NONE,
//    &cbDesc,
//    D3D12_RESOURCE_STATE_GENERIC_READ,
//    nullptr,
//    IID_PPV_ARGS(&m_constantBuffer));

//if (FAILED(result)) {
//    throw std::runtime_error("Failed to create Constant Buffer Resource: " + std::to_string(result));
//}