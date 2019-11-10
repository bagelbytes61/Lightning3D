#include "LGVertexBuffer.h"

using namespace Lightning3D;

VertexBuffer::VertexBuffer(uint32_t size, uint32_t count, Usage usage)
    : m_size(size)
    , m_count(count)
    , Buffer(m_size * m_count, usage) {

}

VertexBuffer::~VertexBuffer() {

}