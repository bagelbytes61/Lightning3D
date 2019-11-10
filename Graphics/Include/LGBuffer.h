#ifndef LG_BUFFER_H_
#define LG_BUFFER_H_

#pragma once

#include "LGResource.h"

namespace Lightning3D {
    class Buffer : Resource {
    public:
        uint32_t GetSize() const;

    protected:
        Buffer(uint32_t size,  Usage usage);
        virtual ~Buffer() override;

    private:
        uint32_t m_size;
    };

    inline uint32_t Buffer::GetSize() const {
        return m_size;
    }
}

#endif