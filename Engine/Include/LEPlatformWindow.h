#ifndef LE_PLATFORM_WINDOW_H_
#define LE_PLATFORM_WINDOW_H_

#pragma once

#include <memory>

namespace Lightning3D::Engine {
    using Handle = void*;

    class PlatformWindow {
    public:
        virtual ~PlatformWindow()  = 0;

        virtual Handle  GetHandle() const = 0;

    protected:
        PlatformWindow(std::shared_ptr<struct PlatformWindowDescription> description);

    protected:
        std::shared_ptr<PlatformWindowDescription> m_description;
    };
}

#endif