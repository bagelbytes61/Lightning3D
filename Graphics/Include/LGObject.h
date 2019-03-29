#ifndef LG_OBJECT_H_
#define LG_OBJECT_H_

#pragma once

#include <cstdint>

namespace Lightning3D::Graphics {
    class Object {
    public:
        virtual ~Object() {}

        virtual int32_t AddRef() = 0;

        virtual int32_t DecRef() = 0;

        virtual int32_t GetRefCount() const = 0;
    };
}

#endif