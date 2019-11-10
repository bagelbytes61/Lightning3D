#include "LGIndexBuffer.h"

using namespace Lightning3D;

IndexBuffer::IndexBuffer(uint32_t size, uint32_t count, /*uint32_t  topology, */Usage usage)
    : m_size(size)
    , m_count(count)
    , Buffer(m_size * m_count, usage) {

}

IndexBuffer::~IndexBuffer() {

}