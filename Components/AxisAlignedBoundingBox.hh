#ifndef LIGHTNING3D_COMPONENTs_AXIS_ALIGNED_BOUNDING_BOX_HH_
#define LIGHTNING3D_COMPONENTs_AXIS_ALIGNED_BOUNDING_BOX_HH_

#include "Transform.hh"

namespace Lightning3D {
    struct AxisAlignedBoundingBox {
        AxisAlignedBoundingBox(const Vec2F &center, const Vec2F &extents)
            : center(center)
            , extents(extents) {
        }

        auto Contains(const Vec2F &point) -> bool {
            if (transform != nullptr) {
                const auto worldTransform = transform->LocalToWorld();
                const auto worldCenter = Vec2F(worldTransform.position.x + center.x, worldTransform.position.y + center.y);

                return AxisAlignedBoundingBox(worldCenter, extents).Contains(point);
            }

            return point.x >= (center.x - extents.x)
                && point.y >= (center.y - extents.y)
                && point.x <= (center.x + extents.x)
                && point.y <= (center.y + extents.y);
        }

        Vec2F center;
        Vec2F extents;

        Transform *transform = nullptr;
    };
}

#endif