#ifndef LG_VERTEX_BUFFER_BASE_H_
#define LG_VERTEX_BUFFER_BASE_H_

#pragma once

#include "LGBuffer.h"

#include <cstddef>
#include <cstdint>
#include <vector>

namespace Lightning3D {
    class VertexBuffer : public Buffer {
    public:
        ~VertexBuffer();

        uint32_t GetVertexSize() const;

        uint32_t GetVertexCount() const;

    protected:
        VertexBuffer(uint32_t size, uint32_t count, Usage usage);

    private:
        uint32_t m_size;

        uint32_t m_count;
    };

   inline uint32_t VertexBuffer::GetVertexSize() const {
        return m_size;
    }

    inline uint32_t VertexBuffer::GetVertexCount() const {
        return m_count;
    }
}

#endif