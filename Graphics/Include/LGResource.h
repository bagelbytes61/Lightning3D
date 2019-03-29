#ifndef LG_RESOURCE_H_
#define LG_RESOURCE_H_

#pragma once

#include "LGObject.h"

namespace Lightning3D::Graphics {
    enum struct Usage {

    };

    class Resource : public Object {
    public:
        virtual ~Resource() override;

        virtual Usage GetUsage() const = 0;

        virtual uint32_t GetId() const = 0;
    };

    inline Resource::~Resource() {

    }
}

#endif