#ifndef LE_PLATFORM_APPLICATION_H_
#define LE_PLATFORM_APPLICATION_H_

#pragma once

#include "LEPlatformApplicationMessageHandler.h"

#include <cstdint>
#include <memory>

namespace Lightning3D {
    class PlatformWindow;

    class PlatformApplication : public PlatformApplicationMessageHandler {
    public:
        static std::unique_ptr<PlatformApplication> Create();

        virtual  ~PlatformApplication()  = 0;

        virtual void PumpMessages() = 0;

        virtual void SetMessageHandler(std::shared_ptr<PlatformApplicationMessageHandler> messageHandler) = 0;

        virtual void InitializeWindow(std::shared_ptr<PlatformWindow>& window, std::shared_ptr<struct PlatformWindowDescription> description) = 0;

    protected:

    private:
        virtual bool OnKeyDown(int32_t keyCode, uint32_t scanCode, InputModifier modifiers, bool repeat) override final;

        virtual bool OnKeyUp(int32_t keyCode, uint32_t scanCode, InputModifier modifiers) override final;
                                                                                                    
        virtual bool OnMouseDown(MouseButton button, int32_t x, int32_t y, InputModifier modifiers) override final;

        virtual bool OnMouseUp(MouseButton button, int32_t x, int32_t y, InputModifier modifiers) override final;

        virtual bool OnMouseDoubleClick(MouseButton button, int32_t x, int32_t y, InputModifier modifiers) override final;

        virtual bool OnMouseMove(int32_t x, int32_t y, InputModifier modifiers) override final;

        virtual void OnWindowClose() override final;

    protected:
        std::shared_ptr<PlatformApplicationMessageHandler> m_messageHandler;
    };

    inline void PlatformApplication::SetMessageHandler(std::shared_ptr<PlatformApplicationMessageHandler> messageHandler) {
        m_messageHandler = messageHandler;
    }
}

#endif