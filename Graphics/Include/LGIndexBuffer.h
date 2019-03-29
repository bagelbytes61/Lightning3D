#ifndef LG_IINDEX_BUFFER_H_
#define LG_IINDEX_BUFFER_H_

#pragma once

#include "LGResource.h"

#include <cstdint>

namespace Lightning3D::Graphics {
    class IndexBuffer : public Resource {
    public:
        virtual ~IndexBuffer() override;

        virtual uint32_t GetIndexSize() const = 0;

        virtual uint32_t GetIndexCount() const = 0;

        virtual uint32_t GetPrimitiveCount() const = 0;

        virtual uint32_t GetTopology() const = 0;
    };

    inline IndexBuffer::~IndexBuffer() {

    }
}

#endif