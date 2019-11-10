#ifndef LG_USAGE_H_
#define LG_USAGE_H_

#pragma once

#include <cstdint>

namespace Lightning3D {
    enum struct Usage : uint32_t {
        Default,
        Dynamic,
        Static
    };
}

#endif