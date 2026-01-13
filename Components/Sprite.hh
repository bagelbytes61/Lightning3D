#ifndef LIGHTNING3D_COMPONENTS_SPRITE_HH_
#define LIGHTNING3D_COMPONENTS_SPRITE_HH_

#include "../Vector.hh"
#include "../Vertex.hh"

#include <vector>

namespace Lightning3D {
    class Material;
    
    struct MaterialHandle {

    };

    struct StaticMesh {
        StaticMesh() = default;

        StaticMesh(const std::vector<Vertex> &vertices, const Vec4F &color)
            : vertices(vertices)
            , color(color) {
        }

        std::vector<Vertex> vertices;

        Vec4F color;
    
        Material *material = nullptr;
        //MaterialHandle material = DEFAULT_MATERIAL;
    };

    inline auto Rectangle(float width, float height, const Vec4F &color = Vec4F(0.5f, 0.5f, 0.5f, 1.0f)) -> StaticMesh {
        const auto vertices = std::vector<Vertex>{
            Vertex({0.0f, height, 0.0f}), Vertex({width, 0.0f, 0.0f}), Vertex({0.0f, 0.0f, 0.0f}),
            Vertex({0.0f, height, 0.0f}), Vertex({width, height, 0.0}), Vertex({width, 0.0f, 0.0f})
        };

        return StaticMesh(vertices, color);
    }

    inline auto Box(float width, float height, float depth, const Vec4F &color = Vec4F(0.5f, 0.5f, 0.5f, 1.0f)) -> StaticMesh {
        const auto vertices = std::vector<Vertex>{
            // Front face
            Vertex({0.0f, height, 0.0f}), Vertex({width, 0.0f, 0.0f}), Vertex({0.0f, 0.0f, 0.0f}),
            Vertex({0.0f, height, 0.0f}), Vertex({width, height, 0.0f}), Vertex({width, 0.0f, 0.0f}),

            // Back face
            Vertex({0.0f, height, depth}), Vertex({width, 0.0f, depth}), Vertex({0.0f, 0.0f, depth}),
            Vertex({0.0f, height, depth}), Vertex({width, height, depth}), Vertex({width, 0.0f, depth}),

             //Top face
            Vertex({0.0f, height, depth}), Vertex({width, height, 0.0f}), Vertex({0.0f, height, 0.0f}),
            Vertex({0.0f, height, depth}), Vertex({width, height, depth}), Vertex({width, height, 0.0f}),

            // Bottom face
            Vertex({0.0f, 0.0f, depth}), Vertex({0.0f, 0.0f, 0.0f}), Vertex({width, 0.0f, depth}),
            Vertex({0.0f, 0.0f, 0.0f}), Vertex({width, 0.0f, 0.0f}), Vertex({width, 0.0f, depth}),

            // Left face
            Vertex({0.0f, 0.0f, depth}),  Vertex({0.0f, height, depth}), Vertex({0.0f, 0.0f, 0.0f}),
            Vertex({0.0f, height, depth}), Vertex({0.0f, height, 0.0f}), Vertex({0.0f, 0.0f, 0.0f}),

            // Right face
            Vertex({width, 0.0f, 0.0f}),  Vertex({width, height, 0.0f}), Vertex({width, 0.0f, depth}),
            Vertex({width, height, 0.0f}), Vertex({width, height, depth}), Vertex({width, 0.0f, depth})
        };

        return StaticMesh(vertices, color);
    }

    struct Sprite {
        Vec3F color;
    };
}

#endif

