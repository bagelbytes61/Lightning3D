#ifndef LG_INDEX_BUFFER_H_
#define LG_INDEX_BUFFER_H_

#pragma once

#include "LGBuffer.h"

#include <cstddef>
#include <cstdint>
#include <vector>

namespace Lightning3D {
    class IndexBuffer : public Buffer {
    public:
        virtual ~IndexBuffer() override;

        uint32_t GetIndexSize() const;

        uint32_t GetIndexCount() const;

    protected:
        IndexBuffer(uint32_t size, uint32_t count, Usage usage);

    private:
        uint32_t m_size;

        uint32_t m_count;

    };

    inline uint32_t IndexBuffer::GetIndexSize() const {
        return m_size;
    }

    inline uint32_t IndexBuffer::GetIndexCount() const {
        return m_count;
    }
}

#endif