#include "LGBuffer.h"

using namespace Lightning3D;

Buffer::Buffer(uint32_t size, Usage usage)
    : m_size(size)
    , Resource(usage, 0) {

}

Buffer::~Buffer() {

}