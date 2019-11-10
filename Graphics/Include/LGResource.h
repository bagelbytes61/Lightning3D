#ifndef LG_RESOURCE_H_
#define LG_RESOURCE_H_

#pragma once

#include "LGObject.h"

#include "LGUsage.h"

#include <cstdint>

namespace Lightning3D {
    class Resource  : public Object {
    public:
       Usage GetUsage() const;

       uint32_t GetId() const;

    protected:
        Resource(Usage _usage, uint32_t _id);
        virtual ~Resource() override;


    protected:
        Usage m_usage;

        uint32_t m_id;
    };

    inline Resource::Resource(Usage usage, uint32_t id)
        : m_usage(usage)
        , m_id(id) {

    }

    inline Resource::~Resource() {

    }

    inline Usage Resource::GetUsage() const {
        return m_usage;
    }

    inline uint32_t Resource::GetId() const {
        return m_id;
    }
}

#endif