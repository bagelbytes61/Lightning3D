#ifndef LG_UNIFORM_BUFFER_H_
#define LG_UNIFORM_BUFFER_H_

#pragma once

#include "LGBuffer.h"

namespace Lightning3D {
    class UniformBuffer : public Buffer {
    public:

    protected:
        UniformBuffer(uint32_t size, Usage usage);
        virtual ~UniformBuffer() override;

    private:

    };
}

#endif