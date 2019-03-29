#ifndef LG_OBJECT_BASE_H_
#define LG_OBJECT_BASE_H_

#pragma once

#include "LGObject.h"

#include <atomic>
#include <cstdint>

namespace Lightning3D::Graphics {
    class ObjectBase : public Object {
    public:
        ObjectBase();
        virtual ~ObjectBase() override;
        
        virtual int32_t AddRef() override final;

        virtual int32_t DecRef() override final;

        virtual int32_t GetRefCount() const override final;

    private:
        std::atomic<int32_t> m_refCount;
    };

    inline ObjectBase::ObjectBase()
        : m_refCount(1) {

    }

    inline ObjectBase::~ObjectBase() {

    }

    inline int32_t ObjectBase::AddRef() {
        return ++m_refCount;
    }

    inline int32_t ObjectBase::DecRef() {
        int32_t refCount = --m_refCount;
        if (m_refCount <= 0) {
            delete this;
        }

        return refCount;
    }

    inline int32_t ObjectBase::GetRefCount() const {
        return m_refCount;
    }
}

#endif