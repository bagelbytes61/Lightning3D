#ifndef LE_ENGINE_H_
#define LE_ENGINE_H_

#pragma once

#include <memory>

namespace Lightning3D::Engine {
    class Engine {
        friend class PlatformApplication;
    public:
        static void Run();
        
    private:
        static inline bool s_running = true;

        static std::shared_ptr<class PlatformApplication>  s_application;

        static std::shared_ptr<class PlatformWindow> s_window;
    };
}

#endif