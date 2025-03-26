#ifndef LIGHTNING3D_COMPONENTS_TRANSFORM_HH_
#define LIGHTNING3D_COMPONENTS_TRANSFORM_HH_

#include "../Vector.hh"

namespace Lightning3D {
    struct Position : Vec3F {
        using Vec3F::Vector;

    };

    struct Rotation : Vec3F {
        using Vec3F::Vector;

    };

    struct Scale : Vec3F {
        using Vec3F::Vector;

    };

    struct Transform {
        auto LocalToWorld(void) const -> Transform {
            auto worldTransform = *this;

            if (parent != nullptr) {
                auto parentWorldTransform = parent->LocalToWorld();

                worldTransform.position += parentWorldTransform.position;
                worldTransform.rotation += parentWorldTransform.rotation;

                worldTransform.scale.x *= parentWorldTransform.scale.x;
                worldTransform.scale.y *= parentWorldTransform.scale.y;
                worldTransform.scale.z *= parentWorldTransform.scale.z;
            }

            return worldTransform;
        }

        Position position;
        Rotation rotation;
        Scale scale = { 1.0f, 1.0f, 1.0f };

        Transform *parent = nullptr;
    };
}

#endif