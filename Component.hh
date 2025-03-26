#ifndef LIGHTNING3D_COMPONENT_HH_
#define LIGHTNING3D_COMPONENT_HH_

#include <cstdint>
#include <utility>
#include <variant>

#include "Components/AxisAlignedBoundingBox.hh"
#include "Components/Sprite.hh"
#include "Components/Transform.hh"

namespace Lightning3D {
    struct ComponentTag {
        enum {
            Default = 1u,

            LocalTransform = 1u << 1,
            WorldTransform = 1u << 2,

            Any = ~0u
        };
    };

    using Component = std::pair<std::variant<AxisAlignedBoundingBox, Transform, StaticMesh, std::monostate>, uint64_t>;

}

#endif