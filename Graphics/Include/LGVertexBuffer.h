#ifndef LG_VERTEX_BUFFER_H_
#define LG_VERTEX_BUFFER_H_

#pragma once

#include "LGResource.h"

#include <cstdint>

namespace Lightning3D::Graphics {
    class VertexBuffer : public Resource {
    public:
        virtual ~VertexBuffer() = 0;

        //virtual void Render(uint32_t _ui32VertexCount) = 0;
    };
}

#endif