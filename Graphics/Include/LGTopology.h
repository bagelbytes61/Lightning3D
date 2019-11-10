#ifndef LG_TOPOLOGY_H_
#define LG_TOPOLOGY_H_

#pragma once

#include <cstdint>

namespace Lightning3D {
    enum struct Topology : uint32_t {
        PointList,
        LineList,
        LineStrip,
        TriangleList,
        TriangleStrip
    };
}

#endif