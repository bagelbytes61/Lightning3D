#ifndef LG_OBJECT_H_
#define LG_OBJECT_H_

#pragma once

#include <atomic>
#include <cstdint>

namespace Lightning3D {
    class Object {
    public:
        Object();
        virtual ~Object();
        
        int32_t AddRef();

        int32_t DecRef();

        int32_t GetRefCount() const;

    private:
        std::atomic<int32_t> m_refCount;
    };

    inline Object::Object()
        : m_refCount(1) {

    }

    inline Object::~Object() {

    }

    inline int32_t Object::AddRef() {
        return ++m_refCount;
    }

    inline int32_t Object::DecRef() {
        int32_t refCount = --m_refCount;
        if (m_refCount <= 0) {
            delete this;
        }

        return refCount;
    }

    inline int32_t Object::GetRefCount() const {
        return m_refCount;
    }
}

#endif