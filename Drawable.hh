#ifndef LIGHTNING3D_DRAWABLE_HH_
#define LIGHTNING3D_DRAWABLE_HH_

#include "Matrix.hh"
#include "Vector.hh"
#include "Vertex.hh"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <ranges>
#include <span>
#include <unordered_map>
#include <vector>

#include <d3d12.h>
#include <d3d12shader.h>

#include <wrl/client.h>

namespace Lightning3D {
    class Material;

    class Drawable {
    public:
        virtual ~Drawable() = default;

        //virtual auto GetPipelineState(void) const->PipelineState;

        //virtual auto GetVertexBuffer(void) const -> const D3D12_VERTEX_BUFFER_VIEW & = 0;

        virtual auto GetVertices(void) const -> const std::vector<Vertex> & = 0;

        virtual auto GetVertexCount(void) const -> uint32_t = 0;

        virtual auto GetWorldTransform(void) const -> Mat4x4 = 0;

        virtual auto GetPosition(void) const -> Vec2F = 0;

        virtual auto GetScale(void) const -> Vec2F = 0;

        auto SetMaterial(Material *material) -> void {
            m_material = material;
        }

        auto GetMaterial(void) -> Material * {
            return m_material;
        }

    private:
        Material *m_material{ nullptr };
    };
}

#endif