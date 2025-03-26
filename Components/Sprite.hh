#ifndef LIGHTNING3D_COMPONENTS_SPRITE_HH_
#define LIGHTNING3D_COMPONENTS_SPRITE_HH_

#include "../Vector.hh"

#include <vector>

namespace Lightning3D {
    struct MaterialHandle {

    };

    struct StaticMesh {
        StaticMesh(const std::vector<Vec2F> &points, const Vec4F &color)
            : points(points)
            , color(color) {
        }

        std::vector<Vec2F> points;

        Vec4F color;
    
        //MaterialHandle material = DEFAULT_MATERIAL;
    };

    inline auto Rectangle(float width, float height, const Vec4F &color = Vec4F(0.5f, 0.5f, 0.5f, 1.0f)) -> StaticMesh {
        const auto points = std::vector<Vec2F>{
            Vec2F(0.0f, height), Vec2F(width, 0.0f), Vec2F(0.0f, 0.0f),
            Vec2F(0.0f, height), Vec2F(width, height), Vec2F(width, 0.0f)
        };

        return StaticMesh(points, color);
    }

    struct Sprite {
        Vec3F color;
    };
}

#endif

//            { Vec3F(0.0f, height, 0.0f) },
//            { Vec3F(width, 0.0f, 0.0f) },
//            { Vec3F(0.0f, 0.0f, 0.0f) },

//            { Vec3F(0.0f, height, 0.0f) },
//            { Vec3F(width, height, 0.0f) },
//            { Vec3F(width, 0.0f, 0.0f) }