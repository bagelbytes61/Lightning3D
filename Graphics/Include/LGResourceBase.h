#ifndef LG_RESOURCE_BASE_H_
#define LG_RESOURCE_BASE_H_

#pragma once

#include "LGResource.h"

#include <cstdint>

namespace Lightning3D::Graphics {
    class ResourceBase  : public Resource {
    public:
        virtual Usage GetUsage() const override final;

        virtual uint32_t GetId() const override final;

    protected:
        ResourceBase(Usage _usage, uint32_t _id);
        virtual ~ResourceBase() override;


    protected:
        Usage m_usage;

        uint32_t m_id;
    };

    inline ResourceBase::ResourceBase(Usage usage, uint32_t id)
        : m_usage(usage)
        , m_id(id) {

    }

    inline ResourceBase::~ResourceBase() {

    }

    inline Usage ResourceBase::GetUsage() const {
        return m_usage;
    }

    inline uint32_t ResourceBase::GetId() const {
        return m_id;
    }
}

#endif