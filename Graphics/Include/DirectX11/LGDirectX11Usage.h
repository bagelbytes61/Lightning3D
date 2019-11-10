#ifndef LG_DIRECTX11_USAGE_H_
#define LG_DIRECTX11_USAGE_H_

#pragma once

#include "LGUsage.h"

#include <d3d11.h>

#include <unordered_map>

namespace Lightning3D {
    static std::unordered_map<Usage, D3D11_USAGE> d3d11Usage = {
        { Usage::Default, D3D11_USAGE_DEFAULT },
        { Usage::Dynamic, D3D11_USAGE_DYNAMIC },
        { Usage::Static,  D3D11_USAGE_IMMUTABLE },
    };

}

#endif