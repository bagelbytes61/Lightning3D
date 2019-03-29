#include "LEEngine.h"

#include "LEPlatformApplication.h"
#include "LEPlatformFileSystem.h"
#include "LEPlatformTime.h"
#include "LEPlatformWindow.h"
#include "LEPlatformWindowDescription.h"

using namespace Lightning3D::Engine;

void Engine::Run() {
    PlatformTime::Initialize();

    s_application = PlatformApplication::Create();

    s_application->SetMessageHandler(s_application);

    auto windowDescription = std::make_shared<PlatformWindowDescription>();
    windowDescription->windowedWidth    = 1024UL;
    windowDescription->windowedHeight   = 768UL;
    windowDescription->fullscreenWidth  = 0UL;
    windowDescription->fullscreenHeight = 0UL;
    windowDescription->windowed         = true;
    windowDescription->title            = "Lightning3D";

    s_application->InitializeWindow(s_window, std::move(windowDescription));
    
    while (s_running) {
        s_application->PumpMessages();
    }
}

std::shared_ptr<PlatformApplication>    Engine::s_application;

std::shared_ptr<PlatformWindow>         Engine::s_window;